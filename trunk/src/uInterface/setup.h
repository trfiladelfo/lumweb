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

/// Enable logging in /log/sys.log on the SD Card
#define ENABLE_LOG		 	 0 // default 0

/// Enable the graphic stuff
#define ENABLE_GRAPHIC		 1 // default 1

/// enable sntp client
#define ENABLE_SNTP			 0 // default 0

/// enable dns client
#define ENABLE_DNS			 0 // default 0

/// enable net bios client
#define	ENABLE_NET_BIOS 	 0 // default 0


/// enable debugging messages for memory ususage
#define DEBUG_MEMORY 		 0 // default 0

/// enable debugging messages for graphic stuff
#define DEBUG_GRAPHIC 		 1 // default 0

/// enable debugging messages for the webclient
#define DEBUG_HTTPC			 1 // default 0

/// enable debugging messages for the graphic editor
#define DEBUG_GRAPHIC_EDITOR 0 // default 0

/// enable debugging messages for ssi handling
#define DEBUG_SSI 			 0 // default 0

/// enable debugging messages for ssi parameters
#define DEBUG_SSI_PARAMS	 0 // default 0

/// enable debugging messages for the logging framework
#define DEBUG_LOG 			 0 // default 0

/// enable debugging messages for tags
#define DEBUG_TAGS			 1 // default 0

/// enable debugging messages for cgi routines
#define DEBUG_CGI 			 0 // default 0

/// enable debugging messages for comTask
#define DEBUG_COM			0 // default 0



/// initialize hardware
void prvSetupHardware(void);

#endif /* SETUP_H_ */
