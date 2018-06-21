/*
 * @Author: Weijie Li 
 * @Date: 2018-06-21 22:50:06
 * @Last Modified by: Weijie Li
 * @Last Modified time: 2018-06-21 22:50:11
 */

#include <math/matrix_utils.h>
#include <assert.h>

int combined_affine_init(CombinedAffine cm, int sub_matrix_size, int sub_matrix_number) {
    assert(sub_matrix_number >= 2);
    cm->number = sub_matrix_number;
    cm->sub_affine = malloc(sub_matrix_number * sizeof(AffineTransform));
    cm->sub_affine_inv = malloc(sub_matrix_number * sizeof(AffineTransform));
    cm->combined_affine = malloc(sizeof(AffineTransform));
    cm->combined_affine_inv = malloc(sizeof(AffineTransform));
    AffineTransform * aff_ptr = cm->sub_affine;
    AffineTransform * aff_ptr_inv = cm->sub_affine_inv;

    MatGf2 T,S;
    T = S = NULL;
    int counter = sub_matrix_number;
    while(counter--) {
        GenRandomAffineTransform(aff_ptr, aff_ptr_inv, sub_matrix_size);
        DumpMatGf2(aff_ptr->linear_map);
        DumpMatGf2(aff_ptr_inv->linear_map);
        aff_ptr++;
        aff_ptr_inv++;
    }
    counter = sub_matrix_number;
    aff_ptr = cm->sub_affine;
    aff_ptr_inv = cm->sub_affine_inv;

    cm->combined_affine->linear_map = GenMatGf2Diag((aff_ptr)->linear_map, (aff_ptr+1)->linear_map);
    aff_ptr += 2;

    cm->combined_affine_inv->linear_map = GenMatGf2Diag((aff_ptr_inv)->linear_map, (aff_ptr_inv+1)->linear_map);
    aff_ptr_inv += 2;

    counter--;
    while(--counter) {
        T = GenMatGf2Diag(cm->combined_affine->linear_map, (aff_ptr++)->linear_map);
        S = cm->combined_affine->linear_map;
        cm->combined_affine->linear_map = T;
        MatGf2Free(S);
        S = T = NULL;

        T = GenMatGf2Diag(cm->combined_affine_inv->linear_map, (aff_ptr_inv++)->linear_map);
        S = cm->combined_affine_inv->linear_map;
        cm->combined_affine_inv->linear_map = T;
        MatGf2Free(S);
        S = T = NULL;
    }
    DumpMatGf2(cm->combined_affine->linear_map);
    DumpMatGf2(cm->combined_affine_inv->linear_map);
    
    int total_dim = sub_matrix_size*sub_matrix_number;
    cm->combined_affine->vector_translation = GenMatGf2(total_dim, 1);
    cm->combined_affine_inv->vector_translation = GenMatGf2(total_dim, 1);
    aff_ptr = cm->sub_affine;
    aff_ptr_inv = cm->sub_affine_inv;
    int j = 0;
    while (j<total_dim) {
        int i;
        for (i=0; i<sub_matrix_size; i++,j++) {
            MatGf2Set(cm->combined_affine->vector_translation, j, 0, MatGf2Get((aff_ptr)->vector_translation, i, 0));
            MatGf2Set(cm->combined_affine_inv->vector_translation, j, 0, MatGf2Get((aff_ptr_inv)->vector_translation, i, 0));
        }
        aff_ptr++;
        aff_ptr_inv++;
    }
    DumpMatGf2(cm->combined_affine->vector_translation);
    DumpMatGf2(cm->combined_affine_inv->vector_translation);
    return 0;
}

int combined_affine_free(CombinedAffine cm) {
    // TODO:
    return 0;
}
