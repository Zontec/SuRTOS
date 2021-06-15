

out:
	@rm -f a.out
	@gcc -w -g main.c sha1.c sha256.c sha512.c hmac_sha1.c  \
	pbkdf2.c hmac_sha256.c hmac_sha512.c rand.c kbkdf.c \
	\
	aes.c -lcrypto -lssl
	@echo "Compilation done!"