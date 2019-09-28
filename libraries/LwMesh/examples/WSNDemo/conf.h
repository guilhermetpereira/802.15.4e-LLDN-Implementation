#ifndef _CONF_H_
#define _CONF_H_

/*- Definitions ------------------------------------------------------------*/
#define APP_ADDR                0x0000
#define APP_PANID               0xCAFE
#define APP_CHANNEL		0x0F

#if APP_ADDR == 0
  #define APP_CAPTION           "Coordinator"
  #define APP_COORDINATOR
  #define APP_SENDING_INTERVAL  1000
#elif APP_ADDR < 0x8000
  #define APP_CAPTION           "Router"
  #define APP_ROUTER
  #define APP_SENDING_INTERVAL  1000
#else
  #define APP_CAPTION           "End Device"
  #define APP_ENDDEVICE
  #define APP_SENDING_INTERVAL  10000
#endif

#endif // _CONF_H_
