/*
 * @Author: Weijie Li 
 * @Date: 2018-07-01 16:01:24
 * @Last Modified by: Weijie Li
 * @Last Modified time: 2018-07-03 00:43:21
 */

#include <swan_wb/swan_wb_generator.h>

#include <math/matrix_utils.h>

#define MAX_ROUND_SIZE 10
#define MAX_RK_SIZE MAX_ROUND_SIZE*8

#include <stdio.h>
typedef struct swan_whitebox_helper 
{
    enum swan_cipher_config_t cfg;
    int aff_in_round;
    int block_size;
    int rounds;
    int piece_count; 
    CombinedAffine *ca;   // aff_in_round AFFINE for every round, rounds+1 needed
    CombinedAffine se[2]; // start encode
    // CombinedAffine ee[2]; // end encode
    int shift_in_round; 
    MatGf2* shift_matrix;   //shift_in_round in a round
    MatGf2 special_transposition, special_transposition_inv;
    uint8_t key_schedule[MAX_RK_SIZE];
} swan_whitebox_helper;

static unsigned char S[16] = {0x0e, 0x04, 0x0b, 0x02, 0x03, 0x08, 0x00, 0x09, 0x01, 0x0a, 0x07, 0x0f, 0x06, 0x0c, 0x05, 0x0d};

static unsigned char double_S[256];


static MatGf2 make_rotate_shift(int dim, int r)
{
    MatGf2 ind = GenMatGf2(dim, dim);
    int i;
    for (i=0; i<dim; i++) {
        MatGf2Set(ind, i, (i+r)%dim, 1);
    }
    // DumpMatGf2(ind);
    return ind;
}

static MatGf2 make_right_rotate_shift(int dim, int r)
{
    MatGf2 ind = GenMatGf2(dim, dim);
    int i;
    for (i=0; i<dim; i++) {
        MatGf2Set(ind, i, (i+dim-r)%dim, 1);
    }
    // DumpMatGf2(ind);
    return ind;
}

static MatGf2 make_special_transposition(int dim)
{
    MatGf2 ind = GenMatGf2(dim, dim);
    int i,j;
    int k = dim/4;
    j=0;
    int pos = 0;
    for (i=0; i<k; i++) {
        for (j=0; j<4; j++) {
            MatGf2Set(ind, pos++, i + j*k, 1);
            // printf("%d\t", i + j*k);
        }
    }
    // DumpMatGf2(ind);
    return ind;
}


int _swan_whitebox_helper_init(swan_whitebox_helper *swh)
{
    swh->shift_in_round = 5;
    swh->aff_in_round = 5;
    int block_size = swh->block_size ;
    int semi_block = block_size/2;
    swh->piece_count = semi_block/SWAN_PIECE_BIT;
    swh->ca = (CombinedAffine *)malloc( swh->rounds * swh->aff_in_round * sizeof(CombinedAffine));
    swh->shift_matrix = (MatGf2*) malloc(swh->shift_in_round * sizeof(MatGf2));

    int i,j;
    CombinedAffine * ptr = swh->ca;
    for (i=0; i < swh->rounds; i++) {
        for (j=0; j < swh->aff_in_round; j++) {
            combined_affine_init(ptr++, SWAN_PIECE_BIT, swh->piece_count);
        }
    }
    combined_affine_init(&(swh->se[0]), SWAN_PIECE_BIT, swh->piece_count);
    combined_affine_init(&(swh->se[1]), SWAN_PIECE_BIT, swh->piece_count);

    *(swh->shift_matrix + 0) = make_right_rotate_shift(semi_block,2);
    *(swh->shift_matrix + 1) = make_rotate_shift(semi_block,5);
    *(swh->shift_matrix + 2) = make_right_rotate_shift(semi_block,8);
    *(swh->shift_matrix + 3) = make_rotate_shift(semi_block,1);

    swh->special_transposition = make_special_transposition(semi_block);
    swh->special_transposition_inv = GenMatGf2Inv(swh->special_transposition);
    
    // DumpMatGf2(swh->special_transposition);
    // DumpMatGf2(swh->special_transposition_inv);
    return 0;
}

int round_key_schedule(const uint8_t* key, swan_whitebox_helper *swh)
{
    uint8_t * ptr = swh->key_schedule;
    const uint8_t* kp = key;
    int round = swh->rounds;
    int i,j;
    int bs = swh->block_size;
    int t = bs/16;
    for (i=0; i< round; i++) {
        for (j=0; j<t; j++) {
            *ptr = *kp ^ (uint8_t)i;
            ptr++;
            kp++;
        }
    }
    return 0;
}

int swan_whitebox_64_helper_init(const uint8_t *key, swan_whitebox_helper *swh)
// designed for swan_64_128
{
    
    // uint8_t rk[MAX_RK_SIZE];
    int ret;
    swh->cfg = swan_cfg_128_64;
    swh->rounds = swan_cfg_rounds[swh->cfg];
    swh->block_size = swan_cfg_blocksizes[swh->cfg];

    ret = round_key_schedule(key, swh);
    if (ret != 0) {
        return ret;
    }
    // memcpy(swh->key_schedule, rk, MAX_RK_SIZE);
    return _swan_whitebox_helper_init(swh);
}

int swan_whitebox_helper_release(swan_whitebox_helper *swh)
{
    int i,j;
    CombinedAffine * ptr = swh->ca;
    for (i=0; i < swh->rounds; i++) {
        for (j=0; j < swh->aff_in_round; j++) {
             combined_affine_free(ptr++);
        }
    }
    free(swh->ca);
    swh->ca = NULL;
    combined_affine_free(&swh->se[0]);
    combined_affine_free(&swh->se[1]);
    for (i=0; i < swh->shift_in_round; i++) {
        MatGf2Free(*(swh->shift_matrix + i));
    }
    free(swh->shift_matrix);
    swh->shift_matrix = NULL;

    MatGf2Free(swh->special_transposition);
    MatGf2Free(swh->special_transposition_inv);
    return 0;
}


int _swan_whitebox_content_init(swan_whitebox_helper* swh, swan_whitebox_content* swc) 
{
    // TODO:
    swc->cfg = swh->cfg;
    swc->aff_in_round = 7;
    swc->rounds = swh->rounds;
    swc->block_size = swh->block_size;
    swc->piece_count = swh->piece_count;
    swc->round_aff = (AffineTransform *)malloc(swc->rounds * swc->aff_in_round  * sizeof(AffineTransform));
    swc->lut = (piece_t*)malloc(swc->rounds  * swc->piece_count * sizeof(piece_t));
    int i,j,k;
    swc->SE = (piece_t*)malloc(2 * swc->piece_count * sizeof(piece_t));
    swc->EE = (piece_t*)malloc(2 * swc->piece_count * sizeof(piece_t));
    // printf("%lu\n", sizeof(piece_t));
    // printf("%lu\n", swc->piece_count * sizeof(piece_t));
    // printf("0x%lx\n", (unsigned long)swc->lut);
    // piece_t* ptr = swc->lut+1;
    // printf("0x%lx\n", (unsigned long)ptr);
    
    return 0;
}

int swan_whitebox_64_content_init(swan_whitebox_helper* swh, swan_whitebox_content* swc) 
{
    return _swan_whitebox_content_init(swh, swc);
}

int _swan_whitebox_content_assemble(swan_whitebox_helper* swh, swan_whitebox_content* swc)
{
    assert(SWAN_PIECE_BIT==8);
    int piece_count = swh->piece_count;
    // start encode and end encode
    int i,j,k;
    int piece_range = 1<<SWAN_PIECE_BIT;

    CombinedAffine * last_round_ca_ptr = swh->ca + (swh->rounds-1) * swh->aff_in_round;
    for (i=0; i < piece_count; i++) {
        for (j=0; j<piece_range; j++) {
            swc->SE[i][j] = ApplyAffineToU8(swh->se[0].sub_affine[i], j);
            swc->EE[i][j] = ApplyAffineToU8((last_round_ca_ptr + 4)->sub_affine_inv[i], j);
        }
    }



    last_round_ca_ptr -= swh->aff_in_round;
    for (i=0; i < piece_count; i++) {
        for (j=0; j<piece_range; j++) {
            swc->SE[piece_count + i][j] = ApplyAffineToU8(swh->se[1].sub_affine[i], j);
            swc->EE[piece_count + i][j] = ApplyAffineToU8((last_round_ca_ptr + 4)->sub_affine_inv[i], j);
        }
    }

    // *****************************
    AffineTransform * aff_ptr = swc->round_aff + 0;
    CombinedAffine * ca_ptr = swh->ca + 0;
    AffineTransform * a_ptr = swh->se[0].combined_affine_inv;
    AffineTransform * b_ptr = (ca_ptr+0)->combined_affine;
    MatGf2 temp1, temp2;
    temp1 = temp2 = NULL;

    for (i=0; i<2; i++) {
        //   B * shift_matrix * (A * x + a) + b = (B * shift_matrix * A) * x +( B * shift_matrix * a) + b
        if (i!=0) {
            aff_ptr++;
        }
        // ca_ptr;
        // a_ptr;
        b_ptr = (ca_ptr+0)->combined_affine;

        MatGf2Mul( b_ptr->linear_map, *(swh->shift_matrix + i), &temp1);
        aff_ptr->linear_map = GenMatGf2Mul( temp1, a_ptr->linear_map);
        
        MatGf2Mul( temp1, a_ptr->vector_translation, &temp2);
        aff_ptr->vector_translation = GenMatGf2Add( temp2, b_ptr->vector_translation);
        
        // DumpMatGf2(aff_ptr->linear_map );
        // DumpMatGf2(aff_ptr->vector_translation );
    }

    // *****************************
    //   B * T * (A * x + a) + b = (B * T * A) * x +( B * T * a) + b
    aff_ptr++;
    // ca_ptr;
    a_ptr = (ca_ptr+0)->combined_affine_inv;
    b_ptr = (ca_ptr+1)->combined_affine;

    MatGf2Mul( b_ptr->linear_map, swh->special_transposition, &temp1);
    aff_ptr->linear_map = GenMatGf2Mul( temp1, a_ptr->linear_map);
    
    MatGf2Mul( temp1, a_ptr->vector_translation, &temp2);
    aff_ptr->vector_translation = GenMatGf2Add( temp2, b_ptr->vector_translation);

    // *****************************
    uint8_t * round_key_ptr = swh->key_schedule + 0;
    // printf("Round 0 key xor: \t");
    piece_t* lut_ptr = swc->lut + 0;
    for (k=0; k<piece_count; k++){
        for (i=0; i<piece_range; i++) {
            uint8_t t8 = ApplyAffineToU8((ca_ptr+2)->sub_affine_inv[k], i) ^ *(round_key_ptr + k);
            t8 = double_S[t8];
            lut_ptr[k][i] = ApplyAffineToU8((ca_ptr+3)->sub_affine[k], t8);
        }
        // printf("%02X:%02X\t", *(round_key_ptr+k), lut_ptr[k][0]);
    }

    // *****************************
    //   B * T * (A * x + a) + b = (B * T * A) * x +( B * T * a) + b
    aff_ptr++;
    // ca_ptr;
    a_ptr = (ca_ptr+2)->combined_affine_inv;
    b_ptr = (ca_ptr+3)->combined_affine;

    MatGf2Mul( b_ptr->linear_map, swh->special_transposition_inv, &temp1);
    aff_ptr->linear_map = GenMatGf2Mul( temp1, a_ptr->linear_map);
    
    MatGf2Mul( temp1, a_ptr->vector_translation, &temp2);
    aff_ptr->vector_translation = GenMatGf2Add( temp2, b_ptr->vector_translation);

    // *****************************
    for (i=0; i<2; i++) {
        //   B * shift_matrix * (A * x + a) + b = (B * shift_matrix * A) * x +( B * shift_matrix * a) + b
        aff_ptr++;
        // ca_ptr;
        a_ptr = (ca_ptr+3)->combined_affine_inv;
        b_ptr = (ca_ptr+4)->combined_affine;

        MatGf2Mul( b_ptr->linear_map, *(swh->shift_matrix + 2 + i), &temp1);
        aff_ptr->linear_map = GenMatGf2Mul( temp1, a_ptr->linear_map);
        
        MatGf2Mul( temp1, a_ptr->vector_translation, &temp2);
        aff_ptr->vector_translation = GenMatGf2Add( temp2, b_ptr->vector_translation);
        
        // DumpMatGf2(aff_ptr->linear_map );
        // DumpMatGf2(aff_ptr->vector_translation );
    }

    // *****************************
    //   B * (A * x + a) + b = (B * A) * x +( B * a) + b
    aff_ptr++;
    // ca_ptr;
    a_ptr = swh->se[1].combined_affine_inv;
    b_ptr = (ca_ptr+4)->combined_affine;

    aff_ptr->linear_map = GenMatGf2Mul( b_ptr->linear_map, a_ptr->linear_map);
    
    MatGf2Mul( b_ptr->linear_map, a_ptr->vector_translation, &temp2);
    aff_ptr->vector_translation = GenMatGf2Add( temp2, b_ptr->vector_translation);
    
    // *****************************

    int round_id;

    CombinedAffine * prev_ca_ptr = NULL;
    AffineTransform * older_than_prev = swh->se[0].combined_affine_inv;
    for (round_id=1; round_id<swc->rounds; round_id++) {
        prev_ca_ptr = ca_ptr;
        ca_ptr += swh->aff_in_round;
        
        for (i=0; i<2; i++) {
            //   B * shift_matrix * (A * x + a) + b = (B * shift_matrix * A) * x +( B * shift_matrix * a) + b
            aff_ptr++;
            // ca_ptr;
            a_ptr = (prev_ca_ptr+4)->combined_affine_inv;;
            b_ptr = (ca_ptr+0)->combined_affine;

            MatGf2Mul( b_ptr->linear_map, *(swh->shift_matrix + i), &temp1);
            aff_ptr->linear_map = GenMatGf2Mul( temp1, a_ptr->linear_map);
            
            MatGf2Mul( temp1, a_ptr->vector_translation, &temp2);
            aff_ptr->vector_translation = GenMatGf2Add( temp2, b_ptr->vector_translation);
            
            // DumpMatGf2(aff_ptr->linear_map );
            // DumpMatGf2(aff_ptr->vector_translation );
        }

        // *****************************
        //   B * T * (A * x + a) + b = (B * T * A) * x +( B * T * a) + b
        aff_ptr++;
        // ca_ptr;
        a_ptr = (ca_ptr+0)->combined_affine_inv;
        b_ptr = (ca_ptr+1)->combined_affine;

        MatGf2Mul( b_ptr->linear_map, swh->special_transposition, &temp1);
        aff_ptr->linear_map = GenMatGf2Mul( temp1, a_ptr->linear_map);
        
        MatGf2Mul( temp1, a_ptr->vector_translation, &temp2);
        aff_ptr->vector_translation = GenMatGf2Add( temp2, b_ptr->vector_translation);

        // *****************************
        uint8_t * round_key_ptr = swh->key_schedule + 0;
        // printf("Round 0 key xor: \t");
        piece_t* lut_ptr = swc->lut + 0;
        for (k=0; k<piece_count; k++){
            for (i=0; i<piece_range; i++) {
                uint8_t t8 = ApplyAffineToU8((ca_ptr+2)->sub_affine_inv[k], i) ^ *(round_key_ptr + k);
                t8 = double_S[t8];
                lut_ptr[k][i] = ApplyAffineToU8((ca_ptr+3)->sub_affine[k], t8);
            }
            // printf("%02X:%02X\t", *(round_key_ptr+k), lut_ptr[k][0]);
        }

        // *****************************
        //   B * T * (A * x + a) + b = (B * T * A) * x +( B * T * a) + b
        aff_ptr++;
        // ca_ptr;
        a_ptr = (ca_ptr+2)->combined_affine_inv;
        b_ptr = (ca_ptr+3)->combined_affine;

        MatGf2Mul( b_ptr->linear_map, swh->special_transposition_inv, &temp1);
        aff_ptr->linear_map = GenMatGf2Mul( temp1, a_ptr->linear_map);
        
        MatGf2Mul( temp1, a_ptr->vector_translation, &temp2);
        aff_ptr->vector_translation = GenMatGf2Add( temp2, b_ptr->vector_translation);

        // *****************************
        for (i=0; i<2; i++) {
            //   B * shift_matrix * (A * x + a) + b = (B * shift_matrix * A) * x +( B * shift_matrix * a) + b
            aff_ptr++;
            // ca_ptr;
            a_ptr = (ca_ptr+3)->combined_affine_inv;
            b_ptr = (ca_ptr+4)->combined_affine;

            MatGf2Mul( b_ptr->linear_map, *(swh->shift_matrix + 2 + i), &temp1);
            aff_ptr->linear_map = GenMatGf2Mul( temp1, a_ptr->linear_map);
            
            MatGf2Mul( temp1, a_ptr->vector_translation, &temp2);
            aff_ptr->vector_translation = GenMatGf2Add( temp2, b_ptr->vector_translation);
            
            // DumpMatGf2(aff_ptr->linear_map );
            // DumpMatGf2(aff_ptr->vector_translation );
        }

        // *****************************
        //   B * (A * x + a) + b = (B * A) * x +( B * a) + b
        aff_ptr++;
        // ca_ptr;
        a_ptr = older_than_prev;
        b_ptr = (ca_ptr+4)->combined_affine;

        aff_ptr->linear_map = GenMatGf2Mul( b_ptr->linear_map, a_ptr->linear_map);
        
        MatGf2Mul( b_ptr->linear_map, a_ptr->vector_translation, &temp2);
        aff_ptr->vector_translation = GenMatGf2Add( temp2, b_ptr->vector_translation);
        
        older_than_prev = (prev_ca_ptr + 4)->combined_affine_inv;

    }

    // printf("aff total: %ld: %u\n", aff_ptr + 1 - swc->round_aff, swc->rounds * swc->aff_in_round );
    // printf("lut total: %ld: %u\n", lut_ptr + piece_count - swc->lut, swc->rounds * piece_count );

    MatGf2Free(temp1);
    MatGf2Free(temp2);
    temp1 = temp2 = NULL;
    
    return 0;
}


int swan_whitebox_64_content_assemble(swan_whitebox_helper* swh, swan_whitebox_content* swc)
{
    return _swan_whitebox_content_assemble(swh, swc);
}


#include <math/matrix_gf2.h>

int swan_whitebox_64_init(const uint8_t *key, int enc, swan_whitebox_content *swc) 
{
    swan_whitebox_helper* swh = (swan_whitebox_helper*)malloc(sizeof(swan_whitebox_helper));
    swan_whitebox_64_helper_init(key, swh);

    // uint32_t kf = 0x00800000;
    // printf("0x%08X\n", kf);
    // printf("0x%08X\n", ApplyMatToU32(swh->shift8, kf));
    // printf("0x%08X\n", ApplyMatToU32(swh->shift1, kf));
    // printf("0x%08X\n", ApplyMatToU32(swh->shift2, kf));

    swan_whitebox_64_content_init(swh, swc);
    swan_whitebox_64_content_assemble(swh, swc);

    
    
    // swan_whitebox_helper_release(swh);
    free(swh);
    swh = NULL;

    return 0;
}
