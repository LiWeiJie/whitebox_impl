/*
 * @Author: Weijie Li 
 * @Date: 2018-06-27 14:45:46
 * @Last Modified by: Weijie Li
 * @Last Modified time: 2018-06-27 14:45:48
 */

#include <simon_wb/simon_wb.h>


#include <stdio.h>

#define SIMON_WB_VERBOSE 0

#define DUMP_VERBOSE(x, n)  if (SIMON_WB_VERBOSE) {print_u8(x, n);}


static void print_u8(const uint8_t *out, int len) {
    int ct = 8;
    for (int i=0; i<len; i++) {
        if (i%ct==0) printf("\n");
        printf("%02X", *(out+i));
    }
    printf("\n");
}

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
        *(x_byte + i) = swc->SE[i+0][*(in + piece_count + i)];
        *(y_byte + i) = swc->SE[i+piece_count][*(in + 0 + i)];
    }

    DUMP_VERBOSE(out, 8);

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
        uint32_t c = ApplyAffineToU32(*(aff_ptr++), *x_word);
        uint8_t* cptr = (uint8_t*)&c;

        // printf("%X %X %X %X\n", *x_word, a,b,c);

        uint32_t dst;
        uint8_t* desptr = (uint8_t*)&dst;
        for (i=0; i<piece_count; i++) {
            piece_t* and_ptr =swc->and_table[round_id*piece_count + i];
            *(desptr++) = and_ptr[*(aptr++)][*(bptr++)];
        }
        dst = dst ^ c;

        c = ApplyAffineToU32(*(aff_ptr++), *y_word);
        a = dst ^ c;
        // printf("%X %X", c, a);

        aptr = (uint8_t*)&a;
        desptr = (uint8_t*)&dst;
        for (i=0; i<piece_count; i++) {
            piece_t* lut_ptr = swc->lut + round_id * piece_count;;
            *(desptr++) = lut_ptr[i][*(aptr++)];
        }

        *y_word = *x_word;
        *x_word = dst;
        DUMP_VERBOSE(out, 8);
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


int simon_whitebox_release(simon_whitebox_content *swc)
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
    j = 1<<PIECE_SIZE;
    for (i=0; i < swc->rounds; i++)
    {
        for (k=0; k<swc->piece_count; k++) {
            free(swc->and_table[i*swc->piece_count + k]);
        }
    }
    free(swc->and_table);
    free(swc->SE);
    free(swc->EE);
    swc->round_aff = NULL;
    swc->lut = NULL;
    swc->and_table = NULL;
    swc->SE = swc->EE = NULL;
    return 0;
}

int simon_wb_export_to_bytes(const simon_whitebox_content* swc, uint8_t **dest)  
{
    if (*dest!=NULL) 
        return -1;
    int sz = 0;
    sz = sizeof(simon_whitebox_content);
    // sz += swc->rounds * swc->aff_in_round * sizeof(AffineTransform);    //round_aff
    sz += swc->rounds  * swc->piece_count * sizeof(piece_t);    //LUT
    sz += 2 * swc->piece_count * sizeof(piece_t);    //SE
    sz += 2 * swc->piece_count * sizeof(piece_t);    //EE
    int t = 1 << swc->piece_size;
    sz += swc->rounds * swc->piece_count * t * sizeof(piece_t);    //and table 

    void** ptr_list = malloc(swc->rounds * swc->aff_in_round * sizeof(void*));

    int i;
    for (i=0; i<swc->rounds * swc->aff_in_round; i++) {
        ptr_list[i] = ExportAffineToStr(swc->round_aff + i);
        sz += *(uint32_t*)ptr_list[i];
    }

    *dest = malloc(sz);
    *((uint32_t*)*dest)=sz;

    uint8_t * ds = *dest + sizeof(uint32_t);
    memcpy(ds, swc, sizeof(simon_whitebox_content));
    ds += sizeof(simon_whitebox_content);
    int k;

    k = swc->rounds  * swc->piece_count * sizeof(piece_t);
    memcpy(ds, swc->lut, k);
    ds += k;

    k = 2 * swc->piece_count * sizeof(piece_t);
    memcpy(ds, swc->SE, k);
    ds += k;

    k = 2 * swc->piece_count * sizeof(piece_t);
    memcpy(ds, swc->EE, k);
    ds += k;
    
    for (i=0; i<swc->rounds * swc->aff_in_round; i++) {
        k = *(uint32_t*)ptr_list[i];
        memcpy(ds, ptr_list[i], k);
        ds += k;
    }

    int j;
    for (i=0; i < swc->rounds; i++) {
        for (j=0; j<swc->piece_count; j++) {
            k = t * sizeof(piece_t);
            memcpy(ds, swc->and_table[i*swc->piece_count + j], k);
            ds += k;
        }
    }

    return sz;   
}

int  simon_wb_import_from_bytes(const uint8_t *source, simon_whitebox_content* swc) 
{

    const void * ptr = source;
    ptr += sizeof(uint32_t);
    
    memcpy(swc, ptr, sizeof(simon_whitebox_content));
    ptr += sizeof(simon_whitebox_content);

    int k;
    k = swc->rounds  * swc->piece_count * sizeof(piece_t);
    swc->lut = (piece_t*)malloc(k);
    memcpy(swc->lut, ptr, k);
    ptr += k;

    k = 2 * swc->piece_count * sizeof(piece_t);
    swc->SE = (piece_t*)malloc(k);
    memcpy(swc->SE, ptr, k);
    ptr += k;

    k = 2 * swc->piece_count * sizeof(piece_t);
    swc->EE = (piece_t*)malloc(k);
    memcpy(swc->EE, ptr, k);
    ptr += k;

    k = swc->rounds * swc->aff_in_round  * sizeof(AffineTransform);
    swc->round_aff = (AffineTransform *)malloc(k);
    int i;
    for (i=0; i<swc->rounds * swc->aff_in_round; i++) {
        uint32_t aff_sz = *((uint32_t*)ptr);
        *(swc->round_aff + i) = ImportAffineFromStr(ptr);
        ptr += aff_sz;
    }

    int j;
    swc->and_table = (piece_t**)malloc(swc->rounds * swc->piece_count * sizeof(piece_t*));
    int t = 1<<swc->piece_size;
    for (i=0; i < swc->rounds; i++) {
        for (j=0; j<swc->piece_count; j++) {
            k = t * sizeof(piece_t);
            swc->and_table[i*swc->piece_count + j] = (piece_t*)malloc(k);
            memcpy(swc->and_table[i*swc->piece_count + j], ptr, k);
            ptr += k;
        }
    }

    return 0;
}