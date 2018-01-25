/*
 * @Author: Weijie Li 
 * @Date: 2017-11-14 16:47:24 
 * @Last Modified by: Weijie Li
 * @Last Modified time: 2017-11-24 10:17:31
 */

#ifndef MATRIXUTILS_H
#define MATRIXUTILS_H

#include <NTL/mat_GF2.h>

typedef struct affine_transform_t {
    NTL::mat_GF2 linearMap;
    NTL::vec_GF2 vectorTranslation;
} affine_transform_t;

int initMatrixFromBit(NTL::mat_GF2 &X, long *data);

int initVecFromBit(NTL::vec_GF2 &M, long data, int len);



/**
 * return a (n+1)*(n+1) matrix, contained the translation and linear map
 * and its inv
 * an affine map is the composition of two functions: a translation and a linear map
 * 
 * */
int genRandomAffineAugmentedMatrix(NTL::mat_GF2 &X,  NTL::mat_GF2 &Y, int n);

/**
 * return a n*n linear map and a n*1  translation vector
 * 
 * */
int genRandomAffineMatrix(NTL::mat_GF2 &X,  NTL::mat_GF2 &Y, NTL::vec_GF2 &V, int n);

/**
 * return a n*n linear map and a n*1  translation vector
 * and its inv
 * 
 * */
int genRandomAffineMatrix(NTL::mat_GF2 &x,  NTL::mat_GF2 &inv_x, NTL::vec_GF2 &v, NTL::vec_GF2 &inv_v, int n);

int genRandomInvMatrix(NTL::mat_GF2 &x,  NTL::mat_GF2 &inv_x, int n);



uint32_t getDigitalFromVec(NTL::vec_GF2 &s);
uint32_t get32FromVec(NTL::vec_GF2 &s);


int getVecFrom32(NTL::vec_GF2 &d, uint32_t s);

/**
 * return a 33 vector
 * 
 * */
int getAugmentedVecFrom32(NTL::vec_GF2 &d, uint32_t s);


/**
 * combine two matrix in a diag matrix
 * 
 * */
int combineDiagMat(NTL::mat_GF2 &d, NTL::mat_GF2 &s1, NTL::mat_GF2 &s2);

uint32_t applyAffineToU32(const affine_transform_t &aff, uint32_t data);
uint8_t applyAffineToU8(const affine_transform_t &aff, uint8_t data);

uint8_t applyMatToU8(NTL::mat_GF2 &mat, uint8_t data);




int genIndMatrix(NTL::mat_GF2 &mat, int size);
int genZeroVec(NTL::vec_GF2 &vec, int size);

uint32_t addVecToU32(NTL::vec_GF2 &vec, uint32_t data);
uint8_t addVecToU8(NTL::vec_GF2 &vec, uint8_t data);


#endif   /* MATRIXUTILS_H */



