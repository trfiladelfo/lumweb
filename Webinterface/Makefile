#/*
# * Copyright (c) 2008-2009 Open Software S/C Ltda
# * All rights reserved.
# *
# * Redistribution and use in source and binary forms, with or without modification,
# * are permitted provided that the following conditions are met:
# *
# * 1. Redistributions of source code must retain the above copyright notice,
# *    this list of conditions and the following disclaimer.
# * 2. Redistributions in binary form must reproduce the above copyright notice,
# *    this list of conditions and the following disclaimer in the documentation
# *    and/or other materials provided with the distribution.
# * 3. The name of the author may not be used to endorse or promote products
# *    derived from this software without specific prior written permission.
# *
# * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
# * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
# * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
# * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
# * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
# * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
# * OF SUCH DAMAGE.
# *
# *
# * Author: Paulo da Silva psilva@opensoftware-br.com
# *
# */

RTOS_SOURCE_DIR=../Source
LUMINARY_DRIVER_DIR=../Common/drivers/LuminaryMicro
LWIP_SOURCE_DIR=../Common/ethernet/lwip-1.3.0
LWIP_DRIVER_DIR=../Common/ethernet
WEB_SERVER_DIR=../Common/ethernet/lwip-1.3.0/Apps
COMMOM_INCLUDE=../Common/include

#/*
#	#I am using linux to compile, and for a unknown reason in Eclipse environment
#	this is necessary.
#	If you are compiling using make from a commando line the COMPILE_DIR variable is not necessari.
#*/

CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
LDSCRIPT=standalone.ld

NAME = Luminary_Webinterface


# should use --gc-sections but the debugger does not seem to be able to cope with the option.
LINKER_FLAGS=-nostartfiles -Xlinker -o$(NAME).axf -Xlinker -M -Xlinker -Map=rtosdemo.map -Xlinker --no-gc-sections


DEBUG=-g
OPTIM=-Os

CFLAGS=$(DEBUG) $(OPTIM) -I . \
		-I ./ComTask \
		-I ./GraphicsLibrary \
		-I $(RTOS_SOURCE_DIR)/include \
		-I $(LWIP_SOURCE_DIR)/src/include/ipv4 \
		-I $(LWIP_SOURCE_DIR)/src/include \
		-I $(LWIP_SOURCE_DIR)/contrib/port/FreeRTOS/LM3S \
		-I $(LWIP_DRIVER_DIR) \
		-I $(COMMOM_INCLUDE) \
		-I $(RTOS_SOURCE_DIR)/portable/GCC/ARM_CM3 \
		-I $(LUMINARY_DRIVER_DIR) \
		-D GCC_ARMCM3_LM3S102 -D LM3S6965_EK -D UART_BUFFERED  \
		-D inline= -mthumb -mcpu=cortex-m3 $(OPTIM) -T$(LDSCRIPT) \
		-D sprintf=usprintf -D snprintf=usnprintf -D printf=uipprintf \
		-ffunction-sections -fdata-sections
#		-D PACK_STRUCT_END=__attribute\(\(packed\)\) -D ALIGN_STRUCT_END=__attribute\(\(aligned\(4\)\)\) \
#-D LWIP_SOCKET


SOURCE=	main.c uart0.c uart1.c \
		rit128x96x4.c \
		./ComTask/comTask.c \
		./GraphicsLibrary/graphicObjects.c \
		./GraphicsLibrary/runTestTask.c \
		$(LWIP_DRIVER_DIR)/ETHIsr.c \
		$(LWIP_DRIVER_DIR)/LWIPStack.c \
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
# NETIFFILES: Files implementing PPP netif
NETIFFILES+=$(LWIP_SOURCE_DIR)/src/netif/ppp/auth.c $(LWIP_SOURCE_DIR)/src/netif/ppp/chap.c \
	$(LWIP_SOURCE_DIR)/src/netif/ppp/chpms.c $(LWIP_SOURCE_DIR)/src/netif/ppp/fsm.c \
	$(LWIP_SOURCE_DIR)/src/netif/ppp/ipcp.c $(LWIP_SOURCE_DIR)/src/netif/ppp/lcp.c \
	$(LWIP_SOURCE_DIR)/src/netif/ppp/magic.c $(LWIP_SOURCE_DIR)/src/netif/ppp/md5.c \
	$(LWIP_SOURCE_DIR)/src/netif/ppp/pap.c $(LWIP_SOURCE_DIR)/src/netif/ppp/ppp.c \
	$(LWIP_SOURCE_DIR)/src/netif/ppp/randm.c $(LWIP_SOURCE_DIR)/src/netif/ppp/vj.c \
	$(LWIP_SOURCE_DIR)/src/netif/sio.c

LIBS= $(LUMINARY_DRIVER_DIR)/arm-none-eabi-gcc/libdriver.a 

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


flash : all
	openocd -f ../lm3s_scripts/lm3s_flash_start.cfg -c "flash write_bank 0 ./$(NAME).bin 0" -f ../lm3s_scripts/lm3s_flash_end.cfg 

doc : 
	doxygen ../../doc/code/DoxyFile


clean :
	touch Makefile
	rm  -f $(OBJS)
	rm  -f *.o
	rm  -f *.map
	rm  -f *.bin
	rm  -f *.axf
	rm  -f *.elf
