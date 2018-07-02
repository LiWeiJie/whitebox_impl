#include <swan_wb/swan_wb.h>


#include <math/matrix_utils.h>


#define SWAN_WB_VERBOSE 0

#define DUMP_VERBOSE(x, n)  if (SWAN_WB_VERBOSE) {print_u8(x, n);}


static void print_u8(const uint8_t *out, int len) {
    int ct = 8;
    for (int i=0; i<len; i++) {
        if (i%ct==0) printf("\n");
        printf("%02X", *(out+i));
    }
    printf("\n");
}

int _swan_wb_64_enc(swan_whitebox_content * swc, const uint8_t *in, uint8_t *out) {
    if (in==NULL || out==NULL)
        return -1;

    int piece_count = swc->piece_count;
    uint8_t* temp;
    temp = (uint8_t*) malloc(piece_count * sizeof(uint8_t));

    uint8_t * x_byte = out + piece_count;
    uint8_t * y_byte = out;

    uint32_t * x_word = ((uint32_t *)out)+1;
    uint32_t * y_word = ((uint32_t *)out)+0;

    int i,j;
    for (i=0; i<piece_count; i++) {
        *(x_byte + i) = swc->SE[i+0][*(in + piece_count + i)];
        *(y_byte + i) = swc->SE[i+piece_count][*(in + 0 + i)];
    }

    int round_id = 0;
    int rounds = swc->rounds;
    AffineTransform * aff_ptr;
    aff_ptr = swc->round_aff;

    for (round_id = 0; round_id < rounds; round_id++) {
        // printf("Round %d:\t", round_id+1);

        uint32_t a = ApplyAffineToU32(*(aff_ptr++), *x_word);
        uint8_t* aptr = (uint8_t*)&a;
        uint32_t b = ApplyAffineToU32(*(aff_ptr++), *x_word);
        uint8_t* bptr = (uint8_t*)&b;

        uint32_t c = a ^ b;
        uint8_t* cptr = (uint8_t*)&c;

        c = ApplyAffineToU32(*(aff_ptr++), c);

        uint32_t dst;
        uint8_t* desptr = (uint8_t*)&dst;
        
        for (i=0; i<piece_count; i++) {
            piece_t* lut_ptr = swc->lut + round_id * piece_count;;
            *(desptr++) = lut_ptr[i][*(cptr++)];
        }

        c = ApplyAffineToU32(*(aff_ptr++), c);

        a = ApplyAffineToU32(*(aff_ptr++), c);
        b = ApplyAffineToU32(*(aff_ptr++), c);

        dst = a ^ b;

        // printf("%X %X %X %X\n", *x_word, a,b,c);

        c = ApplyAffineToU32(*(aff_ptr++), *y_word);
        dst = dst ^ c;
        // printf("%X %X", c, a);

        *y_word = *x_word;
        *x_word = dst;
        DUMP_VERBOSE(out, 8);
    }

    for (i=0; i<piece_count; i++) {
        *(x_byte + i) = swc->EE[i+0][*(x_byte + i)];
        *(y_byte + i) = swc->EE[i+piece_count][*(y_byte + i)];
    }


    return 0;
}


int swan_wb_enc(swan_whitebox_content * swc, const uint8_t *in, uint8_t *out) {
    if (swc->cfg==swan_cfg_128_64) {
        return _swan_wb_64_enc(swc, in, out);
    }
    return -1;
}

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