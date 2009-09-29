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
COMMON_INCLUDE_DIR=../Common/include
LUMINARY_DRIVER_DIR=../Common/drivers/LuminaryMicro

LWIP_SOURCE_DIR=../Common/ethernet/lwip/lwip-1.3.0
WEB_SERVER_DIR=../Common/ethernet/lwip/lwip-1.3.0/Apps


NAME = Luminary_Webinterface


CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
LDSCRIPT=standalone.ld

# should use --gc-sections but the debugger does not seem to be able to cope with the option.
LINKER_FLAGS=-nostartfiles -Xlinker -o$(NAME).axf -Xlinker -M -Xlinker -Map=rtosdemo.map -Xlinker --no-gc-sections

DEBUG=-g 
OPTIM=-Os

#/*
#	#I am using linux to compile, and for a unknown reason in Eclipse environment
#	this is necessary.
#	If you are compiling using make from a commando line the COMPILE_DIR variable is not necessari.
#*/

CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
LDSCRIPT=standalone.ld

# should use --gc-sections but the debugger does not seem to be able to cope with the option.
LINKER_FLAGS=-nostartfiles -Xlinker -omain.elf -Xlinker -M -Xlinker -Map=main.map -Xlinker --no-gc-sections

DEBUG=-g
OPTIM=-Os

CFLAGS=$(DEBUG) $(OPTIM) -I .  -I $(RTOS_SOURCE_DIR)/include  -I $(LWIP_SOURCE_DIR)/src/include/ipv4 \
		-I $(LWIP_SOURCE_DIR)/src/include -I $(LWIP_SOURCE_DIR)/contrib/port/FreeRTOS/LM3S \
		-I $(COMMOM_INCLUDE) -I $(LWIP_DRIVER_DIR) \
		-I $(RTOS_SOURCE_DIR)/portable/GCC/ARM_CM3 -I $(LUMINARY_DRIVER_DIR) \
		-D GCC_ARMCM3_LM3S102   \
		-D inline= -mthumb -mcpu=cortex-m3 $(OPTIM) -T$(LDSCRIPT) \
		-D sprintf=usprintf -D snprintf=usnprintf -D printf=uipprintf \
		-ffunction-sections -fdata-sections
#		-D PACK_STRUCT_END=__attribute\(\(packed\)\) -D ALIGN_STRUCT_END=__attribute\(\(aligned\(4\)\)\) \
#-D LWIP_SOCKET


SOURCE=	main.c  \
		./ComTask/comTask.c\
		./GraphicsLibary/graphicObjects.c \
		./GraphicsLibary/runTestTask.c \
		rit128x96x4.c \
		$(RTOS_SOURCE_DIR)/list.c \
		$(RTOS_SOURCE_DIR)/queue.c \
		$(RTOS_SOURCE_DIR)/tasks.c \
		$(RTOS_SOURCE_DIR)/portable/GCC/ARM_CM3/port.c \
		$(RTOS_SOURCE_DIR)/portable/MemMang/heap_3.c \
		$(LWIP_SOURCE_DIR)/contrib/port/FreeRTOS/sys_arch.c \
		$(LWIP_SOURCE_DIR)/src/api/api_lib.c \
		$(LWIP_SOURCE_DIR)/src/api/api_msg.c \
		$(LWIP_SOURCE_DIR)/src/api/tcpip.c \
		$(LWIP_SOURCE_DIR)/src/api/err.c \
		$(LWIP_SOURCE_DIR)/src/api/sockets.c \
		$(LWIP_SOURCE_DIR)/src/api/netbuf.c \
		$(LWIP_SOURCE_DIR)/src/api/netdb.c \
		$(LWIP_SOURCE_DIR)/src/api/netifapi.c \
		$(LWIP_SOURCE_DIR)/src/core/dhcp.c \
		$(LWIP_SOURCE_DIR)/src/core/dns.c \
		$(LWIP_SOURCE_DIR)/src/core/init.c \
		$(LWIP_SOURCE_DIR)/src/core/mem.c \
		$(LWIP_SOURCE_DIR)/src/core/memp.c \
		$(LWIP_SOURCE_DIR)/src/core/netif.c \
		$(LWIP_SOURCE_DIR)/src/core/pbuf.c \
		$(LWIP_SOURCE_DIR)/src/core/raw.c \
		$(LWIP_SOURCE_DIR)/src/core/stats.c \
		$(LWIP_SOURCE_DIR)/src/core/sys.c \
		$(LWIP_SOURCE_DIR)/src/core/tcp.c \
		$(LWIP_SOURCE_DIR)/src/core/tcp_in.c \
		$(LWIP_SOURCE_DIR)/src/core/tcp_out.c \
		$(LWIP_SOURCE_DIR)/src/core/udp.c \
		$(LWIP_SOURCE_DIR)/src/core/ipv4/autoip.c \
		$(LWIP_SOURCE_DIR)/src/core/ipv4/icmp.c \
		$(LWIP_SOURCE_DIR)/src/core/ipv4/igmp.c \
		$(LWIP_SOURCE_DIR)/src/core/ipv4/inet.c \
		$(LWIP_SOURCE_DIR)/src/core/ipv4/inet_chksum.c \
		$(LWIP_SOURCE_DIR)/src/core/ipv4/ip_addr.c \
		$(LWIP_SOURCE_DIR)/src/core/ipv4/ip.c \
		$(LWIP_SOURCE_DIR)/src/core/ipv4/ip_frag.c
# WEB_SERVER_FILES: Files to create a web server application
WEB_SERVER_FILES=$(WEB_SERVER_DIR)/fs.c \
				$(WEB_SERVER_DIR)/ustdlib.c \
				$(WEB_SERVER_DIR)/httpd.c
# NETIFFILES: Files implementing various generic network interface functions.
NETIFFILES=$(LWIP_SOURCE_DIR)/src/netif/etharp.c \
		$(LWIP_SOURCE_DIR)/src/netif/loopif.c

LIBS= $(LUMINARY_DRIVER_DIR)/arm-none-eabi-gcc/libdriver.a $(LUMINARY_DRIVER_DIR)/arm-none-eabi-gcc/libgr.a

OBJS = $(SOURCE:.c=.o) $(NETIFFILES:.c=.o)   $(WEB_SERVER_FILES:.c=.o)

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
	rm $(OBJS) $(NAME).bin $(NAME).axf

flash : all
	openocd -f ../lm3s_scripts/lm3s_flash_start.cfg -c "flash write_bank 0 ./$(NAME).bin 0" -f ../lm3s_scripts/lm3s_flash_end.cfg 

doc : 
	doxygen ../../doc/code/DoxyFile
