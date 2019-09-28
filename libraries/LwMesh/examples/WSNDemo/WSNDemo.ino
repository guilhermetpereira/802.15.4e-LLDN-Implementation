#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

/*- Includes ---------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "config.h"
#include "hal.h"
#include "phy.h"
#include "sys.h"
#include "nwk.h"
#include <nwk\nwkRoute.h>
#include <nwk\nwkSecurity.h>
#include <sys\sysTimer.h>
#include <hal\halSleep.h>
#include "commands.h"
#include "conf.h"
#include <LwMesh.h>

/*- Definitions ------------------------------------------------------------*/
#if defined(APP_COORDINATOR)
  #define APP_NODE_TYPE     0
#elif defined(APP_ROUTER)
  #define APP_NODE_TYPE     1
#else
  #define APP_NODE_TYPE     2
#endif

#define APP_CAPTION_SIZE    (sizeof(APP_CAPTION) - 1)
#define APP_COMMAND_PENDING 0x01

#define APP_LED_NETWORK     0
#define APP_LED_DATA        1

/*- Types ------------------------------------------------------------------*/
typedef struct PACK
{
  uint8_t      commandId;
  uint8_t      nodeType;
  uint64_t     extAddr;
  uint16_t     shortAddr;
  uint32_t     softVersion;
  uint32_t     channelMask;
  uint16_t     panId;
  uint8_t      workingChannel;
  uint16_t     parentShortAddr;
  uint8_t      lqi;
  int8_t       rssi;

  struct PACK
  {
    uint8_t    type;
    uint8_t    size;
    int32_t    battery;
    int32_t    temperature;
    int32_t    light;
  } sensors;

  struct PACK
  {
    uint8_t    type;
    uint8_t    size;
    char       text[APP_CAPTION_SIZE];
  } caption;
} AppMessage_t;

typedef enum AppState_t
{
  APP_STATE_INITIAL,
  APP_STATE_SEND,
  APP_STATE_WAIT_CONF,
  APP_STATE_SENDING_DONE,
  APP_STATE_WAIT_SEND_TIMER,
  APP_STATE_WAIT_COMMAND_TIMER,
  APP_STATE_PREPARE_TO_SLEEP,
  APP_STATE_SLEEP,
  APP_STATE_WAKEUP,
} AppState_t;

/*- Variables --------------------------------------------------------------*/
static AppState_t appState = APP_STATE_INITIAL;

#if defined(APP_ROUTER) || defined(APP_ENDDEVICE)
static NWK_DataReq_t appNwkDataReq;
static SYS_Timer_t appNetworkStatusTimer;
static SYS_Timer_t appCommandWaitTimer;
static bool appNetworkStatus;
#endif

static AppMessage_t appMsg;
static SYS_Timer_t appDataSendingTimer;

/*- Implementations --------------------------------------------------------*/

/*************************************************************************//**
*****************************************************************************/
static void appUartProcess(void)
{
  while(Serial.available())
  {
    uint8_t byte = Serial.read();
    APP_CommandsByteReceived(byte);
  }
}
static void appUartSendMessage(uint8_t *data, uint8_t size)
{
  uint8_t cs = 0;

  Serial.write(0x10);
  Serial.write(0x02);

  for (uint8_t i = 0; i < size; i++)
  {
    if (data[i] == 0x10)
    {
	  Serial.write(0x10);
      cs += 0x10;
    }
	Serial.write(data[i]);
    cs += data[i];
  }

  Serial.write(0x10);
  Serial.write(0x03);
  cs += 0x10 + 0x02 + 0x10 + 0x03;

  Serial.write(cs);
}
bool rx_frame(NWK_DataInd_t *ind)
{
  AppMessage_t *msg = (AppMessage_t *)ind->data;

  HAL_LedToggle(APP_LED_DATA);

  msg->lqi = ind->lqi;
  msg->rssi = ind->rssi;

  appUartSendMessage(ind->data, ind->size);

  if (APP_CommandsPending(ind->srcAddr))
    NWK_SetAckControl(APP_COMMAND_PENDING);

  return true;
}
static void appDataSendingTimerHandler(SYS_Timer_t *timer)
{
  if (APP_STATE_WAIT_SEND_TIMER == appState)
    appState = APP_STATE_SEND;
  else
    SYS_TimerStart(&appDataSendingTimer);

  (void)timer;
}
#if defined(APP_ROUTER) || defined(APP_ENDDEVICE)
static void appNetworkStatusTimerHandler(SYS_Timer_t *timer)
{
  HAL_LedToggle(APP_LED_NETWORK);
  (void)timer;
}
static void appCommandWaitTimerHandler(SYS_Timer_t *timer)
{
  appState = APP_STATE_SENDING_DONE;
  (void)timer;
}
#endif
#if defined(APP_ROUTER) || defined(APP_ENDDEVICE)
static void appDataConf(NWK_DataReq_t *req)
{
  HAL_LedOff(APP_LED_DATA);

  if (NWK_SUCCESS_STATUS == req->status)
  {
    if (!appNetworkStatus)
    {
      HAL_LedOn(APP_LED_NETWORK);
      SYS_TimerStop(&appNetworkStatusTimer);
      appNetworkStatus = true;
    }
  }
  else
  {
    if (appNetworkStatus)
    {
      HAL_LedOff(APP_LED_NETWORK);
      SYS_TimerStart(&appNetworkStatusTimer);
      appNetworkStatus = false;
    }
  }

  if (APP_COMMAND_PENDING == req->control)
  {
    SYS_TimerStart(&appCommandWaitTimer);
    appState = APP_STATE_WAIT_COMMAND_TIMER;
  }
  else
  {
    appState = APP_STATE_SENDING_DONE;
  }
}
#endif
static void appSendData(void)
{
#ifdef NWK_ENABLE_ROUTING
  appMsg.parentShortAddr = NWK_RouteNextHop(0, 0);
#else
  appMsg.parentShortAddr = 0;
#endif

  appMsg.sensors.battery     = rand() & 0xffff;
  appMsg.sensors.temperature = rand() & 0x7f;
  appMsg.sensors.light       = rand() & 0xff;

#if defined(APP_COORDINATOR)
  appUartSendMessage((uint8_t *)&appMsg, sizeof(appMsg));
  SYS_TimerStart(&appDataSendingTimer);
  appState = APP_STATE_WAIT_SEND_TIMER;
#else
  appNwkDataReq.dstAddr = 0;
  appNwkDataReq.dstEndpoint = LwMeshDefaultEndPoint;
  appNwkDataReq.srcEndpoint = LwMeshDefaultEndPoint;
  appNwkDataReq.options = NWK_OPT_ACK_REQUEST | NWK_OPT_ENABLE_SECURITY;
  appNwkDataReq.data = (uint8_t *)&appMsg;
  appNwkDataReq.size = sizeof(appMsg);
  appNwkDataReq.confirm = appDataConf;

  HAL_LedOn(APP_LED_DATA);
  NWK_DataReq(&appNwkDataReq);

  appState = APP_STATE_WAIT_CONF;
#endif
}
static void appInit(void)
{
  appMsg.commandId            = APP_COMMAND_ID_NETWORK_INFO;
  appMsg.nodeType             = APP_NODE_TYPE;
  appMsg.extAddr              = APP_ADDR;
  appMsg.shortAddr            = APP_ADDR;
  appMsg.softVersion          = 0x01010100;
  appMsg.channelMask          = (1L << APP_CHANNEL);
  appMsg.panId                = APP_PANID;
  appMsg.workingChannel       = APP_CHANNEL;
  appMsg.parentShortAddr      = 0;
  appMsg.lqi                  = 0;
  appMsg.rssi                 = 0;

  appMsg.sensors.type        = 1;
  appMsg.sensors.size        = sizeof(int32_t) * 3;
  appMsg.sensors.battery     = 0;
  appMsg.sensors.temperature = 0;
  appMsg.sensors.light       = 0;

  appMsg.caption.type         = 32;
  appMsg.caption.size         = APP_CAPTION_SIZE;
  memcpy(appMsg.caption.text, APP_CAPTION, APP_CAPTION_SIZE);

  HAL_LedInit();

  appDataSendingTimer.interval = APP_SENDING_INTERVAL;
  appDataSendingTimer.mode = SYS_TIMER_INTERVAL_MODE;
  appDataSendingTimer.handler = appDataSendingTimerHandler;

#if defined(APP_ROUTER) || defined(APP_ENDDEVICE)
  appNetworkStatus = false;
  appNetworkStatusTimer.interval = 500;
  appNetworkStatusTimer.mode = SYS_TIMER_PERIODIC_MODE;
  appNetworkStatusTimer.handler = appNetworkStatusTimerHandler;
  SYS_TimerStart(&appNetworkStatusTimer);

  appCommandWaitTimer.interval = NWK_ACK_WAIT_TIME;
  appCommandWaitTimer.mode = SYS_TIMER_INTERVAL_MODE;
  appCommandWaitTimer.handler = appCommandWaitTimerHandler;
#else
  HAL_LedOn(APP_LED_NETWORK);
#endif

#ifdef PHY_ENABLE_RANDOM_NUMBER_GENERATOR
  srand(PHY_RandomReq());
#endif

  APP_CommandsInit();
  
  Serial.begin(115200);

  appState = APP_STATE_SEND;
}
void app_task(void)
{
  switch (appState)
  {
    case APP_STATE_INITIAL:
    {
      appInit();
    } break;

    case APP_STATE_SEND:
    {
      appSendData();
    } break;

    case APP_STATE_SENDING_DONE:
    {
#if defined(APP_ENDDEVICE)
      appState = APP_STATE_PREPARE_TO_SLEEP;
#else
      SYS_TimerStart(&appDataSendingTimer);
      appState = APP_STATE_WAIT_SEND_TIMER;
#endif
    } break;

    case APP_STATE_PREPARE_TO_SLEEP:
    {
      if (!NWK_Busy())
      {
        NWK_SleepReq();
        appState = APP_STATE_SLEEP;
      }
    } break;

    case APP_STATE_SLEEP:
    {
      HAL_LedClose();
      HAL_Sleep(APP_SENDING_INTERVAL);
      appState = APP_STATE_WAKEUP;
    } break;

    case APP_STATE_WAKEUP:
    {
      NWK_WakeupReq();

      HAL_LedInit();
      HAL_LedOn(APP_LED_NETWORK);

      appState = APP_STATE_SEND;
    } break;

    default:
	  appUartProcess();
      break;
  }
}

#ifdef __cplusplus
}
#endif
