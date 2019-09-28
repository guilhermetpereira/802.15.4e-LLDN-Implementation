#ifndef _PHY_TRAMPOLINE_H_
#define _PHY_TRAMPOLINE_H_

#ifdef __cplusplus
extern "C" {
#endif

/*- Includes ---------------------------------------------------------------*/
#if defined(HAL_ATMEGARFA1)
	#include "phy/atmegarfa1/phy.h"
#elif defined(HAL_ATMEGARFR2)
	#include "phy/atmegarfr2/phy.h"
#else
	#error "Undefined Hardware Abstraction Layer (HAL)"
#endif

/*- Prototypes -------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif // _PHY_TRAMPOLINE_H_