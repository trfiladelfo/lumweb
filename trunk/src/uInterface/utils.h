/**
 * \addtogroup System
 * @{
 *
 * \file utils.h
 * \author Anziner, Hahn
 * \brief Contains some useful Functions
 *
 */

#ifndef UTILS_H_
#define UTILS_H_

#include "lwip/ip_addr.h"

/**
 * converts an ip_addr structure to a string and
 * writes it to the debug console
 *
 * @param addr structure with any ipaddress
 */
void printaddr(struct ip_addr addr);

/**
 * converts an ip_addr structure to a string and
 * writes it to the debug console
 *
 * @param addr pointer to an ip_addr structure with any ipaddress
 */
void printip(struct ip_addr *addr);

/**
 * converts a netif structure to a string and writes it to the debug console
 *
 * @param pointer to the netif structure
 */
void printnetif(struct netif *netif);

/**
 * fetches an ip_addr from the configuration file
 *
 * @param config name of the configuration line
 * @return structure with the ipaddress (Attention! if not used free the return value!)
 */
struct ip_addr* getAddresFromConfig(char* config);

/**
 * copies a string into a new Space
 *
 * @param src Source which has to be copied
 * @return new allocated string with <b>src</b> as copy
 */
char* pcStrdup(char *src);

/**
 * Checks if a character is a space-character.
 * (' ', Return, Tabulator, ...)
 *
 * @param c Character that has to be checked
 * @return 0 if it is not a space-character, c if it is
 */
int iIsSpace(char c);

/**
 * Returns a pointer to the first occurrence of <b>search</b> in <b>str</b>,
 * or a null pointer if <b>search</b> is not part of <b>str</b>.
 *
 * @param str String to be scanned
 * @param string containing the sequence of characters to match
 *
 * @return A pointer to the first occurrence in <b>str</b> of any of the entire sequence of characters specified in <b>search</b>, or a null pointer if the sequence is not present in <b>str</b>.
 */
char* pcStrstr(char* str, char* search);


/**
 * Calculates the absolute value of <b>i</b>
 *
 * @param i number for calculation
 *
 * @return Absolute value
 */
int iAbs(int i);

#endif /* UTILS_H_ */
