CC = avr-gcc
ASM = avr-as
LD = avr-gcc
OBJCPY = avr-objcopy
OBJDMP = avr-objdump
DBG = avr-gdb

CFLAGS += -mmcu=atmega328p -ffreestanding 
ASMFLAGS += -mmcu=atmega328p 
LDFLAGS += -mmcu=atmega328p -ffreestanding -fzero-link -nodevicelib
CUR_MAKEIFLE_DIR = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

ifneq ($(filter product_info, $(LIBS)),)
    SRCS += product_info.c
endif

SRCS += entry.S

include $(CUR_MAKEIFLE_DIR)/Makefile.include
