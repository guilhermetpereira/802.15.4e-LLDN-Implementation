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
#include "nwk\NwkFrame.h"
#include <sys\sysTimer.h>
#include <LwMesh.h>

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
  APP_STATE_WAIT_FOR_MESSAGE,
} AppState_t;


/* Variables Declaration */
static AppState_t appstate = APP_STATE_INITIAL;

/* Functions Declaration */
//
bool rx_frame(NWK_DataInd_t *ind)
{
  Serial.write("\n****************************\nFrame Receiveid: ");
    NwkFrameBeaconHeaderLLDN_t *beacon = (NwkFrameBeaconHeaderLLDN_t*)ind->data;
    if(beacon->macFcf == 0xc)
      Serial.print("\nFrame Type = LLDN-Beacon, Security Enabled ");
    Serial.print("\nNetwork State: "); Serial.print(beacon->Flags.txState);
    Serial.print("\nConfiguration Sequence Number: "); Serial.print(beacon->confSeqNumber);
    
  return true;
}

bool maccommand_received(NWK_DataInd_t *ind)
{
  Serial.write("\nFrame Receiveid: ");
  Serial.print(ind->data[1], HEX);
  if(ind->data[2] == LL_CONFIGURATION_REQUEST)
  {
    NWK_ConfigStatus_t *d= (NWK_ConfigStatus_t *)ind->data;
    Serial.print(d->identifier);
  }
}

static void appInit(void)
{
  Serial.begin(115200);
  
  // Set Network Parameters
  
  PHY_SetPromiscuousMode(true);
  PHY_SetChannel(0x1a); 
  PHY_SetRxState(true);

  NWK_SetAddr(APP_ADDR);        // Endereco desse nodo visto pela Rede
  NWK_SetPanId(APP_PANID);      // Endereco do coordenador visto pela rede
 


  NWK_OpenEndpoint(1, rx_frame);
  NWK_OpenEndpoint(2, maccommand_received);
   
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
