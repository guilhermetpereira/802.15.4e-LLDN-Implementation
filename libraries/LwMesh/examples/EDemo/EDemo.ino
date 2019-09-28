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
#include <sys\sysTimer.h>
#include <LwMesh.h>

#define APP_FIRST_CHANNEL                   11
#define APP_LAST_CHANNEL                    26
#define APP_SCAN_INTERVAL                   500
#define APP_LED                             0

/*- Types ------------------------------------------------------------------*/
typedef enum AppState_t
{
  APP_STATE_INITIAL,
  APP_STATE_MEASURE_ED,
  APP_STATE_WAIT_SCAN_TIMER,
} AppState_t;

/*- Variables --------------------------------------------------------------*/
static AppState_t appState = APP_STATE_INITIAL;
static uint8_t appEdValue[APP_LAST_CHANNEL - APP_FIRST_CHANNEL + 1];
static SYS_Timer_t appScanTimer;

/*- Implementations --------------------------------------------------------*/
static void appPrintEdValues(void)
{
  char hex[] = "0123456789abcdef";

  for (uint8_t i = 0; i < sizeof(appEdValue); i++)
  {
    uint8_t v = appEdValue[i] - PHY_RSSI_BASE_VAL;
	Serial.write(hex[(v >> 4) & 0x0f]);
	Serial.write(hex[v & 0x0f]);
	Serial.write(' ');
  }

  Serial.write('\n');
  Serial.write('\n');
}
static void appScanTimerHandler(SYS_Timer_t *timer)
{
  appState = APP_STATE_MEASURE_ED;
  (void)timer;
}
static void appInit(void)
{
  appScanTimer.interval = APP_SCAN_INTERVAL;
  appScanTimer.mode = SYS_TIMER_PERIODIC_MODE;
  appScanTimer.handler = appScanTimerHandler;
  SYS_TimerStart(&appScanTimer);
  
  Serial.begin(115200);

  HAL_LedInit();
  HAL_LedOn(APP_LED);

  appState = APP_STATE_MEASURE_ED;
}
void app_task(void)
{
  switch (appState)
  {
    case APP_STATE_INITIAL:
    {
      appInit();
    } break;

    case APP_STATE_MEASURE_ED:
    {
      for (uint8_t i = 0; i < sizeof(appEdValue); i++)
      {
        PHY_SetChannel(APP_FIRST_CHANNEL + i);
        appEdValue[i] = PHY_EdReq();
      }

      appPrintEdValues();

      appState = APP_STATE_WAIT_SCAN_TIMER;
    } break;

    default:
      break;
  }
}

#ifdef __cplusplus
}
#endif
