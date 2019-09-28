#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <hal.h>
#include <phy.h>
#include <sys\sysTimer.h>

#include "sniffer_phy.h"
#include "sniffer_mac.h"

#define LED_POWER_ON		0
#define LED_BLINK		2

static PHY_DataInd_t dataind;
static uint8_t frame[WS_FRAME_MAX_SIZE];

static SYS_Timer_t tmrBlinkData;				// Feedback
static bool sniffer_state = false;

static void tmrBlinkDataHandler(SYS_Timer_t* timer)
{
  HAL_LedOff(LED_BLINK);

  (void)timer;
}
void mac_init(void)
{
  HAL_Init();
  HAL_LedInit();
  HAL_LedOn(LED_POWER_ON);

  tmrBlinkData.interval = 50;
  tmrBlinkData.mode = SYS_TIMER_INTERVAL_MODE;
  tmrBlinkData.handler = tmrBlinkDataHandler;

  PHY_Init();
  PHY_SetPromiscuousMode(true);
  PHY_SetPage(0);

  SYS_TimerInit();
  phy_init();

  frame[0] = WS_CAPTURE_FRAME;

}
void mac_task(void)
{
          
  PHY_TaskHandler();
  SYS_TimerTaskHandler();
  phy_recv_task();
  if(sniffer_state && dataind.size > 0)
  {
    mac_send_data(frame, WS_FRAME_HEADER_SIZE + dataind.size);
    dataind.size = 0;
  }
}
void PHY_DataInd(PHY_DataInd_t *ind)
{
  HAL_LedOn(LED_BLINK);
  SYS_TimerStart(&tmrBlinkData);

  ind->size += WS_FRAME_FCS_SIZE;
  if(ind->size > WS_FRAME_MAX_SIZE)
    ind->size = (WS_FRAME_MAX_SIZE - WS_FRAME_HEADER_SIZE);

  memcpy(frame + WS_FRAME_HEADER_SIZE, ind->data, ind->size);
  dataind = *ind;
}
void mac_recv_process(uint8_t* buffer, int16_t length)
{
  switch(buffer[0])
  {
    case WS_GET_VERSION:
    {
      buffer[1] = '1';
      buffer[2] = '.';
      buffer[3] = '0';
      buffer[4] = '0';
      mac_send_data(buffer, 5);
      return;
    }
    case WS_GET_ISM_BAND:
    {
      buffer[1] = 0;
      mac_send_data(buffer, 2);
      return;
    }
    case WS_SET_PAGE:
    {
      PHY_SetPage(buffer[1]);

      buffer[1] = WS_STATUS_OK;
      mac_send_data(buffer, 2);
      break;
    }
    case WS_SET_CHANNEL:
    {
      PHY_SetChannel(buffer[1]);

      buffer[1] = WS_STATUS_OK;
      mac_send_data(buffer, 2);
      break;
    }
    case WS_CAPTURE_START:
    {
      dataind.size = 0;
      sniffer_state = true;
      PHY_SetRxState(sniffer_state);

      buffer[1] = WS_STATUS_OK;
      mac_send_data(buffer, 2);
      break;
    }
    case WS_CAPTURE_STOP:
    {
      dataind.size = 0;
      sniffer_state = false;
      PHY_SetRxState(false);

      buffer[1]	 = WS_STATUS_OK;
      mac_send_data(buffer, 2);
      break;
    }
    case WS_CAPTURE_RESTART:
    {
      dataind.size = 0;
      sniffer_state = true;
      PHY_SetRxState(false);
      PHY_SetRxState(true);

      buffer[1] = WS_STATUS_OK;
      mac_send_data(buffer, 2);
      break;
    }
    default:
    {
      buffer[1] = WS_STATUS_ERROR;
      mac_send_data(buffer, 2);
      break;
    }
  }
}

#ifdef __cplusplus
}
#endif

