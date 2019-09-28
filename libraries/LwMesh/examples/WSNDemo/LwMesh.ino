// include the library code:
#include <LwMesh.h>
#include "conf.h"

LwMesh lwMesh;

#ifdef __cplusplus
extern "C" {
#endif

bool rx_frame(NWK_DataInd_t *ind);
void app_task(void);

#ifdef __cplusplus
}
#endif

void setup()
{
  lwMesh.begin(rx_frame, APP_ADDR, APP_PANID, APP_CHANNEL);
}

void loop()
{
  lwMesh.task();
  app_task();
}


