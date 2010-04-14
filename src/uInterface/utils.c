/**
 * \addtogroup System
 * @{
 *
 * \file utils.c
 * \author Anziner, Hahn
 * \brief Contains some useful Functions
 *
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "FreeRTOS.h"
#include "lwip/ip_addr.h"
#include "lwip/netif.h"

#include "ethernet/LWIPStack.h"
#include "configuration/configloader.h"
#include "graphic/gui/displayBasics.h"

#include "utils.h"

/**
 * converts an ip_addr structure to a string and
 * writes it to the debug console
 *
 * @param addr structure with any ipaddress
 */
void printaddr(struct ip_addr addr)
{
	printf("%d.%d.%d.%d", (unsigned int) ((addr.addr) & 0xFF),
			(unsigned int) ((addr.addr >> 8) & 0xFF),
			(unsigned int) ((addr.addr >> 16) & 0xFF),
			(unsigned int) ((addr.addr >> 24) & 0xFF));
}

/**
 * converts an ip_addr structure to a string and
 * writes it to the debug console
 *
 * @param addr pointer to an ip_addr structure with any ipaddress
 */
void printip(struct ip_addr *addr)
{
	printf("%d.%d.%d.%d", (unsigned int) ((addr->addr) & 0xFF),
			(unsigned int) ((addr->addr >> 8) & 0xFF),
			(unsigned int) ((addr->addr >> 16) & 0xFF),
			(unsigned int) ((addr->addr >> 24) & 0xFF));
}

/**
 * converts a netif structure to a string and writes it to the debug console
 *
 * @param pointer to the netif structure
 */
void printnetif(struct netif *netif)
{
	printf("Adresskonfiguration:\n\tIP Adresse: ");
	printaddr(netif->ip_addr);
	printf("\n\tSubnet    : ");
	printaddr(netif->netmask);
	printf("\n\tGateway   : ");
	printaddr(netif->gw);
	printf("\n");
}

/**
 * fetches an ip_addr from the configuration file
 *
 * @param config name of the configuration line
 * @return structure with the ipaddress (Attention! if not used free the return value!)
 */
struct ip_addr* getAddresFromConfig(char* config)
{
	char buffer[4][4];
	int i, j, pos = 0;
	struct ip_addr *retAddr = NULL;

	//
	// open the config file
	//
	char* configLoad = loadFromConfig(IP_CONFIG_FILE, config);

	//
	// Check if first character is not a number
	//
	if (configLoad[0] < '0' && configLoad[0] > '9')
	{
		//
		// Return with NULL, because it's not a valid IP
		//
		vShowBootText("IP is not Valid!");
		retAddr = NULL;
	}
	else
	{
		//
		// Iterate the Configuration String and parse the IP
		//
		for (i = 0; i < 4; i++)
		{
			for (j = 0; j < 4; j++)
			{
				buffer[i][j] = configLoad[pos];
				pos++;
				if (configLoad[pos] == '.')
				{
					//
					// If a '.' occurs, jump to the next block
					//
					j++;
					pos++;
					buffer[i][j] = 0;
					break;
				}
			}
			//
			// Terminate the string with a 0
			//
			buffer[i][j] = 0;
		}

		//
		// Allocate new Space for the ipaddress structure
		//
		retAddr = (struct ip_addr*) pvPortMalloc(sizeof(struct ip_addr));
		retAddr->addr = 0;

		//
		// convert the string to numbers and shift them into a long value
		//
		for (i = 0; i < 4; i++)
		{
			retAddr->addr = retAddr->addr | ((atoi(buffer[i]) & 0xFF)
					<< (i * 8));
		}
	}

	//
	// free configuration file
	//
	vPortFree(configLoad);

	//
	// return the new Address
	//
	return retAddr;
}

/**
 * copies a string into a new Space
 *
 * @param src Source which has to be copied
 * @return new allocated string with <b>src</b> as copy
 */
char* pcStrdup(char *src)
{
	//
	// get the length of the source string
	//
	int i, len = strlen(src);

	//
	// allocate string for the destination
	//
	char* ret = (char*) pvPortMalloc(len + 1);

	//
	// check if new space has been allocated
	//
	if (ret != NULL)
	{
		//
		// copy every character from the source to the destination
		//
		for (i = 0; i < len && src[i] != 0; i++)
		{
			ret[i] = src[i];
		}

		//
		// terminate the string with 0
		//
		ret[i] = 0;
	}

	//
	// return new String
	//
	return ret;
}

/**
 * Checks if a character is a space-character.
 * (' ', Return, Tabulator, ...)
 *
 * @param c Character that has to be checked
 * @return 0 if it is not a space-character, c if it is
 */
int iIsSpace(char c)
{

	//
	// Check if it is not a character
	//
	if (c != 0x20 && c != 0x09 && c != 0x0a && c != 0x0b && c != 0x0c && c
			!= 0x0d)
	{
		//
		// return 0 if it is no space
		//
		return 0;
	}

	return c;
}

/**
 * Returns a pointer to the first occurrence of <b>search</b> in <b>str</b>,
 * or a null pointer if <b>search</b> is not part of <b>str</b>.
 *
 * @param str String to be scanned
 * @param string containing the sequence of characters to match
 *
 * @return A pointer to the first occurrence in <b>str</b> of any of the entire sequence of characters specified in <b>search</b>, or a null pointer if the sequence is not present in <b>str</b>.
 */
char* pcStrstr(char* str, char* search)
{

	int i, len = strlen(str), searchlen = strlen(search);
	for (i = 0; i < len; i++)
	{
		if (strncmp(str + i, search, searchlen) == 0)
		{
			return str + i;
		}
	}
	return NULL;
}


/**
 * Calculates the absolute value of <b>i</b>
 *
 * @param i number for calculation
 *
 * @return Absolute value
 */
int iAbs(int i)
{
	if (i < 0)
	{
		return i * -1;
	}
	return i;
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
