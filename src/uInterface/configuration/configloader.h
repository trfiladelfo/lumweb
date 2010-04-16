/**
 * \addtogroup Config
 * @{
 *
 * \author Anziner, Hahn
 * \brief Prototypes for conf file handling
 *
 */

#ifndef CONFIGLOADER_H_
#define CONFIGLOADER_H_

#define IP_CONFIG_FILE "/conf/ipconfig.cnf"

char* loadFromConfig(char* file, char* param);

#endif /* CONFIGLOADER_H_ */

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************

