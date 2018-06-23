// designed for Simon_64_128

#include <simon_wb/simon_wb_generator.h>


#define __u8 unsigned char
#define __u32 unsigned int
#define __u64 unsigned long long int

#include <math/matrix_utils.h>

typedef struct simon_whitebox_helper 
{
    int block_size;
    int rounds;
    int piece_count; 
    CombinedAffine *ca;
} simon_whitebox_helper;

int simon_whitebox_64_helper_init(simon_whitebox_helper *swh)
{
    swh->rounds = 44;
    swh->block_size = 64;
    swh->piece_count = 32/8;
    swh->ca = (CombinedAffine *)malloc( swh->rounds * 5 * sizeof(CombinedAffine));
    int i,j;
    CombinedAffine * ptr = swh->ca;
    for (i=0; i < swh->rounds; i++) {
        for (j=0; j < 5; j++) {
             combined_affine_init(ptr++, 8, swh->piece_count);
        }
    }
    return 0;
}

int simon_whitebox_64_helper_release(simon_whitebox_helper *swh)
{
    int i,j;
    CombinedAffine * ptr = swh->ca;
    for (i=0; i < swh->rounds; i++) {
        for (j=0; j < 5; j++) {
             combined_affine_free(ptr++);
        }
    }
    free(swh->ca);
    swh->ca = NULL;
    return 0;
}

int simon_whitebox_64_content_init(simon_whitebox_helper* swh, simon_whitebox_content* swc) 
{
    // TODO:
    swc->rounds = swh->rounds;
    swc->block_size = swh->block_size;
    swc->piece_count = swh->piece_count;
    // swc->round_aff = malloc(swc->rounds * sizeof(AffineTransform *));

    return 0;
}



int simon_whitebox_64_init(const uint8_t *key, int enc, simon_whitebox_content *swc) 
{
    simon_whitebox_helper* swh = (simon_whitebox_helper*)malloc(sizeof(simon_whitebox_helper));
    simon_whitebox_64_helper_init(swh);

    simon_whitebox_64_content_init(swh, swc);

    
    
    simon_whitebox_64_helper_release(swh);
    free(swh);
    swh = NULL;

    return 0;
}

