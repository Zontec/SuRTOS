#include "utils.h"
void secure_clean(uint8_t *arr, uint32_t arr_len) {
    for (int i = 0; i < arr_len; i++) {
        *(arr + i) = 0xFF;
    }
}

void print_arr(void *arr, int len){
    printf("#####################################\n");
    printf("Len: %d\n", len);
    for (int i = 0; i < len; i++)
        printf("%02X", ((uint8_t*)arr)[i]);
    printf("\n");
    printf("#####################################\n");

}