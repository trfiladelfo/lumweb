NAME = Luminary_Webinterface

LUMINARY_DRIVER_DIR=external/drivers
RTOS_SOURCE_DIR=external/freeRTOS/Source
LWIP_COMMON_DIR=external/ethernet/lwip131
FATFS_COMMON_DIR=external/fatfs
GRLIB_COMMON_DIR=external/grlib
BGET_COMMON_DIR=external/bget

SOURCE_DIR=./uInterface
ETHERNET_DIR=$(SOURCE_DIR)/ethernet
GRAPHIC_DIR=$(SOURCE_DIR)/graphic
UART_DIR=$(SOURCE_DIR)/uart
COMM_DIR=$(SOURCE_DIR)/communication
CONF_DIR=$(SOURCE_DIR)/configuration

SCRIPT_DIR=lm3s_scripts

CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
LDSCRIPT=standalone.ld

# should use --gc-sections but the debugger does not seem to be able to cope with the option.
LINKER_FLAGS=-nostartfiles -Xlinker -o$(NAME).axf -Xlinker -M -Xlinker -Map=rtosdemo.map -Xlinker --gc-sections

DEBUG=-g
OPTIM=-O5


CFLAGS=$(DEBUG) -I $(SOURCE_DIR) \
		-I external \
		-I $(SOURCE_DIR)/ethernet \
		-I $(LWIP_COMMON_DIR)/src/include \
		-I $(LWIP_COMMON_DIR)/src/include/ipv4 \
		-I $(LWIP_COMMON_DIR)/port/LM3S \
		-I $(RTOS_SOURCE_DIR)/include \
		-I $(RTOS_SOURCE_DIR)/portable/GCC/ARM_CM3 \
		-D GCC_ARMCM3_LM3S102 \
		-I $(LUMINARY_DRIVER_DIR) \
		-I $(LUMINARY_DRIVER_DIR)/driverlib \
		-I $(LUMINARY_DRIVER_DIR)/inc \
		-D inline= -mthumb -mcpu=cortex-m3 \
		$(OPTIM) -T$(LDSCRIPT) \
		-D sprintf=usprintf -D snprintf=usnprintf \
		-D _sprintf=usprintf -D _snprintf=usnprintf \
		-D malloc=pvPortMalloc -D free=vPortFree \
		-D printf=UARTprintf -D gcc=1 \
		-ffunction-sections -fdata-sections 

SOURCE=	$(SOURCE_DIR)/main.c \
		$(SOURCE_DIR)/setup.c \
		$(SOURCE_DIR)/timer.c \
		$(SOURCE_DIR)/realtime.c \
		$(ETHERNET_DIR)/lmi_fs.c\
		$(ETHERNET_DIR)/httpd/httpd.c \
		$(ETHERNET_DIR)/httpd/cgi/cgifuncs.c \
		$(ETHERNET_DIR)/httpd/cgi/io.c \
		$(ETHERNET_DIR)/LWIPStack.c \
		$(ETHERNET_DIR)/ETHIsr.c \
		$(COMM_DIR)/comTask.c \
		$(LUMINARY_DRIVER_DIR)/set_pinout.c \
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
		$(LUMINARY_DRIVER_DIR)/driverlib/ustdlib.c \
		$(FATFS_COMMON_DIR)/ff.c \
		$(FATFS_COMMON_DIR)/mmc-dk-lm3s9b96.c \
		$(FATFS_COMMON_DIR)/SDcard.c \
		$(LUMINARY_DRIVER_DIR)/sound.c \
		$(LUMINARY_DRIVER_DIR)/touch.c \
		$(LUMINARY_DRIVER_DIR)/tlv320aic23b.c \
		$(LUMINARY_DRIVER_DIR)/kitronix320x240x16_ssd2119_8bit.c \
		$(GRAPHIC_DIR)/graphicTask.c \
		$(GRAPHIC_DIR)/graphicLib.c \
		$(GRAPHIC_DIR)/graphicWebClient.c
		
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
	touch Makefile
	rm  -f $(OBJS)
	rm  -f *.o
	rm  -f *.map
	rm  -f *.bin
	rm  -f *.axf
	rm  -f *.elf

flash : all
	openocd -f $(SCRIPT_DIR)/lm3s_flash_start.cfg -c "flash write_bank 0 ./$(NAME).bin 0" -f $(SCRIPT_DIR)/lm3s_flash_end.cfg 

doc : 
	doxygen ../../doc/code/DoxyFile
	
cleanflash : clean flash
	
