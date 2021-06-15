CC =        arm-none-eabi-gcc
ASM =       arm-none-eabi-gcc
LD =        arm-none-eabi-gcc
OBJCPY =    arm-none-eabi-objcopy
OBJDMP =    arm-none-eabi-objdump
DBG =       arm-none-eabi-gdb

CFLAGS  += -O0                  \
            -g                  \
            -Wall               \
            -mcpu=cortex-m4     \
            -mthumb             \
            -mfpu=fpv4-sp-d16   \
            -mfloat-abi=hard    \
            -std=c11            \
            -I. $(INCLUDE)


ASMFLAGS += $(CFLAGS)
LDFLAGS += -mthumb -mcpu=cortex-m4 -nostdlib

CUR_MAKEIFLE_DIR = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))


SRCS += entry.S
SRCS += platform_specific.c
SRCS += kmal.c
SRCS += isr.c

ifneq ($(filter STDIO, $(SYS)),)
    SRCS += serial.c
endif


INCLUDES += $(PLATFORM_SPECIFIC_DIR)/include

include $(PLATFORM_SPECIFIC_DIR)/Makefile.include
