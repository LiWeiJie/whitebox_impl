/*
 * @Author: Weijie Li 
 * @Date: 2017-11-27 16:44:54 
 * @Last Modified by: Weijie Li
 * @Last Modified time: 2018-06-27 21:41:55
 */

#include <math/affine_transform.h>


int GenRandomAffineTransform(AffineTransform *at, AffineTransform *at_inv, int dim) {
    at->linear_map = GenMatGf2(dim, dim);
    at->vector_translation = GenMatGf2(dim, 1);

    at_inv->linear_map = GenMatGf2(dim, dim);
    at_inv->vector_translation = GenMatGf2(dim, 1);

    RandomMatGf2(at->linear_map);
    while (IsMatGf2Invertible(at->linear_map)==0) {
        RandomMatGf2(at->linear_map);
    }    
    RandomMatGf2(at->vector_translation);

    MatGf2Inv(at->linear_map, &(at_inv->linear_map));
    MatGf2Mul(at_inv->linear_map, at->vector_translation, &(at_inv->vector_translation));

    return 0;
}

int GenIndAffineTransform(AffineTransform *at, AffineTransform *at_inv, int dim) {
    at->linear_map = GenIndMatrix(dim);
    at->vector_translation = GenMatGf2(dim, 1);

    at_inv->linear_map = GenIndMatrix(dim);
    at_inv->vector_translation = GenMatGf2(dim, 1);
    return 0;
}


int ApplyAffineTransform(const AffineTransform at, const MatGf2 mat, MatGf2* dst) {
    // MatGf2 to_be_free = *dst;
    MatGf2Mul(at.linear_map, mat, dst);
    MatGf2Add(at.vector_translation, *dst, dst);
    // MatGf2Free(to_be_free);
    return 0;
}

uint32_t ApplyAffineToU32(const AffineTransform aff, uint32_t x) {
    MatGf2 mat_x = NULL;
    ReAllocatedMatGf2(32, 1, &mat_x);
    InitVecFromBit(x, mat_x);
    MatGf2Add( MatGf2Mul(aff.linear_map, mat_x, &mat_x), aff.vector_translation, &mat_x);
    uint32_t result = get32FromVec(mat_x);
    MatGf2Free(mat_x);
    return result;
}

uint16_t ApplyAffineToU16(const AffineTransform aff, uint16_t x) {
    MatGf2 mat_x = NULL;
    ReAllocatedMatGf2(16, 1, &mat_x);
    InitVecFromBit(x, mat_x);
    MatGf2Add( MatGf2Mul(aff.linear_map, mat_x, &mat_x), aff.vector_translation, &mat_x);
    uint16_t result =  (uint16_t)getDigitalFromVec(mat_x);
    MatGf2Free(mat_x);
    return result;
}

uint8_t ApplyAffineToU8(const AffineTransform aff, uint8_t data) {
    MatGf2 mat_x = NULL;
    ReAllocatedMatGf2(8, 1, &mat_x);
	InitVecFromBit(data, mat_x);
    MatGf2Add( MatGf2Mul(aff.linear_map, mat_x, &mat_x), aff.vector_translation, &mat_x);
    uint8_t result = (uint8_t)getDigitalFromVec(mat_x);
    MatGf2Free(mat_x);
    return result;
}

int AffineTransformFree(AffineTransform *aff) {
    MatGf2Free(aff->linear_map);
    aff->linear_map = NULL;
    MatGf2Free(aff->vector_translation);
    aff->vector_translation = NULL;
    return 0;
}

void * ExportAffineToStr(const AffineTransform* aff) {
    void *a = ExportMatGf2ToStr(aff->linear_map);
    void *b = ExportMatGf2ToStr(aff->vector_translation);
    uint32_t * sz = a;
    int sza = *sz;
    sz ++;
    sza = sza * (*sz);

    sz = b;
    int szb = *sz;
    sz ++;
    szb = szb * (*sz);

    void * result = malloc(4*sizeof(uint32_t) + sza + szb);
    memcpy(result, a, sza + 2*sizeof(uint32_t));
    memcpy(result + sza*sizeof(char) + 2*sizeof(uint32_t),  b,  szb+ 2*sizeof(uint32_t));

    free(a);
    free(b);

    return result;
}

AffineTransform ImportAffineFromStr(const void *source) {
    AffineTransform result;
    result.linear_map = ImportMatGf2FromStr(source);

    uint32_t * sz = (uint32_t *)source;
    int sza = *sz;
    sz ++;
    sza = sza * (*sz);

    result.vector_translation = ImportMatGf2FromStr(source + sza + 2*sizeof(uint32_t));
    return result;
}