#ifndef _LWMESH_H_
#define _LWMESH_H_

#include <stdint.h>
#include <nwk\nwkRx.h>

#define LwMeshDefaultEndPoint 1

class LwMesh
{
	public:
		void begin(bool (*handler)(NWK_DataInd_t *ind), uint16_t netaddr, uint16_t panid = 0xCAFE, uint8_t channel = 0x0F);
		void task(void);

	public:

};

#endif /* _LWMESH_H_ */