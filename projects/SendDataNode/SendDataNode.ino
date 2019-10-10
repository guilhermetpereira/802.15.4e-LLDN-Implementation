#ifdef __cplusplus
extern "C" {
#endif

///*- Includes ---------------------------------------------------------------*/
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "hal.h"
#include "phy.h"
#include "sys.h"
#include "nwk.h"
#include <sys\sysTimer.h>
#include <LwMesh.h>

 void println(char *x) { Serial.println(x); Serial.flush(); }

/* Definitions */
#define APP_ADDR    0x0001
#define APP_PANID   0x0002
#define APP_CHANNEL 0x0F

#define APP_DATA_ENDPOINT 1

/* Types */
typedef enum AppState_t
{
  APP_STATE_INITIAL,
  APP_STATE_SEND_DATA_PREPARE,
  APP_STATE_SEND_DATA,
  APP_STATE_IDLE,
} AppState_t;


/* Variables Declaration */
static AppState_t appstate = APP_STATE_INITIAL;
static NWK_DataReq_t msgReq;
static SYS_Timer_t tmrSendData;
static SYS_Timer_t tmrSeeStatus;
// static AppMessageFrame_t  msgFrame;
static char message = 'A';
static NWK_ConfigRequest_t msgFrame; 
/* Functions Declaration */

static void appDataConf(NWK_DataReq_t *req)
{
  if(NWK_SUCCESS_STATUS == req->status) Serial.write("\nMESSAGE RECEIVED IN OTHER NODE");
  else Serial.write("\nMESSAGE NOT RECEIVED IN OTHER NODE");
}

static void tmrSendDataHandler(SYS_Timer_t *timer)
{
  NWK_DataReq(&msgReq);
  Serial.write("\nMESSAGE SENT");
//  Serial.write(message);
}

static void tmrSeeStatusHandler(SYS_Timer_t *timer)
{
//  NWK_DataReq(&msgReq);
  Serial.write(msgReq.status);
//  Serial.write(message);
}

static void appInit(void)
{
  Serial.begin(115200);
  
//  // Set Network Parameters
  PHY_SetPromiscuousMode(true);
  PHY_SetTdmaMode(true);
  NWK_SetAddr(APP_PANID);        // Endereco desse nodo visto pela Rede
  NWK_SetPanId(APP_PANID);       // Endereco do coordenador visto pela rede 

  PHY_SetChannel(0x1a); 
  PHY_SetRxState(true);
  msgFrame.identifier = LL_CONFIGURATION_REQUEST;
  msgFrame.s_macAddr = 0x02;
  msgFrame.tx_channel = 0x1a;
  msgFrame.assTimeSlot = 0x03;
  msgFrame.macAddr = APP_PANID;
  msgFrame.conf.tsDuration = 0b1111111;
  msgFrame.conf.mgmtFrames = 0b1;
  
  // Set Up Data Message Frame
//  msgReq.dstAddr      = APP_ADDR;
//  msgReq.dstEndpoint  = APP_DATA_ENDPOINT;
//  msgReq.srcEndpoint  = APP_DATA_ENDPOINT;
  msgReq.options      =  NWK_OPT_MAC_COMMAND;//NWK_OPT_LLDN_BEACON | NWK_OPT_LLDN_BEACON_DISCOVERY | NWK_OPT_LLDN_BEACON_SECOND; 
  msgReq.data         = (uint8_t *)&msgFrame;
  msgReq.size         = sizeof(msgFrame);
//   msgReq.confirm      = appDataConf; // function called after ACK CONFIRM


  // Set up Timer
  tmrSendData.interval = 2500;
  tmrSendData.mode = SYS_TIMER_PERIODIC_MODE;
  tmrSendData.handler = tmrSendDataHandler;
  SYS_TimerStart(&tmrSendData);

  tmrSeeStatus.interval = 6000;
  tmrSeeStatus.mode = SYS_TIMER_PERIODIC_MODE;
  tmrSeeStatus.handler = tmrSeeStatusHandler;
//  SYS_TimerStart(&tmrSeeStatus);
}

bool rx_frame(NWK_DataInd_t *ind)
{
  return true;
}

void app_task(void)
{
  switch(appstate)
  {
    case APP_STATE_INITIAL:
    {
      appInit();
      appstate = APP_STATE_IDLE;
      break;
    }
    default:
    break;
  }
      
}




///*- Definitions ------------------------------------------------------------*/
//
//#define APP_FLUSH_TIMER_INTERVAL 20
//#define APP_BUFFER_SIZE NWK_MAX_PAYLOAD_SIZE
//#define APP_LED 0
//
///*- Types ------------------------------------------------------------------*/
//typedef enum AppState_t
//{
//  APP_STATE_INITIAL,
//  APP_STATE_IDLE,
//  APP_STATE_SERIAL,
//} AppState_t;
//
///*- Prototypes -------------------------------------------------------------*/
//static void appSendData(void);
//
///*- Variables --------------------------------------------------------------*/
//static AppState_t appState = APP_STATE_INITIAL;
//static SYS_Timer_t appTimer;
//static NWK_DataReq_t appDataReq;
//static bool appDataReqBusy = false;
//static uint8_t appDataReqBuffer[APP_BUFFER_SIZE];
//static uint8_t appUartBuffer[APP_BUFFER_SIZE];
//static uint8_t appUartBufferPtr = 0;
//
///*- Implementations --------------------------------------------------------*/
//static void appDataConf(NWK_DataReq_t *req)
//{
//  appDataReqBusy = false;
//  (void)req;
//}
//static void appSendData(void)
//{
//  if (appDataReqBusy || 0 == appUartBufferPtr)
//    return;
//
//  memcpy(appDataReqBuffer, appUartBuffer, appUartBufferPtr);
//
//  appDataReq.dstAddr = 1;
//  appDataReq.dstEndpoint = LwMeshDefaultEndPoint;
//  appDataReq.srcEndpoint = 
//#include <stdio.h>LwMeshDefaultEndPoint;
//  appDataReq.options = NWK_OPT_ENABLE_SECURITY;
//  appDataReq.data = appDataReqBuffer;
//  appDataReq.size = appUartBufferPtr;
//  appDataReq.confirm = appDataConf;
//  NWK_DataReq(&appDataReq);
//
//  appUartBufferPtr = 0;
//  appDataReqBusy = true;
//}
//static void appTimerHandler(SYS_Timer_t *timer)
//{
//  appSendData();
//  (void)timer;
//}
//bool rx_frame(NWK_DataInd_t *ind)
//{
//  for (uint8_t i = 0; i < ind->size; i++)
//    Serial.write(ind->data[i]);
//
//  return true;
//}
//void app_task(void)
//{
//  switch (appState)
//  {
//    case APP_STATE_INITIAL:
//    {
//      appTimer.interval = APP_FLUSH_TIMER_INTERVAL;
//      appTimer.mode = SYS_TIMER_INTERVAL_MODE;
//      appTimer.handler = appTimerHandler;
//			
//      Serial.begin(115200);
//
//      HAL_LedInit();
//      HAL_LedOn(APP_LED);
//
//      appState = APP_STATE_IDLE;
//      break;
//    }
//    case APP_STATE_IDLE:
//    {
//      if(Serial.available())
//        appState = APP_STATE_SERIAL;	
//
//      break;
//    }
//    case APP_STATE_SERIAL:
//    {
//      while(Serial.available())
//      {
//        uint8_t byte = Serial.read();
//
//        if (appUartBufferPtr == sizeof(appUartBuffer))
//	  appSendData();
//					
//	if (appUartBufferPtr < sizeof(appUartBuffer))
//	  appUartBuffer[appUartBufferPtr++] = byte;
//      }
//				
//      SYS_TimerStop(&appTimer);
//      SYS_TimerStart(&appTimer);
//			
//      appState = APP_STATE_IDLE;
//
//      break;
//    }
//    default:
//      break;
//  }
//}
//
#ifdef __cplusplus
}
#endif
