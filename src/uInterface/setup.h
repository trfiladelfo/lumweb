/**
 * \addtogroup Configuration
 * @{
 *
 * \file setup.h
 * \author Anziner, Hahn
 * \brief global config file for the project
 *
 *
 */

#ifndef SETUP_H_
#define SETUP_H_

#define SYSTICK_INT_PRIORITY    0x80
#define ETHERNET_INT_PRIORITY   0xC0

#define ENABLE_LOG		 	 0 // default 0
#define ENABLE_GRAPHIC		 1 // default 1

#define ENABLE_SNTP			 0 // default 0
#define ENABLE_DNS			 0 // default 0
#define	ENABLE_NET_BIOS 	 0 // default 0

#define DEBUG_MEMORY 		 0 // default 0
#define DEBUG_GRAPHIC 		 0 // default 0
#define DEBUG_HTTPC			 0 // default 0
#define DEBUG_GRAPHIC_EDITOR 0 // default 0
#define DEBUG_SSI 			 0 // default 0
#define DEBUG_SSI_PARAMS	 0 // default 0
#define DEBUG_LOG 			 0 // default 0
#define DEBUG_CGI 			 0 // default 0
#define DEBUG_COM			 0 // default 0

void prvSetupHardware(void);

#endif /* SETUP_H_ */
