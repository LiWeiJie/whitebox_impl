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

#include <simon/simon.h>

int main(int argc, char **argv) 
{
    double dur;
    clock_t start,end;
    

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

    start = clock();

    Simon_Init(&sc, cfg_128_64, ECB, k, NULL, NULL);
    Simon_Encrypt(sc, in, out);

    end = clock();

    print_u8(in, 8);
    print_u8(out, 8);

    dur = (double)(end - start);
    printf("\nEnc Use Time:%f s\n",(dur/CLOCKS_PER_SEC));

    simon_whitebox_content swc;

    start = clock();

    simon_whitebox_64_init(k, SIMON_ENCRYPT, &swc);

    end = clock();
    dur = (double)(end - start);
    printf("Generate Whitebox Use Time:%f s\n",(dur/CLOCKS_PER_SEC));


    print_u8(in, 8);

    start = clock();

    simon_wb_encrypt(&swc, in, out);

    end = clock();
    dur = (double)(end - start);
    printf("Whitebox Enc Use Time:%f s\n",(dur/CLOCKS_PER_SEC));

    print_u8(out, 8);
    simon_whitebox_release(&swc);


    // SWAN_Encrypt_64_main();
    // SWAN_Encrypt_128_main();
    // gen_gamma_table();    
    return 0;
}