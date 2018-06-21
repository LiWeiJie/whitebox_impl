/*
 * @Author: Weijie Li 
 * @Date: 2018-06-21 16:51:34
 * @Last Modified by: Weijie Li
 * @Last Modified time: 2017-12-27 10:39:04
 */


#ifndef  _SIMON_WHITEBOX_GENERATOR_H_
#define  _SIMON_WHITEBOX_GENERATOR_H_

#include <stdint.h>
#include <simon_wb/simon_wb.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief generate SimonWhitebox instance  
 * 
 * @param key encrypt key
 * @param SIMON_wb_ctx a pointer to an instance of SIMON_wb_ctx
 * @param enc encrypto mode. {SIMON_ENCRYPT: encrypto mode; SIMON_DECRYPT: decrypto mode}
 * @return int 0 is successful, otherwise fault
 */
int simon_wb_gen_tables(const uint8_t *key, SimonWhitebox *simon_wb_ctx, int enc);


#ifdef __cplusplus
}
#endif
#endif //_SIMON_WHITEBOX_GENERATOR_H_