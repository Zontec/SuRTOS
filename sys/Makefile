ifneq ($(filter STDIO, $(SYS)),)
    SRCS += stdio.c
endif

ifneq ($(filter STDUTILS, $(SYS)),)
    SRCS += stdutils.c
endif

ifneq ($(filter STACK, $(SYS)),)
    SRCS += stack.c
endif

ifneq ($(filter QUEUE, $(SYS)),)
    SRCS += queue.c
endif

INCLUDES += $(SYS_DIR)/include