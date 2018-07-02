/*
 * @Author: Weijie Li 
 * @Date: 2018-06-23 16:51:34
 * @Last Modified by: Weijie Li
 * @Last Modified time: 2018-06-27 14:45:27
 */

#include <simon_wb/simon_wb_generator.h>


#define __u8 unsigned char
#define __u32 unsigned int
#define __u64 unsigned long long int

#include <math/matrix_utils.h>

#include <stdio.h>
typedef struct simon_whitebox_helper 
{
    int aff_in_round;
    int block_size;
    int rounds;
    int piece_count; 
    CombinedAffine *ca;   // aff_in_round AFFINE for every round, rounds+1 needed
    CombinedAffine se[2]; // start encode
    // CombinedAffine ee[2]; // end encode
    MatGf2 shift8, shift1, shift2;
    uint8_t key_schedule[576];
} simon_whitebox_helper;

int _simon_whitebox_helper_init(simon_whitebox_helper *swh, int block_size, int  rounds)
{
    swh->aff_in_round = 4;
    swh->rounds = rounds;
    swh->block_size = block_size;
    swh->piece_count = block_size/2/PIECE_SIZE;
    swh->ca = (CombinedAffine *)malloc( swh->rounds * swh->aff_in_round * sizeof(CombinedAffine));
    int i,j;
    CombinedAffine * ptr = swh->ca;
    for (i=0; i < swh->rounds; i++) {
        for (j=0; j < swh->aff_in_round; j++) {
            combined_affine_init(ptr++, PIECE_SIZE, swh->piece_count);
        }
    }
    combined_affine_init(&(swh->se[0]), PIECE_SIZE, swh->piece_count);
    combined_affine_init(&(swh->se[1]), PIECE_SIZE, swh->piece_count);
    
    return 0;
}

MatGf2 make_rotate_shift(int dim, int r)
{
    MatGf2 ind = GenMatGf2(dim, dim);
    int i,j;
    for (i=0; i<dim; i++) {
        MatGf2Set(ind, i, (i+r)%dim, 1);
    }
    // DumpMatGf2(ind);
    return ind;
}

int simon_whitebox_64_helper_init(const uint8_t *key, simon_whitebox_helper *swh)
// designed for Simon_64_128
{
    swh->shift1 = make_rotate_shift(32, 1);
    swh->shift8 = make_rotate_shift(32, 8);
    swh->shift2 = make_rotate_shift(32, 2);
    SimSpk_Cipher cipher_object;
    Simon_Init(&cipher_object, cfg_128_64, ECB, key, NULL, NULL);
    memcpy(swh->key_schedule, cipher_object.key_schedule, 576);
    return _simon_whitebox_helper_init(swh, 64, 44);
}

int simon_whitebox_helper_release(simon_whitebox_helper *swh)
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
    return 0;
}

int _simon_whitebox_content_init(simon_whitebox_helper* swh, simon_whitebox_content* swc) 
{
    // TODO:
    swc->aff_in_round = 4;
    swc->rounds = swh->rounds;
    swc->block_size = swh->block_size;
    swc->piece_count = swh->piece_count;
    swc->round_aff = (AffineTransform *)malloc(swc->rounds * swc->aff_in_round  * sizeof(AffineTransform));
    swc->lut = (piece_t*)malloc(swc->rounds  * swc->piece_count * sizeof(piece_t));
    swc->and_table = (piece_t**)malloc(swc->rounds * swc->piece_count * sizeof(piece_t*));
    int i,j,k;
    j = 1<<PIECE_SIZE;
    for (i=0; i < swc->rounds; i++)
    {
        for (k=0; k<swc->piece_count; k++) {
            swc->and_table[i*swc->piece_count + k] = (piece_t*)malloc(j * sizeof(piece_t));
        }
    }
    swc->SE = (piece_t*)malloc(2 * swc->piece_count * sizeof(piece_t));
    swc->EE = (piece_t*)malloc(2 * swc->piece_count * sizeof(piece_t));
    // printf("%lu\n", sizeof(piece_t));
    // printf("%lu\n", swc->piece_count * sizeof(piece_t));
    // printf("0x%lx\n", (unsigned long)swc->lut);
    // piece_t* ptr = swc->lut+1;
    // printf("0x%lx\n", (unsigned long)ptr);
    
    return 0;
}

int simon_whitebox_64_content_init(simon_whitebox_helper* swh, simon_whitebox_content* swc) 
{
    swc->cfg = cfg_128_64;
    return _simon_whitebox_content_init(swh, swc);
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

int _simon_whitebox_content_assemble(simon_whitebox_helper* swh, simon_whitebox_content* swc)
{
    assert(PIECE_SIZE==8);
    int piece_count = swh->piece_count;
    // start encode and end encode
    int i,j,k;
    int piece_range = 1<<PIECE_SIZE;

    CombinedAffine * last_round_ca_ptr = swh->ca + (swc->rounds-1) * swc->aff_in_round;
    for (i=0; i < piece_count; i++) {
        for (j=0; j<piece_range; j++) {
            swc->SE[i][j] = ApplyAffineToU8(swh->se[0].sub_affine[i], j);
            swc->EE[i][j] = ApplyAffineToU8((last_round_ca_ptr + 3)->sub_affine_inv[i], j);
        }
    }
    last_round_ca_ptr -= swc->aff_in_round;
    for (i=0; i < piece_count; i++) {
        for (j=0; j<piece_range; j++) {
            swc->SE[piece_count + i][j] = ApplyAffineToU8(swh->se[1].sub_affine[i], j);
            swc->EE[piece_count + i][j] = ApplyAffineToU8((last_round_ca_ptr + 3)->sub_affine_inv[i], j);
        }
    }

    // *****************************
    AffineTransform * aff_ptr = swc->round_aff + 0;
    CombinedAffine * ca_ptr = swh->ca + 0;
    AffineTransform * a_ptr = swh->se[0].combined_affine_inv;
    AffineTransform * b_ptr = (ca_ptr+0)->combined_affine;
    MatGf2 temp1, temp2;
    temp1 = temp2 = NULL;

    MatGf2 shift_list[] = {swh->shift8, swh->shift1, swh->shift2};
    for (i=0; i<3; i++) {
        //   B * shift_matrix * (A * x + a) + b = (B * shift_matrix * A) * x +( B * shift_matrix * a) + b
        if (i!=0) {
            aff_ptr++;
        }
        // ca_ptr;
        // a_ptr;
        b_ptr = (ca_ptr+i)->combined_affine;

        MatGf2Mul( b_ptr->linear_map, shift_list[i], &temp1);
        aff_ptr->linear_map = GenMatGf2Mul( temp1, a_ptr->linear_map);
        
        MatGf2Mul( temp1, a_ptr->vector_translation, &temp2);
        aff_ptr->vector_translation = GenMatGf2Add( temp2, b_ptr->vector_translation);
        
        // DumpMatGf2(aff_ptr->linear_map );
        // DumpMatGf2(aff_ptr->vector_translation );
    }

    // *****************************
    piece_t* piece_ptr;
    for (k=0; k<piece_count; k++){
        piece_ptr = swc->and_table[0*piece_count + k];
        for (i=0; i<piece_range; i++) {
            for (j=0; j<piece_range; j++) {
                uint8_t t8 =    ApplyAffineToU8((ca_ptr+0)->sub_affine_inv[k], i) & \
                                    ApplyAffineToU8((ca_ptr+1)->sub_affine_inv[k], j);
                piece_ptr[i][j] =   ApplyAffineToU8((ca_ptr+2)->sub_affine[k], t8);
                // uint8_t che = ApplyAffineToU8((ca_ptr+2)->sub_affine_inv[k] , piece_ptr[i][j]);
                // if (che!=t8) {
                //     printf("failure\n");
                //     return 1;
                // }
                // if (k==0) 
                //     printf("%x ", piece_ptr[i][j] );
            }
            // if (k==0) 
            //     printf("\n");
        }
    }

    // *****************************
    //   B * (A * x + a) + b = (B * A) * x +( B * a) + b
    aff_ptr++;
    // ca_ptr;
    a_ptr = swh->se[1].combined_affine_inv;
    b_ptr = (ca_ptr+2)->combined_affine;

    aff_ptr->linear_map = GenMatGf2Mul( b_ptr->linear_map, a_ptr->linear_map);
    
    MatGf2Mul( b_ptr->linear_map, a_ptr->vector_translation, &temp2);
    aff_ptr->vector_translation = GenMatGf2Add( temp2, b_ptr->vector_translation);

    // *****************************
    uint8_t * round_key_ptr = swh->key_schedule + 0;
    // printf("Round 0 key xor: \t");
    piece_t* lut_ptr = swc->lut + 0;
    for (k=0; k<piece_count; k++){
        for (i=0; i<piece_range; i++) {
            uint8_t t8 = ApplyAffineToU8((ca_ptr+2)->sub_affine_inv[k], i) ^ *(round_key_ptr + k);
            lut_ptr[k][i] = ApplyAffineToU8((ca_ptr+3)->sub_affine[k], t8);
        }
        // printf("%02X:%02X\t", *(round_key_ptr+k), lut_ptr[k][0]);
    }
    
    // *****************************

    int round_id;

    CombinedAffine * prev_ca_ptr = NULL;
    AffineTransform * older_than_prev = swh->se[0].combined_affine_inv;
    for (round_id=1; round_id<swc->rounds; round_id++) {
        prev_ca_ptr = ca_ptr;
        ca_ptr += swh->aff_in_round;
        for (i=0; i<3; i++) {
            //   B * shift_matrix * (A * x + a) + b = (B * shift_matrix * A) * x +( B * shift_matrix * a) + b
            aff_ptr++;
            // ca_ptr;
            a_ptr = (prev_ca_ptr+3)->combined_affine_inv;
            b_ptr = (ca_ptr+i)->combined_affine;

            MatGf2Mul( b_ptr->linear_map, shift_list[i], &temp1);
            aff_ptr->linear_map = GenMatGf2Mul( temp1, a_ptr->linear_map);
            
            MatGf2Mul( temp1, a_ptr->vector_translation, &temp2);
            aff_ptr->vector_translation = GenMatGf2Add( temp2, b_ptr->vector_translation);
            
            // DumpMatGf2(aff_ptr->linear_map );
            // DumpMatGf2(aff_ptr->vector_translation );
        }

        // *****************************
        for (k=0; k<piece_count; k++){
            piece_ptr = swc->and_table[round_id * piece_count + k];
            for (i=0; i<piece_range; i++) {
                for (j=0; j<piece_range; j++) {
                    uint8_t t8 =    ApplyAffineToU8((ca_ptr+0)->sub_affine_inv[k], i) & \
                                        ApplyAffineToU8((ca_ptr+1)->sub_affine_inv[k], j);
                    piece_ptr[i][j] =   ApplyAffineToU8((ca_ptr+2)->sub_affine[k], t8);
                    // uint8_t che = ApplyAffineToU8((ca_ptr+2)->sub_affine_inv[k] , piece_ptr[i][j]);
                    // if (che!=t8) {
                    //     printf("failure\n");
                    //     return 1;
                    // }
                    // if (k==0) 
                    //     printf("%x ", piece_ptr[i][j] );
                }
                // if (k==0) 
                //     printf("\n");
            }
        }

        // *****************************
        //   B * (A * x + a) + b = (B * A) * x +( B * a) + b
        aff_ptr++;
        // ca_ptr;
        // a_ptr;
        b_ptr = (ca_ptr+2)->combined_affine;

        aff_ptr->linear_map = GenMatGf2Mul( b_ptr->linear_map, older_than_prev->linear_map);
        
        MatGf2Mul( b_ptr->linear_map, older_than_prev->vector_translation, &temp2);
        aff_ptr->vector_translation = GenMatGf2Add( temp2, b_ptr->vector_translation);

        older_than_prev = (prev_ca_ptr+3)->combined_affine_inv;

        // *****************************
        round_key_ptr = swh->key_schedule + round_id * swc->piece_count;
        lut_ptr = swc->lut + round_id * piece_count;
        for (k=0; k<piece_count; k++){
            for (i=0; i<piece_range; i++) {
                uint8_t t8 = ApplyAffineToU8((ca_ptr+2)->sub_affine_inv[k], i) ^ *(round_key_ptr + k);
                lut_ptr[k][i] = ApplyAffineToU8((ca_ptr+3)->sub_affine[k], t8);
            }
        }

    }

    // printf("aff total: %ld: %u\n", aff_ptr + 1 - swc->round_aff, swc->rounds * swc->aff_in_round );
    // printf("lut total: %ld: %u\n", lut_ptr + piece_count - swc->lut, swc->rounds * piece_count );

    MatGf2Free(temp1);
    MatGf2Free(temp2);
    temp1 = temp2 = NULL;
    
    return 0;
}

int simon_whitebox_64_content_assemble(simon_whitebox_helper* swh, simon_whitebox_content* swc)
{
    return _simon_whitebox_content_assemble(swh, swc);
}


#include <math/matrix_gf2.h>

int simon_whitebox_64_init(const uint8_t *key, int enc, simon_whitebox_content *swc) 
{
    simon_whitebox_helper* swh = (simon_whitebox_helper*)malloc(sizeof(simon_whitebox_helper));
    simon_whitebox_64_helper_init(key, swh);

    // uint32_t kf = 0x00800000;
    // printf("0x%08X\n", kf);
    // printf("0x%08X\n", ApplyMatToU32(swh->shift8, kf));
    // printf("0x%08X\n", ApplyMatToU32(swh->shift1, kf));
    // printf("0x%08X\n", ApplyMatToU32(swh->shift2, kf));

    simon_whitebox_64_content_init(swh, swc);
    simon_whitebox_64_content_assemble(swh, swc);

    
    
    simon_whitebox_helper_release(swh);
    free(swh);
    swh = NULL;

    return 0;
}

