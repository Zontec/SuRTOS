
/*
    char pass[64];
    char salt[SHA512_DIGEST_LENGTH];

    pbkdf2_sha256_rnd((uint8_t*)"1234", 4, (uint8_t*)"1234", 4, salt, pass);
    print_arr(pass, 64);
    printf("\n");
*/

/*

    unsigned char *src = "Hello world!";
    unsigned char *dst = (unsigned char*)calloc(1, 1024);
    AES_KEY key;
    unsigned char pass[] = {"password"};
    unsigned char iv[AES_BLOCK_SIZE];
    if(AES_set_decrypt_key(pass, 256, &key) < 0) {
		fprintf(stderr, "AES_set_decrypt_key failed in crypt!\n");
	}

    AES_cbc_encrypt(src, dst, AES_BLOCK_SIZE, &key, iv, AES_ENCRYPT);
    */