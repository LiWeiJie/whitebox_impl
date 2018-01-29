


#include <affine/NTLUtils.h>

#include <NTL/mat_GF2.h>
#include <NTL/vec_GF2.h>

// #include <LBlock/LBlock80.h>

void print_u8(uint8_t *out, int len) {
    int ct = 8;
    for (int i=0; i<len; i++) {
        if (i%ct==0) printf("\n");
        printf("%02X", *(out+i));
    }
    printf("\n");
}

int test_sms4_wb128(int idx);
int NTLTest();

int klein_test_main();

int lblock_test_main();


int main(int argc, char **argv) 
{
    
    int ct = 0;
    ct += test_sms4_wb128(0)==0;
    ct += test_sms4_wb128(1)==0;
    ct += test_sms4_wb128(2)==0;
    ct += test_sms4_wb128(3)==0;
    // ct += NTLTest()==0;

    printf("succesful count:\t%d/4\n", ct);
    
    printf("-------------klein test-------------\n");
    klein_test_main();

    printf("-------------LBlock test-------------\n");
    lblock_test_main();

    return 0;
}
