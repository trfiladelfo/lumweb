
NAME = Luminary_Webinterface

BOARD=lm3s9b96
#BOARD=lm3s8962
#Board = lm3s9b96 or lm3s8962

EXTERNAL=external
LUMINARY_DRIVER_DIR=$(EXTERNAL)/$(BOARD)/drivers
LUMINARY_DRIVER_LIB=$(EXTERNAL)/$(BOARD)/driverlib
LUMINARY_DRIVER_INC=$(EXTERNAL)/$(BOARD)/inc
GRAPHICLIB=$(EXTERNAL)/$(BOARD)/grlib
RTOS_SOURCE_DIR=$(EXTERNAL)/freeRTOS/Source
LWIP_COMMON_DIR=$(EXTERNAL)/ethernet/lwip-1.3.0
FATFS_COMMON_DIR=$(EXTERNAL)/fatfs




SOURCE_DIR=./uInterface
ETHERNET_DIR=$(SOURCE_DIR)/ethernet
GRAPHIC_DIR=$(SOURCE_DIR)/graphic
UART_DIR=$(SOURCE_DIR)/uart
COMM_DIR=$(SOURCE_DIR)/communication
CONF_DIR=$(SOURCE_DIR)/configuration

SCRIPT_DIR=lm3s_scripts

CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
LDSCRIPT=standalone_$(BOARD).ld

# should use --gc-sections but the debugger does not seem to be able to cope with the option.
LINKER_FLAGS=-nostartfiles -Xlinker -o$(NAME).axf -Xlinker -M -Xlinker -Map=rtosdemo.map -Xlinker --no-gc-sections

DEBUG=-g
OPTIM=-O0


CFLAGS=$(DEBUG) -I $(SOURCE_DIR) \
		-I $(EXTERNAL) \
		-I $(EXTERNAL)/$(BOARD) \
		-I ./ \
		-D GCC_ARMCM3_LM3S102 \
		-I $(LUMINARY_DRIVER_DIR) \
		-I $(LUMINARY_DRIVER_LIB) \
		-I $(LUMINARY_DRIVER_INC) \
		-I $(SOURCE_DIR)/ethernet \
		-I $(LWIP_COMMON_DIR)/src/include \
		-I $(LWIP_COMMON_DIR)/src/include/ipv4 \
		-I $(LWIP_COMMON_DIR)/ports/stellaris/include \
		-I $(RTOS_SOURCE_DIR)/include \
		-I $(RTOS_SOURCE_DIR)/portable/GCC/ARM_CM3 \
		-D inline= -mthumb -mcpu=cortex-m3 \
		$(OPTIM) -T$(LDSCRIPT) \
		-D sprintf=usprintf -D snprintf=usnprintf \
		-D _sprintf=usprintf -D _snprintf=usnprintf \
		-D malloc=pvPortMalloc -D free=vPortFree \
		-D printf=UARTprintf -D BOARD=${BOARD} \
		-ffunction-sections -fdata-sections \
		-fno-builtin-printf
		
SOURCE= $(SOURCE_DIR)/main.c \
		$(SOURCE_DIR)/setup_$(BOARD).c \
		$(SOURCE_DIR)/timer.c \
		$(SOURCE_DIR)/realtime.c \
		$(COMM_DIR)/comTask.c \
		$(UART_DIR)/uartstdio.c \
		$(RTOS_SOURCE_DIR)/list.c \
		$(RTOS_SOURCE_DIR)/queue.c \
		$(RTOS_SOURCE_DIR)/tasks.c \
		$(RTOS_SOURCE_DIR)/portable/GCC/ARM_CM3/port.c \
		$(RTOS_SOURCE_DIR)/portable/MemMang/heap_2.c \
		$(LUMINARY_DRIVER_LIB)/ustdlib.c \
		$(ETHERNET_DIR)/lwiplib.c \
		$(ETHERNET_DIR)/enet_lwip.c\
		$(ETHERNET_DIR)/lmi_fs.c\
		$(ETHERNET_DIR)/httpd/httpd.c \
		$(FATFS_COMMON_DIR)/src/ff.c \

#		$(FATFS_COMMON_DIR)/mmc-ek-lm3s8962.c \
		$(FATFS_COMMON_DIR)/SDcard.c 
		


LIBS = $(LUMINARY_DRIVER_LIB)/gcc/libdriver.a	

ifeq (${BOARD}, lm3s9b96)
SOURCE += \
		$(LUMINARY_DRIVER_DIR)/sound.c \
		$(LUMINARY_DRIVER_DIR)/set_pinout.c \
		$(LUMINARY_DRIVER_DIR)/touch.c \
		$(LUMINARY_DRIVER_DIR)/tlv320aic23b.c \
		$(FATFS_COMMON_DIR)/port/mmc-dk-lm3s9b96.c \
		$(LUMINARY_DRIVER_DIR)/kitronix320x240x16_ssd2119_8bit.c \
		$(GRAPHIC_DIR)/graphicTask.c \
		$(GRAPHIC_DIR)/settings/setip.c

		
LIBS += $(GRAPHICLIB)/gcc/libgr.a

endif

ifeq (${BOARD}, lm3s8962)
SOURCE += \
		$(GRAPHIC_DIR)/port/rit128x96x4.c \
		$(FATFS_COMMON_DIR)/port/mmc-ek-lm3s8962.c 
endif



OBJS = $(SOURCE:.c=.o)

all: $(NAME).bin

$(NAME).bin : $(NAME).axf
	$(OBJCOPY) $(NAME).axf -O binary $(NAME).bin

$(NAME).axf : $(OBJS) startup_$(BOARD).o Makefile
	$(CC) $(CFLAGS) $(OBJS) startup_$(BOARD).o $(LIBS) $(LINKER_FLAGS)

$(OBJS) : %.o : %.c Makefile
	$(CC) -c $(CFLAGS) $< -o $@

startup_$(BOARD).o : startup_$(BOARD).c Makefile
	$(CC) -c $(CFLAGS) -O1 startup_$(BOARD).c -o startup_$(BOARD).o

clean :
	touch Makefile
	rm  -f $(OBJS)
	rm  -f *.o
	rm  -f *.map
	rm  -f *.bin
	rm  -f *.axf
	rm  -f *.elf

flash : all
	openocd -f $(SCRIPT_DIR)/$(BOARD).cfg -f $(SCRIPT_DIR)/lm3s_flash_start.cfg -c "flash write_bank 0 ./$(NAME).bin 0" -f $(SCRIPT_DIR)/lm3s_flash_end.cfg
	
	

doc : 
	doxygen ../../doc/code/DoxyFile
	
cleanflash : clean flash

openocdserver : 
	openocd -f $(SCRIPT_DIR)/$(BOARD).cfg
