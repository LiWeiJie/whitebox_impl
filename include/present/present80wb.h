/*
 * @Author: Weijie Li 
 * @Date: 2018-01-23 23:35:52 
 * @Last Modified by: Weijie Li
 * @Last Modified time: 2018-01-24 10:48:34
 */
/*
 * Copyright (c) 2009, Shanghai Jiao Tong University
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * - Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in
 *   the documentation and/or other materials provided with the
 *   distribution.
 * - Neither the name of the Shanghai Jiao Tong University nor the
 *   names of its contributors may be used to endorse or promote
 *   products derived from this software without specific prior
 *   written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * The core function of PRESENT block cipher.
 * 1kB look-up tables are used for perferment tunning.
 *
 * @author Bo Zhu, http://cis.sjtu.edu.cn/index.php/Bo_Zhu
 * @author Zheng Gong, DIES Group, University of Twente
 * @date   July 21, 2009
 */

#ifndef __PRESENT80WB_H__
#define __PRESENT80WB_H__

#ifdef __cplusplus 
extern "C" { 
#endif

#include <NTL/mat_GF2.h>

// comment this out if this is used on PC
//#define __UINT_T__

#ifndef __UINT_T__
#define __UINT_T__
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long uint32_t;
typedef unsigned long long uint64_t;
#endif /* __UINT_T__ */

#define PRESENT_ROUNDS 32

typedef struct _present_wb_ctx {
    uint8_t rk[PRESENT_ROUNDS+1][8][256];
    uint8_t stmp[8][256]; // start map
    uint8_t secrect_sbox_pmt_3[PRESENT_ROUNDS][8][256];
    uint8_t secrect_sbox_pmt_2[PRESENT_ROUNDS][8][256];
    uint8_t secrect_sbox_pmt_1[PRESENT_ROUNDS][8][256];
    uint8_t secrect_sbox_pmt_0[PRESENT_ROUNDS][8][256];
    uint8_t rounds;
} present_wb_ctx;

// // full-round PRESENT block cipher
// #define present_wb(plain, key, cipher) present_wb_rounds((plain), (key), 31, (cipher))
// // full-round PRESENT whitebox block cipher init
// #define present_wb_init(key, ctx) present_wb_init_rounds((key), 31, (ctx))

// full-round should be 31, i.e. rounds = 31
void present_wb_init(const uint8_t *key, present_wb_ctx ctx);

// recycling memory space
void present_wb_release(present_wb_ctx ctx);

// plain and cipher can overlap, so do key and cipher
void present_wb(const uint8_t *plain, const present_wb_ctx ctx, uint8_t *cipher);

#ifdef __cplusplus 
}
#endif

#endif /* __PRESENT80WB_H__ */
