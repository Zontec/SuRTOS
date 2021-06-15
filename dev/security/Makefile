BUILD_DIR = build
SRC_DIR = src
INCLUDE_DIR = include


SRCS = main.c \
	$(SRC_DIR)/aes.c \
	$(SRC_DIR)/sha.c \
	$(SRC_DIR)/utils.c \
	$(SRC_DIR)/hmac.c \
	$(SRC_DIR)/pbkdf.c \
	$(SRC_DIR)/kbkdf.c \
	$(SRC_DIR)/rand.c \

	
OBJS = $(SRCS:$/%.c=$(BUILD_DIR)/%.o)

CFLAGS = $(addprefix -I, $(INCLUDES))

all: clean test

test: $(OBJS)
	@$(CC) $^ -o $@ -lssl -lcrypto
	

clean:
	@rm -rf build

$(OBJS): $(BUILD_DIR)/%.o : %.c
	@mkdir -p $(dir $@)
	@$(CC) -I$(INCLUDE_DIR) -c $< -o $@
	@echo cc -c $< -o $@
