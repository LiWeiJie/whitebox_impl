#include <swan_wb/swan_wb.h>

unsigned char S[16] = {0x0e, 0x04, 0x0b, 0x02, 0x03, 0x08, 0x00, 0x09, 0x01, 0x0a, 0x07, 0x0f, 0x06, 0x0c, 0x05, 0x0d};

#define __u8 unsigned char
#define __u32 unsigned int
#define __u64 unsigned long long int

#include <math/matrix_utils.h>

typedef struct swan_whitebox_helper 
{
    int block_size;
    int rounds;
    int piece_count; 
    CombinedAffine *ca;
}swan_whitebox_helper;

int swan_whitebox_helper_init(swan_whitebox_helper* swh, int rounds, int block_size)
{
    swh->rounds = rounds;
    swh->block_size = block_size;
    swh->piece_count = block_size/8;
    swh->ca = malloc((rounds+1) * 4 * sizeof(CombinedAffine));
    memset(swh->ca, 0, (rounds+1) * 4 * sizeof(CombinedAffine));
    int i,j;
    CombinedAffine *ptr = swh->ca;
    int aff_rounds = rounds + 1;
    for (i=0; i<aff_rounds; i++) {
        for (j=0; j<4; j++) {
            combined_affine_init(ptr++, 8, swh->piece_count);
        }
    }
    return 0;
}

int swan_whitebox_helper_release(swan_whitebox_helper* swh)
{
    int aff_rounds = swh->rounds + 1;
    CombinedAffine *ptr = swh->ca;
    int i,j;
    for (i=0; i<aff_rounds; i++) {
        for (j=0; j<4; j++) {
            combined_affine_free(ptr++);
        }
    }
    return 0;
}

int swan_whitebox_content_init(swan_whitebox_helper* swh, swan_whitebox_content* swc)
{
    // TODO:
    swc->rounds = swh->rounds;
    swc->block_size = swh->block_size;
    swc->piece_count = swh->piece_count;
    swc->round_aff = malloc(swc->rounds * sizeof(AffineTransform *));

    return 0;
}

int swan_whitebox_init(int rounds, int block_size, swan_whitebox_content* swc) 
{
    swan_whitebox_helper* swh;
    swan_whitebox_helper_init(swh, rounds, block_size);
    
    
    
    swan_whitebox_helper_release(swh);
    free(swh);
    swh = NULL;
    return 0;
}

int swan_whitebox_release(swan_whitebox_content *swc) 
// TODO: tbc
{
    return 0;
}
