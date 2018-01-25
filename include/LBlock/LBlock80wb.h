/*************************************
@berif  : this file is the implemention of Lblock in the way of 4-bit S-box. 
		  It is writed to test the speed of LBlock80.
@author : Shusheng Liu
@time   : 11.4 2011
@Copyright (c) 2011, south china normal university
**************************************/

#ifndef _LBLOCK80WB_H_
#define _LBLOCK80WB_H_

#include "LblocSbox.h"
#include <cstdint>

#define LBLOCK80_ROUNDS 32

typedef struct LBlock80wb_ctx {
	int rounds ;
	uint8_t Te[LBLOCK80_ROUNDS][8][16];
	uint8_t remap[LBLOCK80_ROUNDS][8][16];
	uint8_t fi[2][8][16];
	uint8_t g [8][16];
} LBlock80wb_ctx;

int gen_LBlock80_wb_ctx(LBlock80wb_ctx *ctx, const uint8_t *key);

void LBlock80wb_encrypte_algorithm(const uint8_t *plain, const LBlock80wb_ctx *ctx, uint8_t* cipher);

#endif //_LBLOCK80WB_H_