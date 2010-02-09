/*
 * syscall.c
 *
 *  Created on: 09.02.2010
 *      Author: root
 */

#include <stdlib.h>
#include <stdio.h>
//#include <errno.h>
//#include <time.h>
//#include <sys/stat.h>


//sbrk
//Increase program data space. As malloc and related functions depend on this,
//it is useful to have a working implementation.
//The following suffices for a standalone system; it exploits the symbols created in the linker script.
//linker (standalone.ld) sets heap start and end
extern unsigned int _HEAP_START;
extern unsigned int _HEAP_END;
static caddr_t heap = NULL;
caddr_t _sbrk(int increment) {
	caddr_t prevHeap;
	caddr_t nextHeap;
	if (heap == NULL) {
		// first allocation
		heap = (caddr_t) & _HEAP_START;
	}
	prevHeap = heap;
	// Always return data aligned on a 8 byte boundary
	nextHeap = (caddr_t)(((unsigned int) (heap + increment) + 7) & ~7);
	// Check enough space and there is no collision with stack coming the other way
	// if stack is above start of heap
	if (nextHeap >= (caddr_t) & _HEAP_END) {
		//errno = ENOMEM;
		return NULL; // error - no more memory
	} else {
		heap = nextHeap;
		return (caddr_t) prevHeap;
	}

	//return pvPortMalloc(increment);
}

/*
#undef errno
extern int errno;
//exit
//Minimal implementation:
void _exit(int i) {
	printf("Program exit");
	while (1)
		;
}
//open
//Open a file. Minimal implementation:
int _open(const char *name, int flags, int mode) {
	return -1;
}
//read
//Read from a file. Minimal implementation:
int _read(int file, char *ptr, int len) {
	return 0;
}
//write
//Write a character to a file. `libc' subroutines will use this system routine for output to all files,
//including stdout---so if you need to generate any output, for example to a serial port for debugging,
//you should make your minimal write capable of doing this.
//putChar must be implemented by the user
extern int sendchar (int pcChar);
int _write(int file, char *ptr, int len) {
	for (; len > 0; len--) {
		sendchar(*ptr++);
	}
	return len;
}
//lseek
//Set position in a file. Minimal implementation:
int _lseek(int file, int ptr, int dir) {
	return 0;
}
//fstat
//Status of an open file.
//For consistency with other minimal implementations in these examples,
//all files are regarded as character special devices.
//The `sys/stat.h' header file required is distributed in the `include' subdirectory for this C library.
int _fstat(int file, struct stat *st) {
	st->st_mode = S_IFCHR;
	return 0;
}

//link
//Establish a new name for an existing file. Minimal implementation:
int _link(char *old, char *new) {
	errno = EMLINK;
	return -1;
}
//unlink
//Remove a file's directory entry. Minimal implementation:
int _unlink(char *name) {
	errno = ENOENT;
	return -1;
}
//stat
//Status of a file (by name). Minimal implementation:
int _stat(const char *file, struct stat *st) {
	st->st_mode = S_IFCHR;
	return 0;
}
//close
//Close a file. Minimal implementation:
int _close(int file) {
	return -1;
}
//environ
//A pointer to a list of environment variables and their values.
//For a minimal environment, this empty list is adequate:
char *__env = { 0 };
char **environ = &__env;
//execve
//Transfer control to a new process. Minimal implementation (for a system without processes):
int _execve(char *name, char **argv, char **env) {
	errno = ENOMEM;
	return -1;
}
//fork
//Create a new process. Minimal implementation (for a system without processes):
int _fork() {
	errno = EAGAIN;
	return -1;
}
//getpid
//Process-ID; this is sometimes used to generate strings unlikely to conflict with other processes.
//Minimal implementation, for a system without processes:
int _getpid() {
	return 1;
}
//isatty
//Query whether output stream is a terminal.
//For consistency with the other minimal implementations,

//which only support output to stdout, this minimal implementation is suggested:
int _isatty(int file) {
	return 1;
}
//kill
//Send a signal. Minimal implementation:
int _kill(int pid, int sig) {
	errno = EINVAL;
	return (-1);
}

//times
//Timing information for current process. Minimal implementation:
int times(struct tm *buf) {
	return -1;
}
//wait
//Wait for a child process. Minimal implementation:
int _wait(int *status) {
	errno = ECHILD;

	return -1;
}*/
