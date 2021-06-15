#Basic Makefile setup

BOARD ?= stm32f4-discovery
#BOARD ?= stm32f4xx

PROJECT ?= blink_led

OS_BASE_DIR := $(CURDIR)/../..

# Entry sources
SRCS = main.c

CFLAGS += -w

SECURITY += SHA1
SECURITY += SHA256
SECURITY += SHA512
SECURITY += HMAC_SHA1
SECURITY += HMAC_SHA256
SECURITY += HMAC_SHA512
SECURITY += AES
SECURITY += RAND
SECURITY += PBKDF2
SECURITY += KBKDF

LIB := SPL
CLI := ENABLED

SYS += STDIO
SYS += STACK
SYS += QUEUE
SYS += STDUTILS

INCLUDES += $(CURDIR)/

#FLASH_TOOL_IMAGE = -Uflash:w:build/$(PLATFORM)/blink_led.elf
FLASH_TOOL_IMAGE = build/$(PLATFORM)/blink_led.elf

include $(OS_BASE_DIR)/Makefile.include
