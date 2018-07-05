#include <iostream>

#include <math/matrix_utils.h>
#include <simon_wb/simon_wb_generator.h>
#include <simon/simon.h>

#include "count_cycles.h"

using namespace std;

static void print_u8(uint8_t *out, int len) {
    int ct = 8;
    for (int i=0; i<len; i++) {
        if (i%ct==0&&i) printf("\n");
        printf("%02X", *(out+i));
    }
    printf("\n");
}

int simon_bench_test(int times) 
{
    printf("[simon bench test]\n");
    printf("test times: %d\n", times);
    double dur;
    clock_t start,end;

    uint8_t k[] = {0xff, 0xf0, 0xff, 0xff, 0x2f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    uint8_t in[] = {0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t out[16] = {0};


    SimSpk_Cipher sc;

    set_time_start();

    Simon_Init(&sc, cfg_128_64, ECB, k, NULL, NULL);
    Simon_Encrypt(sc, in, out);

    set_time_ends();

    print_u8(in, 8);
    print_u8(out, 8);

    printf("Enc Use Time: %f s, %lld cycles\n", get_clock_elapsed(), get_cycles_elapsed());

    simon_whitebox_content swc;

    set_time_start();

    simon_whitebox_64_init(k, SIMON_ENCRYPT, &swc);

    set_time_ends();

    printf("Generate Whitebox Use Time: %f s, %lld cycles\n", get_clock_elapsed(), get_cycles_elapsed());


    print_u8(in, 8);

    set_time_start();

    simon_wb_encrypt(&swc, in, out);

    set_time_ends();

    print_u8(out, 8);
    printf("Whitebox Enc Use Time: %f s, %lld cycles\n", get_clock_elapsed(), get_cycles_elapsed());

    simon_whitebox_content swct;
    uint8_t * swct_ptr = NULL;

    simon_wb_export_to_bytes(&swc, &swct_ptr);

    printf("Whitebox bytes size %d\n", *(uint32_t*)swct_ptr);

    simon_wb_import_from_bytes(swct_ptr, &swct);

    set_time_start();

    simon_wb_encrypt(&swct, in, out);

    set_time_ends();


    print_u8(out, 8);
    printf("Whitebox copy Enc Use Time: %f s, %lld cycles\n", get_clock_elapsed(), get_cycles_elapsed());

    simon_whitebox_release(&swct);

    simon_whitebox_release(&swc);

    return 0;
}
