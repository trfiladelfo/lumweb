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
LUMINARY_DRIVER_DIR=../Common/drivers/LuminaryMicro

LWIP_DIR=../Common/ethernet/lwip/lwip-1.3.1/src
LWIP_PORT_DIR=../Common/ethernet/lwip/lwip-1.3.1/port



NAME = Luminary_Webinterface


CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
LDSCRIPT=standalone.ld

# should use --gc-sections but the debugger does not seem to be able to cope with the option.
LINKER_FLAGS=-nostartfiles -Xlinker -o$(NAME).axf -Xlinker -M -Xlinker -Map=rtosdemo.map -Xlinker --no-gc-sections

DEBUG=-g
OPTIM=-O0


CFLAGS=$(DEBUG) -I . -I $(RTOS_SOURCE_DIR)/include -I $(RTOS_SOURCE_DIR)/portable/gcc/ARM_CM3 \
		-I $(DEMO_INCLUDE_DIR) \
		-D GCC_ARMCM3_LM3S102 -D inline= -mthumb -mcpu=cortex-m3 $(OPTIM) -T$(LDSCRIPT) \
		-D PACK_STRUCT_END=__attribute\(\(packed\)\) -D ALIGN_STRUCT_END=__attribute\(\(aligned\(4\)\)\) -D sprintf=usprintf -D snprintf=usnprintf -D printf=uipprintf \
		-ffunction-sections -fdata-sections -I $(LUMINARY_DRIVER_DIR) \
		-I $(LWIP_DIR)/include -I $(LWIP_DIR)/include/ipv4 -I $(LWIP_DIR)/include/lwip -I $(LWIP_DIR)/include/netif -I $(LWIP_DIR)/include/ipv4/lwip -I $(LWIP_PORT_DIR)/include  

SOURCE=	main.c \
		timertest.c \
		rit128x96x4.c \
		osram128x64x4.c \
		formike128x128x16.c \
		./ComTask/comTask.c \
		./ParTest/ParTest.c \
		$(LUMINARY_DRIVER_DIR)/ustdlib.c \
		$(DEMO_COMMON_DIR)/BlockQ.c \
		$(DEMO_COMMON_DIR)/blocktim.c \
		$(DEMO_COMMON_DIR)/death.c \
		$(DEMO_COMMON_DIR)/integer.c \
		$(DEMO_COMMON_DIR)/PollQ.c \
		$(DEMO_COMMON_DIR)/semtest.c \
		$(DEMO_COMMON_DIR)/GenQTest.c \
		$(DEMO_COMMON_DIR)/QPeek.c \
		$(DEMO_COMMON_DIR)/recmutex.c \
		$(DEMO_COMMON_DIR)/IntQueue.c \
		./IntQueueTimer.c \
		./GraphicsLibary/runTestTask.c \
		./GraphicsLibary/graphicObjects.c \
		$(RTOS_SOURCE_DIR)/list.c \
		$(RTOS_SOURCE_DIR)/queue.c \
		$(RTOS_SOURCE_DIR)/tasks.c \
		$(RTOS_SOURCE_DIR)/portable/GCC/ARM_CM3/port.c \
		$(RTOS_SOURCE_DIR)/portable/MemMang/heap_2.c \
		$(LWIP_DIR)/api/api_lib.c \
        $(LWIP_DIR)/api/api_msg.c \
        $(LWIP_DIR)/api/err.c \
        $(LWIP_DIR)/api/netbuf.c \
        $(LWIP_DIR)/api/netifapi.c \
        $(LWIP_DIR)/api/tcpip.c \
        $(LWIP_DIR)/core/ipv4/icmp.c \
        $(LWIP_DIR)/core/ipv4/inet.c \
        $(LWIP_DIR)/core/ipv4/inet_chksum.c \
        $(LWIP_DIR)/core/ipv4/ip.c \
        $(LWIP_DIR)/core/ipv4/ip_addr.c \
        $(LWIP_DIR)/core/ipv4/ip_frag.c \
        $(LWIP_DIR)/core/dhcp.c \
        $(LWIP_DIR)/core/dns.c \
        $(LWIP_DIR)/core/init.c \
        $(LWIP_DIR)/core/mem.c \
        $(LWIP_DIR)/core/memp.c \
        $(LWIP_DIR)/core/netif.c \
        $(LWIP_DIR)/core/pbuf.c \
        $(LWIP_DIR)/core/raw.c \
        $(LWIP_DIR)/core/stats.c \
        $(LWIP_DIR)/core/sys.c \
        $(LWIP_DIR)/core/tcp.c \
        $(LWIP_DIR)/core/tcp_in.c \
        $(LWIP_DIR)/core/tcp_out.c \
        $(LWIP_DIR)/core/udp.c \
        $(LWIP_DIR)/core/snmp/asn1_dec.c \
        $(LWIP_DIR)/core/snmp/asn1_enc.c \
        $(LWIP_DIR)/core/snmp/mib_structs.c \
        $(LWIP_DIR)/core/snmp/mib2.c \
        $(LWIP_DIR)/core/snmp/msg_in.c \
        $(LWIP_DIR)/core/snmp/msg_out.c \
        $(LWIP_DIR)/netif/etharp.c \
        $(LWIP_PORT_DIR)/sys_arch.c \
        $(LWIP_PORT_DIR)/netif/stellarisif.c 

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
	rm $(OBJS) $(NAME).bin $(NAME).axf

flash : all
	openocd -f ../lm3s_scripts/lm3s_flash_start.cfg -c "flash write_bank 0 ./$(NAME).bin 0" -f ../lm3s_scripts/lm3s_flash_end.cfg 

doc : 
	doxygen ../../doc/code/DoxyFile
