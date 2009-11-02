#/*
#	FreeRTOS.org V5.3.1 - Copyright (C) 2003-2009 Richard Barry.
#
#	This file is part of the FreeRTOS.org distribution.
#
#	FreeRTOS.org is free software; you can redistribute it and/or modify it
#	under the terms of the GNU General Public License (version 2) as published
#	by the Free Software Foundation and modified by the FreeRTOS exception.
#	**NOTE** The exception to the GPL is included to allow you to distribute a
#	combined work that includes FreeRTOS.org without being obliged to provide
#	the source code for any proprietary components.  Alternative commercial
#	license and support terms are also available upon request.  See the 
#	licensing section of http://www.FreeRTOS.org for full details.
#
#	FreeRTOS.org is distributed in the hope that it will be useful,	but WITHOUT
#	ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
#	FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
#	more details.
#
#	You should have received a copy of the GNU General Public License along
#	with FreeRTOS.org; if not, write to the Free Software Foundation, Inc., 59
#	Temple Place, Suite 330, Boston, MA  02111-1307  USA.
#
#
#	***************************************************************************
#	*                                                                         *
#	* Get the FreeRTOS eBook!  See http://www.FreeRTOS.org/Documentation      *
#	*                                                                         *
#	* This is a concise, step by step, 'hands on' guide that describes both   *
#	* general multitasking concepts and FreeRTOS specifics. It presents and   *
#	* explains numerous examples that are written using the FreeRTOS API.     *
#	* Full source code for all the examples is provided in an accompanying    *
#	* .zip file.                                                              *
#	*                                                                         *
#	***************************************************************************
#
#	1 tab == 4 spaces!
#
#	Please ensure to read the configuration and relevant port sections of the
#	online documentation.
#
#	http://www.FreeRTOS.org - Documentation, latest information, license and
#	contact details.
#
#	http://www.SafeRTOS.com - A version that is certified for use in safety
#	critical systems.
#
#	http://www.OpenRTOS.com - Commercial support, development, porting,
#	licensing and training services.
#*/


#/************************************************************************* 
# * Please ensure to read http://www.freertos.org/portLM3Sxxxx_Eclipse.html
# * which provides information on configuring and running this demo for the
# * various Luminary Micro EKs.
# *************************************************************************/

RTOS_SOURCE_DIR=../Source
DEMO_COMMON_DIR=../Common/Minimal
DEMO_INCLUDE_DIR=../Common/include
UIP_COMMON_DIR=../Common/ethernet/uIP/uip-1.0/uip
LUMINARY_DRIVER_DIR=../Common/drivers/LuminaryMicro
FATFS_COMMON_DIR=../Common/fatfs

NAME = Luminary_Webinterface

CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
LDSCRIPT=standalone.ld

# should use --gc-sections but the debugger does not seem to be able to cope with the option.
LINKER_FLAGS=-nostartfiles -Xlinker -o$(NAME).axf -Xlinker -M -Xlinker -Map=rtosdemo.map -Xlinker --no-gc-sections

DEBUG=-g
OPTIM=-O0


CFLAGS=$(DEBUG) -I . \
		-I $(RTOS_SOURCE_DIR)/include \
		-I $(RTOS_SOURCE_DIR)/portable/GCC/ARM_CM3 \
		-I $(DEMO_INCLUDE_DIR) -D GCC_ARMCM3_LM3S102 \
		-I $(UIP_COMMON_DIR) \
		-I $(FATFS_COMMON_DIR)/src \
		-I $(FATFS_COMMON_DIR)/port \
		-I ./webserver \
		-I $(LUMINARY_DRIVER_DIR) \
		-D inline= -mthumb -mcpu=cortex-m3 \
		$(OPTIM) -T$(LDSCRIPT) \
		-D PACK_STRUCT_END=__attribute\(\(packed\)\) \
		-D ALIGN_STRUCT_END=__attribute\(\(aligned\(4\)\)\) \
		-D sprintf=usprintf -D snprintf=usnprintf -D printf=uipprintf \
		-ffunction-sections -fdata-sections 

SOURCE=	main.c \
		timertest.c \
		./ComTask/comTask.c \
		$(LUMINARY_DRIVER_DIR)/ustdlib.c \
		./webserver/uIP_Task.c \
		./webserver/emac.c \
		./webserver/httpd.c \
		./webserver/httpd-cgi.c \
		./webserver/httpd-fs.c \
		./webserver/http-strings.c \
		./GraphicsLibrary/runGraphics.c \
		./GraphicsLibrary/renderGraphics.c \
		./GraphicsLibrary/graphicButton.c \
		./GraphicsLibrary/graphicTextBox.c \
		./GraphicsLibrary/graphicObjects.c \
		./GraphicsLibrary/port/rit128x96x4.c \
		./DebugUART/debugUART.c \
		$(UIP_COMMON_DIR)/uip_arp.c \
		$(UIP_COMMON_DIR)/psock.c \
		$(UIP_COMMON_DIR)/timer.c \
		$(UIP_COMMON_DIR)/uip.c \
		$(RTOS_SOURCE_DIR)/list.c \
		$(RTOS_SOURCE_DIR)/queue.c \
		$(RTOS_SOURCE_DIR)/tasks.c \
		$(RTOS_SOURCE_DIR)/portable/GCC/ARM_CM3/port.c \
		$(RTOS_SOURCE_DIR)/portable/MemMang/heap_2.c \
		$(FATFS_COMMON_DIR)/port/fs.c \
		$(FATFS_COMMON_DIR)/src/ff.c \
		$(FATFS_COMMON_DIR)/port/mmc-lmi-rtos.c \
		
		
#		./IntQueueTimer.c \
#		$(FATFS_COMMON_DIR)/port/fs.c \
#		./ParTest/ParTest.c \

LIBS= $(LUMINARY_DRIVER_DIR)/arm-none-eabi-gcc/libdriver.a $(LUMINARY_DRIVER_DIR)/arm-none-eabi-gcc/libgr.a

OBJS = $(SOURCE:.c=.o)

all: $(NAME).bin

$(NAME).bin : $(NAME).axf
	$(OBJCOPY) $(NAME).axf -O binary $(NAME).bin

$(NAME).axf : $(OBJS) startup.o Makefile
	$(CC) $(CFLAGS) $(OBJS) startup.o $(LIBS) $(LINKER_FLAGS)

$(OBJS) : %.o : %.c Makefile FreeRTOSConfig.h
	$(CC) -c $(CFLAGS) $< -o $@

startup.o : startup.c Makefile
	$(CC) -c $(CFLAGS) -O1 startup.c -o startup.o

clean :
	touch Makefile
	rm  -f $(OBJS)
	rm  -f *.o
	rm  -f *.map
	rm  -f *.bin
	rm  -f *.axf
	rm  -f *.elf

flash : all
	openocd -f ../lm3s_scripts/lm3s_flash_start.cfg -c "flash write_bank 0 ./$(NAME).bin 0" -f ../lm3s_scripts/lm3s_flash_end.cfg 

doc : 
	doxygen ../../doc/code/DoxyFile
	
cleanflash : clean flash
	
