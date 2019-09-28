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

/* Definitions */
#define APP_ADDR    0x8001
#define APP_PANID   0x0001
#define APP_CHANNEL 0x0F
#define APP_DATA_ENDPOINT  1

/* Types */
typedef enum AppState_t
{
  APP_STATE_INITIAL,
} AppState_t;


/* Variables Declaration */
static AppState_t appstate = APP_STATE_INITIAL;

/* Functions Declaration */
static void appInit(void)
{
  // Set Network Parameters
  NWK_SetAddr(APP_ADDR);        // node network addres 
  NWK_SetPanId(APP_PANID);      // pan network address
  PHY_SetChannel(APP_CHANNEL); 
  PHY_SetRxState(true);
  
}

void app_task(void)
{
  switch(appstate)
  {
    case APP_STATE_INITIAL:
    {
      appInit();
    }
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
