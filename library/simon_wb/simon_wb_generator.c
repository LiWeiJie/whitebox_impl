// 

#include <simon_wb/simon_wb_generator.h>


#define __u8 unsigned char
#define __u32 unsigned int
#define __u64 unsigned long long int

#include <math/matrix_utils.h>

typedef struct simon_whitebox_helper 
{
    int aff_in_round;
    int block_size;
    int rounds;
    int piece_count; 
    CombinedAffine *ca;   // 4 AFFINE for every round, rounds+1 needed
    CombinedAffine se; // start encode
    CombinedAffine ee; // end encode
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
    combined_affine_init(&(swh->se), PIECE_SIZE, swh->piece_count);
    combined_affine_init(&(swh->ee), PIECE_SIZE, swh->piece_count);
    return 0;
}


int simon_whitebox_64_helper_init(simon_whitebox_helper *swh)
// designed for Simon_64_128
{
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
    combined_affine_free(&swh->se);
    combined_affine_free(&swh->ee);
    return 0;
}

#include <stdio.h>

int simon_whitebox_64_content_init(simon_whitebox_helper* swh, simon_whitebox_content* swc) 
{
    // TODO:
    swc->aff_in_round = 4;
    swc->rounds = swh->rounds;
    swc->block_size = swh->block_size;
    swc->piece_count = swh->piece_count;
    swc->round_aff = (AffineTransform *)malloc(swc->rounds * swc->aff_in_round  * sizeof(AffineTransform));
    swc->lut = (piece_t*)malloc(swc->rounds  * swc->piece_count * sizeof(piece_t));
    swc->and_table = (piece_t**)malloc(swc->rounds * sizeof(piece_t*));
    int i,j;
    j = 1<<PIECE_SIZE;
    for (i=0; i < swc->rounds; i++)
    {
        swc->and_table[i] = (piece_t*)malloc(j * sizeof(piece_t));
    }
    swc->SE = (piece_t*)malloc(swc->piece_count * sizeof(piece_t));
    swc->EE = (piece_t*)malloc(swc->piece_count * sizeof(piece_t));
    // printf("%lu\n", sizeof(piece_t));
    // printf("%lu\n", swc->piece_count * sizeof(piece_t));
    // printf("0x%lx\n", (unsigned long)swc->lut);
    // piece_t* ptr = swc->lut+1;
    // printf("0x%lx\n", (unsigned long)ptr);
    
    return 0;
}

int simon_whitebox_release(simon_whitebox_content *swc)
{
    // TODO:
    // AffineTransformFree(swc->)

    //free memory
    free(swc->round_aff);
    free(swc->lut);
    int i,j;
    j = 1<<PIECE_SIZE;
    for (i=0; i < swc->rounds; i++)
    {
        free(swc->and_table[i]);
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



int simon_whitebox_64_init(const uint8_t *key, int enc, simon_whitebox_content *swc) 
{
    simon_whitebox_helper* swh = (simon_whitebox_helper*)malloc(sizeof(simon_whitebox_helper));
    simon_whitebox_64_helper_init(swh);

    simon_whitebox_64_content_init(swh, swc);

    
    
    simon_whitebox_helper_release(swh);
    free(swh);
    swh = NULL;

    return 0;
}

