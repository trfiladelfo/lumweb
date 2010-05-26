#include "hw_memmap.h"
#include "hw_ints.h"
#include "hw_nvic.h"
#include "hw_types.h"
#include "gpio.h"
#include "sysctl.h"
#include "interrupt.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "systick.h"
#include "rit128x96x4.h"
#include "uartstdio.h"

int findChar(char *s, char c);

int main (void) { 
	
	char read_buffer[32];
	char kurve_buf[64];
	char d1_buf[64];

	char* str;
	char* id;

	char* value;
	char *pch;

	int i_value;
	int pos_value;
	int kurve=12;
	int d1 = 1;

  	// Set the clocking to run directly from the crystal.
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |	SYSCTL_XTAL_8MHZ);
	

    UARTStdioInit(0);
 
  	// Initialize the OLED display.
    RIT128x96x4Init(1000000);

	//Display the constants
	RIT128x96x4StringDraw("Maschinensimulation!", 0, 0, 15);
	RIT128x96x4StringDraw("by Anzinger und Hahn", 0, 80, 15);

	UARTprintf("Hi UART\n;");



	while(1){//wait until output is necassary
		UARTgets(&read_buffer, 32);
		id = &read_buffer;
		str = id+3;
		id=str;
		if(read_buffer[0] == '!' && read_buffer[1] == 'g'){

			RIT128x96x4StringDraw("Wert get", 0, 50, 15);

			if(strcmp(id, "kurve") == 0)
				UARTprintf("%d", kurve);
			if(strcmp(id, "d1") == 0)
				UARTprintf("%d", d1);
		}

		if(read_buffer[0] == '!' && read_buffer[1] == 's'){
			RIT128x96x4StringDraw("Wert set", 0, 50, 15);
			pos_value = findChar(str, '=');
			value = str + pos_value + 1;

			id[pos_value] = 0;

			i_value = atoi(value);

			if(strcmp(id, "kurve") == 0)
				kurve = i_value;
			if(strcmp(id, "d1") == 0)
				d1 = i_value;

			UARTprintf("%s=%d\n", id, i_value);
		}




		sprintf(&kurve_buf, "Wert Kurve: %d", kurve);
		sprintf(&d1_buf, "Wert d1: %d", d1);

		RIT128x96x4StringDraw(kurve_buf, 0, 60, 15);
		RIT128x96x4StringDraw(d1_buf, 0, 70, 15);

      }
}

int findChar(char *s, char c){
	int len, i;

	len = strlen(s);

	for(i=0;i<len;i++){
		if(s[i] == c)
			return i;
	}

	return -1;
}
