/*
 * @Author: Weijie Li 
 * @Date: 2018-07-01 19:47:36
 * @Last Modified by: Weijie Li
 * @Last Modified time: 2018-07-01 19:47:32
 */


#ifndef  _SWAN_WHITEBOX_GENERATOR_H_
#define  _SWAN_WHITEBOX_GENERATOR_H_

#include <stdint.h>
#include <swan_wb/swan_wb.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief generate SimonWhitebox instance  
 * 
 * @param key encrypt key
 * @param SWAN_wb_ctx a pointer to an instance of SWAN_wb_ctx
 * @param enc encrypto mode. {SWAN_ENCRYPT: encrypto mode; SWAN_DECRYPT: decrypto mode}
 * @return int 0 is successful, otherwise fault
 */
int swan_whitebox_64_init(const uint8_t *key, int enc, swan_whitebox_content *swc);


#ifdef __cplusplus
}
#endif

#endif //_SWAN_WHITEBOX_GENERATOR_H_