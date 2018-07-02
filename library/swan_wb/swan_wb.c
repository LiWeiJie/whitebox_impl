#include <swan_wb/swan_wb.h>

unsigned char S[16] = {0x0e, 0x04, 0x0b, 0x02, 0x03, 0x08, 0x00, 0x09, 0x01, 0x0a, 0x07, 0x0f, 0x06, 0x0c, 0x05, 0x0d};

#define __u8 unsigned char
#define __u32 unsigned int
#define __u64 unsigned long long int

#include <math/matrix_utils.h>

int swan_whitebox_release(swan_whitebox_content *swc)
{
    // TODO:
    // AffineTransformFree(swc->)
    int i,j,k;
    for (i=0; i<swc->rounds * swc->aff_in_round; i++) {
        AffineTransformFree(swc->round_aff + i);
    }

    //free memory
    free(swc->round_aff);
    free(swc->lut);

    free(swc->SE);
    free(swc->EE);
    swc->round_aff = NULL;
    swc->lut = NULL;
    swc->SE = swc->EE = NULL;
    return 0;
}