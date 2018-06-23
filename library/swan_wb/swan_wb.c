#include <swan_wb/swan_wb.h>

S[16] = {0x0e, 0x04, 0x0b, 0x02, 0x03 0x08, 0x00, 0x09, 0x01, 0x0a, 0x07, 0x0f, 0x06, 0x0c, 0x05, 0x0d};

#include <math/matrix_utils.h>

struct swan_whitebox_helper 
{
    int block_size;
    int rounds;
    int piece_count; 
}

int swan_whitebox_init(int rounds, int block_size, swan_whitebox_content* swc) 
{
    swc->rounds = rounds;
    swc->block_size = block_size;
    swc->piece_count = block_size/8;
    swc->round_aff = malloc(rounds * sizeof(AffineTransform *));
    

}

int swan_whitebox_release(swan_whitebox_content *swc) 
// TODO: tbc
{

}
