/*
 * @Author: Weijie Li 
 * @Date: 2018-06-27 14:45:46
 * @Last Modified by: Weijie Li
 * @Last Modified time: 2018-06-27 14:45:48
 */

#include <simon_wb/simon_wb.h>


int _simon_wb_64_enc(simon_whitebox_content * swc, const uint8_t *in, uint8_t *out) 
{
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
        *(x_byte + i) = swc->SE[i+0][*(in + piece_count)];
        *(y_byte + i) = swc->SE[i+piece_count][*(in + 0)];
    }

    int round_id = 0;
    int rounds = swc->rounds;
    AffineTransform * aff_ptr;
    aff_ptr = swc->round_aff;
    for (round_id = 0; round_id < rounds; round_id++) {
        uint32_t a = ApplyAffineToU32(*(aff_ptr++), *x_word);
        uint8_t* aptr = (uint8_t*)&a;
        uint32_t b = ApplyAffineToU32(*(aff_ptr++), *x_word);
        uint8_t* bptr = (uint8_t*)&b;
        uint32_t c = ApplyAffineToU32(*(aff_ptr++), *x_word);
        uint8_t* cptr = (uint8_t*)&c;

        uint32_t dst;
        uint8_t* desptr = (uint8_t*)&dst;
        for (i=0; i<piece_count; i++) {
            piece_t* and_ptr =swc->and_table[round_id*piece_count + i];
            *(desptr++) = and_ptr[*(aptr++)][*(bptr++)];
        }
        dst = dst ^ c;

        c = ApplyAffineToU32(*(aff_ptr++), *y_word);
        a = dst ^ c;
        aptr = (uint8_t*)&a;
        desptr = (uint8_t*)&dst;
        for (i=0; i<piece_count; i++) {
            piece_t* lut_ptr = swc->lut + round_id * piece_count;;
            *(desptr++) = lut_ptr[i][*(aptr++)];
        }

        *y_word = *x_word;
        *x_word = dst;
    }

    for (i=0; i<piece_count; i++) {
        *(x_byte + i) = swc->EE[i+0][*(x_byte + i)];
        *(y_byte + i) = swc->EE[i+piece_count][*(y_byte + i)];
    }
    return 0;
    // memcpy(x_byte, in + piece_count, piece_count);
    // memcpy(y_byte, in, piece_count);

    // uint8_t *round_key_ptr = (uint32_t *)key_schedule;

    // for (i=0; i<swc->rounds; i++) {

    // }
}

int simon_wb_enc(simon_whitebox_content * swc, const uint8_t *in, uint8_t *out) 
{
    if (swc->cfg==cfg_128_64) {
        return _simon_wb_64_enc(swc, in, out);
    }
    
    return -1;
}