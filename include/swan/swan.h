#ifndef _SWAN_H_
#define _SWAN_H_

void SWAN_Encrypt_64(char *plaintext, char *key, char *ciphertext);
void SWAN_Encrypt_128(char *plaintext, char *key, char *ciphertext);

int SWAN_Encrypt_64_main();
int SWAN_Encrypt_128_main();

#endif //_SWAN_H_
