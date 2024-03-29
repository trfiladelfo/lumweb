# Makefile for the Luminary Project.
# This Makefile is made for the CodeSourcery G++ Lite Compiler and optimized for the LM3S9B96
#
# Please change only the first Part of this file. 
# The last part of this file contains the compiler options.
#
# Author:  Anzinger Martin, Hahn Florian
# Version: 1.0
# Date:	   2009 - 2010
#

NAME = Maschinen_Simulation

LUMINARY_DRIVER_DIR=external/lm3s9b96
RTOS_SOURCE_DIR=external/freeRTOS/Source
LWIP_COMMON_DIR=external/ethernet/lwip131
FATFS_COMMON_DIR=external/fatfs
GRLIB_COMMON_DIR=external/grlib
BGET_COMMON_DIR=external/bget

DOC_DIR=../doc

SOURCE_DIR=./uInterface
ETHERNET_DIR=$(SOURCE_DIR)/ethernet
GRAPHIC_DIR=$(SOURCE_DIR)/graphic
TAGLIB_DIR=$(SOURCE_DIR)/taglib
UART_DIR=$(SOURCE_DIR)/uart
COMM_DIR=$(SOURCE_DIR)/communication
CONF_DIR=$(SOURCE_DIR)/configuration

DOXYFILE=$(DOC_DIR)/product/code/DoxyFile

SOURCE=	syscall.c \
		$(SOURCE_DIR)/main.c \
		$(SOURCE_DIR)/setup.c \
		$(SOURCE_DIR)/timer.c \
		$(SOURCE_DIR)/realtime.c \
		$(SOURCE_DIR)/lmi_fs.c \
		$(SOURCE_DIR)/utils.c \
		$(CONF_DIR)/configloader.c \
		$(ETHERNET_DIR)/httpd/httpd.c \
		$(ETHERNET_DIR)/httpd/cgi/cgifuncs.c \
		$(ETHERNET_DIR)/httpd/cgi/ssiparams.c \
		$(ETHERNET_DIR)/httpd/cgi/io.c \
		$(ETHERNET_DIR)/LWIPStack.c \
		$(ETHERNET_DIR)/ETHIsr.c \
		$(COMM_DIR)/comTask.c \
		$(COMM_DIR)/impl/UARTImpl.c \
		$(UART_DIR)/uartstdio.c \
		$(RTOS_SOURCE_DIR)/list.c \
		$(RTOS_SOURCE_DIR)/queue.c \
		$(RTOS_SOURCE_DIR)/tasks.c \
		$(RTOS_SOURCE_DIR)/portable/GCC/ARM_CM3/port.c \
		$(RTOS_SOURCE_DIR)/portable/MemMang/heap_2.c \
		$(LWIP_COMMON_DIR)/src/core/dhcp.c \
		$(LWIP_COMMON_DIR)/src/core/dns.c \
		$(LWIP_COMMON_DIR)/port/sys_arch.c \
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
		$(LWIP_COMMON_DIR)/src/core/sntp.c \
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
		$(LWIP_COMMON_DIR)/src/netif/loopif.c \
		$(FATFS_COMMON_DIR)/ff.c \
		$(FATFS_COMMON_DIR)/mmc-dk-lm3s9b96.c \
		$(FATFS_COMMON_DIR)/SDcard.c \
		$(LUMINARY_DRIVER_DIR)/drivers/sound.c \
		$(LUMINARY_DRIVER_DIR)/drivers/touch.c \
		$(LUMINARY_DRIVER_DIR)/drivers/tlv320aic23b.c \
		$(LUMINARY_DRIVER_DIR)/drivers/kitronix320x240x16_ssd2119_8bit.c \
		$(LUMINARY_DRIVER_DIR)/driverlib/ustdlib.c \
		$(LUMINARY_DRIVER_DIR)/drivers/set_pinout.c \
		$(GRAPHIC_DIR)/graphicTask.c \
		$(GRAPHIC_DIR)/gui/touchActions.c \
		$(GRAPHIC_DIR)/gui/displayBasics.c \
		$(GRAPHIC_DIR)/gui/displayDraw.c \
		$(GRAPHIC_DIR)/gui/valueEditor.c \
		$(GRAPHIC_DIR)/httpc/webClient.c \
      	$(SOURCE_DIR)/log/logging.c \
      	$(TAGLIB_DIR)/taglib.c \
      	$(TAGLIB_DIR)/tags.c \
      	$(TAGLIB_DIR)/tags/CheckboxInputField.c \
      	$(TAGLIB_DIR)/tags/FloatInputField.c \
      	$(TAGLIB_DIR)/tags/Group.c \
      	$(TAGLIB_DIR)/tags/Hyperlink.c \
      	$(TAGLIB_DIR)/tags/IntegerInputField.c \
      	$(TAGLIB_DIR)/tags/SavedParams.c \
      	$(TAGLIB_DIR)/tags/SubmitInputField.c \
      	$(TAGLIB_DIR)/tags/TimeInputField.c \
      	$(TAGLIB_DIR)/tags/Titel.c \
      	$(TAGLIB_DIR)/tags/DefaultTags.c

SCRIPT_DIR=lm3s_scripts


###############################################################################
###############               PLEASE DO NOT EDIT!               ###############
###############################################################################

CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
LDSCRIPT=standalone.ld

LINKER_FLAGS= -Xlinker -o$(NAME).axf -Xlinker -M -Xlinker -Map=rtosdemo.map -T$(LDSCRIPT)
#-nostartfiles -Xlinker  --no-gc-sections
DEBUG=-g
OPTIM=-O1


CFLAGS=-I $(SOURCE_DIR) \
		-I external \
		-I $(SOURCE_DIR)/ethernet \
		-I $(LWIP_COMMON_DIR)/src/include \
		-I $(LWIP_COMMON_DIR)/src/include/ipv4 \
		-I $(LWIP_COMMON_DIR)/port/LM3S \
		-I $(RTOS_SOURCE_DIR)/include \
		-I $(RTOS_SOURCE_DIR)/portable/GCC/ARM_CM3 \
		-I $(LUMINARY_DRIVER_DIR) \
		-I $(LUMINARY_DRIVER_DIR)/drivers \
		-I $(LUMINARY_DRIVER_DIR)/driverlib \
		-I $(LUMINARY_DRIVER_DIR)/inc \
		-D inline= -mthumb -mcpu=cortex-m3 \
		$(OPTIM) $(DEBUG) \
		-ffunction-sections \
		-Wall \
		-D sprintf=usprintf -D snprintf=usnprintf \
		-D _sprintf=usprintf -D _snprintf=usnprintf \
		-D printf=UARTprintf \
		-Dgcc \
		-D malloc=pvPortMalloc -D free=vPortFree \
		
LIBS= $(LUMINARY_DRIVER_DIR)/driverlib/gcc/libdriver.a $(GRLIB_COMMON_DIR)/gcc/libgr.a  

OBJS = $(SOURCE:.c=.o)

all: $(NAME).bin

$(NAME).bin : $(NAME).axf
	$(OBJCOPY) $(NAME).axf -O binary $(NAME).bin

$(NAME).axf : $(OBJS) startup.o Makefile
	$(CC) $(CFLAGS) $(OBJS) startup.o $(LIBS) $(LINKER_FLAGS)

$(OBJS) : %.o : %.c Makefile $(SOURCE_DIR)/FreeRTOSConfig.h
	$(CC) -c $(CFLAGS) $< -o $@

startup.o : startup.c Makefile
	$(CC) -c $(CFLAGS) -O1 startup.c -o startup.o

clean :
	rm  -f $(OBJS)
	rm  -f *.o
	rm  -f *.map
	rm  -f *.bin
	rm  -f *.axf
	rm  -f *.elf

flash : all
	openocd -f $(SCRIPT_DIR)/lm3s_flash_start.cfg -c "flash write_bank 0 ./$(NAME).bin 0" -f $(SCRIPT_DIR)/lm3s_flash_end.cfg 

doc : 
	doxygen $(DOXYFILE)
	@echo "Output fertig erstellt"
	
cleanflash : clean flash
	
