#ifndef _SWAN_WB_H_
#define _SWAN_WB_H_

#include <swan/swan.h>
#include <math/affine_transform.h>

#ifdef __cplusplus
extern "C" {
#endif

struct swan_whitebox_content {
    int type;
    int block_size;
    int rounds;
    int piece_count;
    AffineTransform ** round_aff;
    AffineTransform *final_aff;
    unsigned int **lut;
} swan_whitebox_content;

int swan_whitebox_init(int rounds, int block_size, swan_whitebox_content* swc);

int swan_whitebox_release(swan_whitebox_content *swc);


#ifdef __cplusplus
}
#endif

#endif //_SWAN_WB_H_
