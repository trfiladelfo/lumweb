/**
 * \addtogroup System
 * @{
 *
 * \author Anziner, Hahn
 * \brief Contains a allocation function for standard libraries (e.g. printf)
 *
 */

#include <stdlib.h>
#include <stdio.h>

//sbrk
//Increase program data space. As malloc and related functions depend on this,
//it is useful to have a working implementation.
//The following suffices for a standalone system; it exploits the symbols created in the linker script.
//linker (standalone.ld) sets heap start and end
extern unsigned int _HEAP_START;
extern unsigned int _HEAP_END;
static caddr_t heap = NULL;

caddr_t _sbrk(int increment)
{
	caddr_t prevHeap;
	caddr_t nextHeap;
	if (heap == NULL)
	{
		//
		// first allocation
		//
		heap = (caddr_t) & _HEAP_START;
	}
	prevHeap = heap;
	//
	// Always return data aligned on a 8 byte boundary
	//
	nextHeap = (caddr_t)(((unsigned int) (heap + increment) + 7) & ~7);
	//
	// Check enough space and there is no collision with stack coming the other way
	// if stack is above start of heap
	//
	if (nextHeap >= (caddr_t) & _HEAP_END)
	{
		printf("no more Memory!!\n");
		return NULL; // error - no more memory
	}
	else
	{	//
		// return next free Heap
		//
		heap = nextHeap;
		return (caddr_t) prevHeap;
	}
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************

