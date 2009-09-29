
#ifndef __LWIPLIB_H__
#define __LWIPLIB_H__

#include "lwip/ip_addr.h"

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

//*****************************************************************************
//
// lwIP Options
//
//*****************************************************************************
//#include "../third_party/lwip-1.3.0/src/include/lwip/opt.h"

//*****************************************************************************
//
// Ensure that AUTOIP COOP option is configured correctly.
//
//*****************************************************************************
#undef LWIP_DHCP_AUTOIP_COOP
#define LWIP_DHCP_AUTOIP_COOP   ((LWIP_DHCP) && (LWIP_AUTOIP))

//*****************************************************************************
//
// lwIP API Header Files
//
//*****************************************************************************
//#include "../third_party/lwip-1.3.0/src/include/lwip/api.h"
//#include "../third_party/lwip-1.3.0/src/include/lwip/netifapi.h"
//#include "../third_party/lwip-1.3.0/src/include/lwip/tcp.h"
//#include "../third_party/lwip-1.3.0/src/include/lwip/udp.h"
//#include "../third_party/lwip-1.3.0/src/include/lwip/tcpip.h"
//#include "../third_party/lwip-1.3.0/src/include/lwip/sockets.h"
//#include "../third_party/lwip-1.3.0/src/include/lwip/mem.h"

//*****************************************************************************
//
// IP Address Acquisition Modes
//
//*****************************************************************************
#define IPADDR_USE_STATIC       0
#define IPADDR_USE_DHCP         1
#define IPADDR_USE_AUTOIP       2

//*****************************************************************************
//
// lwIP Abstraction Layer API
//
//*****************************************************************************

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif // __LWIPLIB_H__
