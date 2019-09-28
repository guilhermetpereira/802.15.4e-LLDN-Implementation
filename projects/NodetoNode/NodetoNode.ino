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

#define APP_DATA_ENDPOINT 1

/* Types */
typedef enum AppState_t
{
  APP_STATE_INITIAL,
  APP_STATE_SEND_DATA_PREPARE,
  APP_STATE_SEND_DATA,
  APP_STATE_IDLE,
  APP_STATE_WAIT_FOR_MESSAGE,
} AppState_t;


/* Variables Declaration */
static AppState_t appstate = APP_STATE_INITIAL;
//static SYS_Timer_t tmrOpenEndPoint; ( Usado Para Debugar )

/* Functions Declaration */
//
bool rx_frame(NWK_DataInd_t *ind)
{
  if(beacon->state == '') app_state = 
  if(
  Serial.write("\nMENSAGEM = ");
  char rec_data = (char)*(ind->data);
  Serial.println(rec_data);
  return true;
}
//static void tmrOpenEndPointHandler(SYS_Timer_t *timer)
//{
//  NWK_OpenEndpoint(1, rx_frame);
//  Serial.write("END POINT ABERTO");
//}
static void appInit(void)
{
  Serial.begin(115200);
  
  // Set Network Parameters
  NWK_SetAddr(APP_ADDR);        // Endereco desse nodo visto pela Rede
  NWK_SetPanId(APP_PANID);      // Endereco do coordenador visto pela rede
  PHY_SetChannel(0x1a); 
  PHY_SetRxState(true);
  
  // Set up Timer ( Usado para Debugar ) 
//  tmrOpenEndPoint.interval = 10000;
//  tmrOpenEndPoint.mode = SYS_TIMER_INTERVAL_MODE;
//  tmrOpenEndPoint.handler = tmrOpenEndPointHandler;
//  SYS_TimerStart(&tmrOpenEndPoint);     

  NWK_OpenEndpoint(1, rx_frame); 
}


void app_task(void)
{
  switch(appstate)
  {
    case APP_STATE_INITIAL:
    {
      appInit();
      appstate = APP_STATE_WAIT_FOR_MESSAGE;
      break;
    }
    default:
    break;
  }
      
}

#ifdef __cplusplus
}
#endif
