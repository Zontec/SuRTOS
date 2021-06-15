#include "sha.h"
#include "utils.h"
#include <stdio.h>

int main(){

    char str[] = "password";
    char str1[] = "pass";
    char str2[] = "word";
	uint32_t status;
	uint8_t out[66];
    status = SHA(SHA_256, str, sizeof(str), out);
    print_arr(out, get_sha_hash_size(SHA_256));
    printf("%d\n", status);


    return 0;
}
