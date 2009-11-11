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

LUMINARY_DRIVER_DIR=../common/drivers/LuminaryMicro
RTOS_SOURCE_DIR=../common/rtos/Source
LWIP_COMMON_DIR=../common/ethernet/LMlwip
FATFS_COMMON_DIR=../common/fatfs

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
		-D GCC_ARMCM3_LM3S102 \
		-I $(LWIP_COMMON_DIR)/src/include \
		-I $(LWIP_COMMON_DIR)/src/include/ipv4 \
		-I $(LWIP_COMMON_DIR)/contrib/port/FreeRTOS/LM3S \
		-I $(FATFS_COMMON_DIR) \
		-I $(LUMINARY_DRIVER_DIR) \
		-D inline= -mthumb -mcpu=cortex-m3 \
		$(OPTIM) -T$(LDSCRIPT) \
		-D PACK_STRUCT_END=__attribute\(\(packed\)\) \
		-D ALIGN_STRUCT_END=__attribute\(\(aligned\(4\)\)\) \
		-D sprintf=usprintf -D snprintf=usnprintf -D printf=uipprintf \
		-D _sprintf=usprintf -D _snprintf=usnprintf \
		-D malloc=pvPortMalloc \
		-ffunction-sections -fdata-sections 

SOURCE=	main.c \
		timertest.c \
		$(LUMINARY_DRIVER_DIR)/ustdlib.c \
		$(RTOS_SOURCE_DIR)/list.c \
		$(RTOS_SOURCE_DIR)/queue.c \
		$(RTOS_SOURCE_DIR)/tasks.c \
		$(RTOS_SOURCE_DIR)/portable/GCC/ARM_CM3/port.c \
		$(RTOS_SOURCE_DIR)/portable/MemMang/heap_2.c \
		$(LWIP_COMMON_DIR)/src/core/sntp.c \
		$(LWIP_COMMON_DIR)/src/core/dhcp.c \
		$(LWIP_COMMON_DIR)/src/core/dns.c \
		$(LWIP_COMMON_DIR)/contrib/port/FreeRTOS/sys_arch.c \
		$(LWIP_COMMON_DIR)/src/core/stats.c \
		$(LWIP_COMMON_DIR)/src/core/netif.c \
		$(LWIP_COMMON_DIR)/src/core/pbuf.c \
		$(LWIP_COMMON_DIR)/src/core/ipv4/ip_frag.c \
		$(LWIP_COMMON_DIR)/src/core/ipv4/autoip.c \
		$(LWIP_COMMON_DIR)/src/core/ipv4/icmp.c \
		$(LWIP_COMMON_DIR)/src/core/ipv4/igmp.c \
		$(LWIP_COMMON_DIR)/src/core/ipv4/inet.c \
		$(LWIP_COMMON_DIR)/src/core/ipv4/inet_chksum.c \
		$(LWIP_COMMON_DIR)/src/core/ipv4/ip.c \
		$(LWIP_COMMON_DIR)/src/core/ipv4/ip_addr.c \
		$(LWIP_COMMON_DIR)/src/core/udp.c \
		$(LWIP_COMMON_DIR)/src/core/tcp_out.c \
		$(LWIP_COMMON_DIR)/src/core/tcp_in.c \
		$(LWIP_COMMON_DIR)/src/core/tcp.c \
		$(LWIP_COMMON_DIR)/src/core/init.c \
		$(LWIP_COMMON_DIR)/src/core/mem.c \
		$(LWIP_COMMON_DIR)/src/core/raw.c \
		$(LWIP_COMMON_DIR)/src/core/memp.c \
		$(LWIP_COMMON_DIR)/src/core/sys.c \
		$(LWIP_COMMON_DIR)/src/api/tcpip.c \
		$(LWIP_COMMON_DIR)/src/api/api_msg.c \
		$(LWIP_COMMON_DIR)/src/api/sockets.c \
		$(LWIP_COMMON_DIR)/src/api/netbuf.c \
		$(LWIP_COMMON_DIR)/src/api/api_lib.c \
		$(LWIP_COMMON_DIR)/src/netif/etharp.c \
		$(FATFS_COMMON_DIR)/ff.c \
		$(FATFS_COMMON_DIR)/mmc-ek-lm3s8962.c \
		$(FATFS_COMMON_DIR)/SDcard.c \
		ethernet/ETHIsr.c \
		ethernet/LWIPStack.c \
		httpd/lmi_fs.c \
		httpd/httpd.c \
		httpd/cgifuncs.c \
		httpd/netbios.c
		
		

#		./debugTask/debugTask.c \
#		./comTask/comTask.c \
#		./configuration/configReader.c \
#		./graphicsLibrary/runGraphics.c \
#		./graphicsLibrary/renderGraphics.c \
#		./graphicsLibrary/graphicButton.c \
#		./graphicsLibrary/graphicTextBox.c \
#		./graphicsLibrary/graphicObjects.c \
#		./graphicsLibrary/port/rit128x96x4.c \
#		$(FATFS_COMMON_DIR)/port/fs.c \
#		$(FATFS_COMMON_DIR)/src/ff.c \
#		$(FATFS_COMMON_DIR)/port/mmc-lmi-rtos.c \

#old
#		./IntQueueTimer.c \
#		$(FATFS_COMMON_DIR)/port/fs.c \
#		./ParTest/ParTest.c \

#unsused
#		

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
	
