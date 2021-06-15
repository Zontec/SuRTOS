ifneq ($(filter AES,$(SECURITY)),)
	SRCS += aes.c
endif

ifneq ($(filter HMAC_SHA1, $(SECURITY)),)
	SRCS += hmac_sha1.c
endif

ifneq ($(filter HMAC_SHA256, $(SECURITY)),)
	SRCS += hmac_sha256.c
endif

ifneq ($(filter HMAC_SHA512, $(SECURITY)),)
	SRCS += hmac_sha512.c
endif

ifneq ($(filter SHA1, $(SECURITY)),)
	SRCS += sha1.c
endif

ifneq ($(filter SHA256, $(SECURITY)),)
	SRCS += sha256.c
endif

ifneq ($(filter SHA512, $(SECURITY)),)
	SRCS += sha512.c
endif

ifneq ($(filter RAND, $(SECURITY)),)
	SRCS += rand.c
endif

ifneq ($(filter PBKDF2, $(SECURITY)),)
	SRCS += pbkdf2.c
endif

ifneq ($(filter KBKDF, $(SECURITY)),)
	SRCS += kbkdf.c
endif

INCLUDES += $(SECURITY_DIR)/include/

