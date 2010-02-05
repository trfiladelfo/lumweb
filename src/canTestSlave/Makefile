#/************************************************************************* 
# * Please ensure to read http://www.freertos.org/portLM3Sxxxx_Eclipse.html
# * which provides information on configuring and running this demo for the
# * various Luminary Micro EKs.
# *************************************************************************/

LUMINARY_DRIVER_DIR=../external/drivers/lm3s8962

NAME = CanTestSlave

CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
LDSCRIPT=standalone.ld

# should use --gc-sections but the debugger does not seem to be able to cope with the option.
LINKER_FLAGS=-nostartfiles -Xlinker -o$(NAME).axf -Xlinker -M -Xlinker -Map=rtosdemo.map -Xlinker --no-gc-sections

DEBUG=-g
OPTIM=-O0


CFLAGS=$(DEBUG) -I .  -D GCC_ARMCM3_LM3S102 -D inline= -mthumb -mcpu=cortex-m3 $(OPTIM) -T$(LDSCRIPT) \
		-D PACK_STRUCT_END=__attribute\(\(packed\)\) -D ALIGN_STRUCT_END=__attribute\(\(aligned\(4\)\)\) -D sprintf=usprintf  -D snprintf=usnprintf -D printf=uipprintf \
		 -ffunction-sections -fdata-sections -I $(LUMINARY_DRIVER_DIR) -I $(LUMINARY_DRIVER_DIR)/driverlib


SOURCE= can_test_slave.c \
		rit128x96x4.c

LIBS= $(LUMINARY_DRIVER_DIR)/driverlib/arm-none-eabi-gcc/libdriver.a $(LUMINARY_DRIVER_DIR)/driverlib/arm-none-eabi-gcc/libgr.a

OBJS = $(SOURCE:.c=.o)

all: $(NAME).bin

$(NAME).bin : $(NAME).axf
	$(OBJCOPY) $(NAME).axf -O binary $(NAME).bin

$(NAME).axf : $(OBJS) startup.o Makefile
	$(CC) $(CFLAGS) $(OBJS) startup.o $(LIBS) $(LINKER_FLAGS)

$(OBJS) : %.o : %.c Makefile
	$(CC) -c $(CFLAGS) $< -o $@

startup.o : startup.c Makefile
	$(CC) -c $(CFLAGS) -O1 startup.c -o startup.o

clean :
	touch Makefile
	rm $(OBJS) $(NAME).bin $(NAME).axf

flash : all
	openocd -f ../lm3s_scripts/lm3s8962_flash_start.cfg -c "flash write_bank 0 ./$(NAME).bin 0" -f ../lm3s_scripts/lm3s8962_flash_end.cfg 

