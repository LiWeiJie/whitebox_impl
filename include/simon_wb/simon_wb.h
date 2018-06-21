#ifndef SIMON_WB_H
#define SIMON_WB_H

#include <simon/simon.h>


#ifdef __cplusplus
extern "C" {
#endif

#include <simon/simon.h>
#include <math/affine_transform.h>

#define SIMON_WHITEBOX_ROUND_MAX 68

#define SIMON_ENCRYPT 1
#define SIMON_DECRYPT 0

typedef struct simon_wb_t{
    uint32_t rounds;

    uint32_t    ssbox_enc[SIMON_WHITEBOX_ROUND_MAX][4][256];
    AffineTransform M[SIMON_WHITEBOX_ROUND_MAX][3];
    AffineTransform C[SIMON_WHITEBOX_ROUND_MAX];
    AffineTransform D[SIMON_WHITEBOX_ROUND_MAX];
    
    // start encoding
    AffineTransform SE[4];
    AffineTransform FE[4];

} SimonWhitebox;

/**
 * @brief simon whitebox encrypto function  
 * 
 * @param in both plaintext and ciphertext are ok, only accept one block: 16 uint8_t 
 * @param out the text that processed by simon_wb_ctx 
 * @param simon_wb_ctx simon_whitebox ctx, generated by int simon_wb_gen_tables(const uint8_t *key, SimonWhitebox *simon_wb_ctx, int enc) or int simon_wb_gen_tables_with_dummyrounds(const uint8_t *key, SimonWhitebox *simon_wb_ctx, int enc, int dummyrounds);
 */
void simon_wb_enc(const uint8_t *in, uint8_t *out, SimonWhitebox *simon_wb_ctx);

#define simon_wb_encrypt(in,out,simon_wb_ctx) simon_wb_enc(in, out, simon_wb_ctx);
#define simon_wb_decrypt(in,out,simon_wb_ctx)  simon_wb_encrypt(in,out,simon_wb_ctx)


/**
 * @brief free the space of simon_wb_ctx
 * 
 * @param simon_wb_ctx 
 * @return int 0 is successful, otherwise fault
 */
int simon_wb_free(SimonWhitebox *simon_wb_ctx);


typedef struct _bword_24{
  uint32_t data: 24;
} bword_24;

typedef struct _bword_48{
  uint64_t data: 48;
} bword_48;

uint8_t Simon_Init(SimSpk_Cipher *cipher_object, enum cipher_config_t cipher_cfg, enum mode_t c_mode, void *key, uint8_t *iv, uint8_t *counter);

uint8_t Simon_Encrypt(SimSpk_Cipher cipher_object, const void *plaintext, void *ciphertext);

void Simon_Encrypt_32(uint8_t round_limit, const uint8_t *key_schedule, const uint8_t *plaintext,
                      uint8_t *ciphertext);
void Simon_Encrypt_48(const uint8_t round_limit, const uint8_t *key_schedule, const uint8_t *plaintext,
                      uint8_t *ciphertext);
void Simon_Encrypt_64(const uint8_t round_limit, const uint8_t *key_schedule, const uint8_t *plaintext,
                      uint8_t *ciphertext);
void Simon_Encrypt_96(const uint8_t round_limit, const uint8_t *key_schedule, const uint8_t *plaintext,
                      uint8_t *ciphertext);
void Simon_Encrypt_128(const uint8_t round_limit, const uint8_t *key_schedule, const uint8_t *plaintext,
                       uint8_t *ciphertext);

uint8_t Simon_Decrypt(SimSpk_Cipher cipher_object, const void *ciphertext, void *plaintext);
void Simon_Decrypt_32(const uint8_t round_limit, const uint8_t *key_schedule, const uint8_t *ciphertext,
                      uint8_t *plaintext);
void Simon_Decrypt_48(const uint8_t round_limit, const uint8_t *key_schedule, const uint8_t *ciphertext,
                      uint8_t *plaintext);
void Simon_Decrypt_64(const uint8_t round_limit, const uint8_t *key_schedule, const uint8_t *ciphertext,
                      uint8_t *plaintext);
void Simon_Decrypt_96(const uint8_t round_limit, const uint8_t *key_schedule, const uint8_t *ciphertext,
                      uint8_t *plaintext);
void Simon_Decrypt_128(const uint8_t round_limit, const uint8_t *key_schedule, const uint8_t *ciphertext,
                       uint8_t *plaintext);

#endif