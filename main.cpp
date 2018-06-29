#include <iostream>

#include <math/matrix_utils.h>
#include <simon_wb/simon_wb_generator.h>

using namespace std;

void print_u8(uint8_t *out, int len) {
    int ct = 8;
    for (int i=0; i<len; i++) {
        if (i%ct==0) printf("\n");
        printf("%02X", *(out+i));
    }
    printf("\n");
}

int SWAN_Encrypt_64_main();
int SWAN_Encrypt_128_main();

#define __u32 unsigned int
#define __u8 unsigned char

static __u32 Gamma(__u32 x)
{
    __u8 t[4], r[4];
    __u8 * a = (__u8 *)&x;
    t[3] = ~(a[3]|a[2]);
    r[3] = a[0] ^ t[3];
    t[2] = ~(a[2]|a[1]);
    r[2] = a[3] ^ t[2];
    t[1] = ~(a[1] | r[3]);
    r[1] = ~(a[2] ^ t[1]);
    t[0] = ~(r[3] | r[2]);
    r[0] = a[1] ^ t[0];
    __u32 *ret = (__u32 *) r;
    return *ret;
}

int gen_gamma_table()
{
    __u32 test_vector[] = {0x00000000, 0x00000001, 
    0x00000100, 0x00000101, 
    0x00010000, 0x00010001,0x00010100, 0x00010101, 
    0x01000000, 0x01000001, 0x01000100, 0x01000101, 
    0x01010000, 0x01010001,0x01010100, 0x01010101};
    for (int i=0; i < 16; i++) {
        __u32 y = Gamma(test_vector[i]);
        __u32 k =  ((y & 0x01000000)>>21) |
                        ((y & 0x00010000)>>15) |
                        ((y & 0x00000100)>>7) |
                        ((y & 0x00000001));
        cout << i << ":" << k << endl;
    }
    return 0;
}
    
#include <simon/simon.h>

int main(int argc, char **argv) 
{
    CombinedAffine *ptr = (CombinedAffine*)malloc(sizeof(CombinedAffine));
    combined_affine_init(ptr, 4, 4);
    combined_affine_free(ptr);
    free(ptr);
    ptr = NULL;
    cout << "hello" << endl;

    uint8_t k[] = {0xff, 0xff, 0xff, 0xff, 0x2f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    uint8_t in[] = {0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t out[16] = {0};


    SimSpk_Cipher sc;
    Simon_Init(&sc, cfg_128_64, ECB, k, NULL, NULL);
    Simon_Encrypt(sc, in, out);
    print_u8(in, 8);
    print_u8(out, 8);

    simon_whitebox_content swc;
    simon_whitebox_64_init(k, SIMON_ENCRYPT, &swc);
    print_u8(in, 8);
    simon_wb_encrypt(&swc, in, out);
    print_u8(out, 8);
    simon_whitebox_release(&swc);


    // SWAN_Encrypt_64_main();
    // SWAN_Encrypt_128_main();
    // gen_gamma_table();    
    return 0;
}
