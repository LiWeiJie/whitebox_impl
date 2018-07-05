#include <iostream>

#include <math/matrix_utils.h>
#include <simon_wb/simon_wb_generator.h>

#include <test_bench.h>

using namespace std;

static void print_u8(uint8_t *out, int len) {
    int ct = 8;
    for (int i=0; i<len; i++) {
        if (i%ct==0) printf("\n");
        printf("%02X", *(out+i));
    }
    printf("\n");
}


int main(int argc, char **argv) 
{
    int test_times = 1;
    swan_bench_test(test_times);
    printf("\n");
    simon_bench_test(test_times);
    return 0;
}
