TARGET=main
STMLIB=lib
CMSIS=CMSIS
STARTUP=startup
SRCDIR=src
SECDIR=security
BUILD_DIR=build
BIN=bin
INFO=info
###################################################

CC=arm-none-eabi-gcc
LD=arm-none-eabi-ld
AR=arm-none-eabi-ar
AS=arm-none-eabi-as
CP=arm-none-eabi-objcopy
OD=arm-none-eabi-objdump
SE=arm-none-eabi-size
SF=st-flash

###################################################
INC_DIRS += $(STMLIB)/
INC_DIRS += $(CMSIS)/
INC_DIRS += $(STARTUP)/
INC_DIRS += $(SRCDIR)/
INC_DIRS += $(SRCDIR)/Include
INC_DIRS += $(SECDIR)/Include

INC_DIRS += .

INCLUDE = $(addprefix -I,$(INC_DIRS))

###################################################

CFLAGS  +=  -specs=nosys.specs -T$(STARTUP)/stm32f407vg.ld -g -O0 -std=gnu99 -w
CFLAGS  +=  -mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb-interwork 
CFLAGS  += 	-mfloat-abi=hard 
CFLAGS += -mfpu=fpv4-sp-d16 
#CFLAGS  += 	-ffreestanding -ffunction-sections 

CFLAGS += -DMEMORY_MANAGER_POOL
CFLAGS += -DPRINT_ENABLE=FALSE $(INCLUDE)

###################################################

SRCS += main.c
SRCS += $(SRCDIR)/drv_uart.c
SRCS += $(SRCDIR)/serial.c
SRCS += $(SRCDIR)/stdutils.c
SRCS += $(SRCDIR)/stdio.c
SRCS += $(SRCDIR)/assert.c
SRCS += $(SRCDIR)/panic.c
SRCS += $(SRCDIR)/bootrom.c
SRCS += $(SRCDIR)/bootloader.c
SRCS += $(SRCDIR)/stack.c
SRCS += $(SRCDIR)/queue.c
SRCS += $(SRCDIR)/platform_specific.c
SRCS += $(SRCDIR)/cd.c
SRCS += $(SRCDIR)/flash.c

#SRCS += $(SRCDIR)/thread.c
SRCS += $(SRCDIR)/sched.c


#SRCS += $(SRCDIR)/memory_linear.c
SRCS += $(SRCDIR)/memory_pool.c
#SRCS += $(SRCDIR)/memory_buddy.c
#SRCS += $(SRCDIR)/memory_slab.c

SRCS += $(STMLIB)/stm32f4xx_gpio.c
SRCS += $(STMLIB)/stm32f4xx_rcc.c
SRCS += $(STMLIB)/stm32f4xx_usart.c
SRCS += $(STMLIB)/stm32f4xx_flash.c
SRCS += $(STMLIB)/stm32f4xx_syscfg.c
SRCS += $(STMLIB)/stm32f4xx_tim.c
SRCS += $(STMLIB)/misc.c

SRCS += $(SRCDIR)/light_stack.c

SRCS += $(SRCDIR)/cli.c
SRCS += $(SRCDIR)/cli_sha.c
SRCS += $(SRCDIR)/cli_pins.c
SRCS += $(SRCDIR)/cli_pbkdf2.c
SRCS += $(SRCDIR)/mbox.c
SRCS += $(SRCDIR)/cd.c



SRCS += $(SECDIR)/sha1.c
SRCS += $(SECDIR)/sha256.c
SRCS += $(SECDIR)/sha512.c

SRCS += $(SECDIR)/hmac_sha1.c
SRCS += $(SECDIR)/hmac_sha256.c
SRCS += $(SECDIR)/hmac_sha512.c

SRCS += $(SECDIR)/pbkdf2.c


SRCS += $(SECDIR)/rand.c
SRCS += $(SECDIR)/kbkdf.c
SRCS += $(SECDIR)/aes.c



#################STARTUP SRC#######################
SRCS += $(STARTUP)/stm32f407vg.S 
SRCS += $(STMLIB)/system_stm32f4xx.c



###################################################
OBJS = $(SRCS:$/%.c=$(BUILD_DIR)/%.o)

.PHONY: $(TARGET)

$(TARGET): 
	@echo STM32f407VGT6 uROS build started!
	$(MAKE) $(TARGET).elf

$(OBJS): $(BUILD_DIR)/%.o : %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo cc -c $(notdir $<) -o $(notdir $@)


$(TARGET).elf: $(OBJS)
	@$(CC) $(CFLAGS) $^ -o $(BIN)/$@
	$(CP) -O ihex $(BIN)/$(TARGET).elf $(BIN)/$(TARGET).hex
	$(CP) -O binary $(BIN)/$(TARGET).elf $(BIN)/$(TARGET).bin
	@echo Build done success!

clean:
	rm -f *.o $(BIN)/$(TARGET).elf $(BIN)/$(TARGET).hex $(BIN)/$(TARGET).bin
	rm -rf ./build

flash: 
	/home/user/STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin/STM32_Programmer_CLI -c port=SWD -w bin/main.hex

flash-dbg:
	/home/user/STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin/STM32_Programmer_CLI -c port=SWD -w bin/main.elf

start-server:
	gnome-terminal
	openocd -f $(shell pwd)/bin/stm32f4discovery.cfg

help:
	cat $(INFO)/README.info

debug-info:
	cat $(INFO)/README.debug