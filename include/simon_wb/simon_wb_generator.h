/*
 * @Author: Weijie Li 
 * @Date: 2018-06-21 16:51:34
 * @Last Modified by: Weijie Li
 * @Last Modified time: 2018-06-23 15:35:16
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
int simon_whitebox_64_init(const uint8_t *key, int enc, simon_whitebox_content *swc);

int simon_whitebox_release(simon_whitebox_content *swc);

#ifdef __cplusplus
}
#endif
#endif //_SIMON_WHITEBOX_GENERATOR_H_