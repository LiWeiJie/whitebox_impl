#include<stdio.h>
#include<stdlib.h>
#include <cstring>
#include <stdint.h>
#define TEST 50000000
#define TIMES 100
#define SIZE 1024
#define __u64 long long
#define __u32 unsigned int
#define __u8 unsigned char
#define ROUND 1
#define KEYSIZE 128
#define HALFTEXT 2
#define R11 2
#define R12 5
#define R21 8
#define R22 14
static __u64 start_rdtsc(){
    uint32_t cycles_high, cycles_low;
    asm volatile (
    "CPUID\n\t"
    "RDTSC\n\t"
    "mov %%edx, %0\n\t"
    "mov %%eax, %1\n\t": "=r" (cycles_high), "=r"
    (cycles_low):: "%rax", "%rbx", "%rcx", "%rdx");
    return ((__u64)cycles_low) | (((__u64)cycles_high) << 32);
}


 static __u64 end_rdtsc(){
    uint32_t cycles_high, cycles_low;
    asm volatile (
    "RDTSCP\n\t"
    "mov %%edx, %0\n\t"
    "mov %%eax, %1\n\t"
    "CPUID\n\t": "=r" (cycles_high), "=r"
    (cycles_low):: "%rax", "%rbx", "%rcx", "%rdx");
    return ((__u64)cycles_low) | (((__u64)cycles_high) << 32);
}


static __u32 Gamma(__u32 x)
{
    __u8 t[4], r[4];
    __u8 * a = (__u8 *)&x;
    t[3] = ~(a[3]|a[2]);
    r[3] = a[0] ^ t[3];
    t[2] = ~(a[2]|a[1]);
    r[2] = a[3] ^ t[2];
    t[1] = ~(a[1] | r[3]);
    r[1] = ~(a[2] ^ t[1]);
    t[0] = ~(r[3] | r[2]);
    r[0] = a[1] ^ t[0];
    __u32 *ret = (__u32 *) r;
    return *ret;
}

#define ROL(x, n) ((x >> n) | (x << (32-n)))

__u32 R1(__u32 x)
{
    return ROL(x, 2) ^ ROL(x, 32-5);
}

__u32 R2(__u32 x)
{
    return ROL(x, 8) ^ ROL(x, 32-14);
}

void SWAN_Encrypt_64(char *plaintext, char *key, char *ciphertext)
{
    printf("gamma 0:%08X\n", Gamma(0xFFFFFFFF));
    __u32 L = *((__u32*) plaintext);
    __u32 R = *((__u32*) (plaintext+HALFTEXT));
    __u32 key1 = *((__u32*) key);
    __u32 key2 = *((__u32*) (key+HALFTEXT));
    for(int i=0; i<ROUND; i++)
    {
        __u32 tmp = R2(Gamma(R1(L))^key1) ^ R;
        R = R2(Gamma(R1(tmp))^key2) ^ L;
        L = tmp;
//        printf("%08X %08X\n", L, R);
    }
    for(int i=0; i<4; i++)
    {
        ciphertext[i] = (L >> (i*4))&0xFF;
        ciphertext[i+4] = ( R >> (i*4))& 0xFF;
    }
}

int SWAN_Encrypt_64_main()
{
    __u64 begin;
    __u64 end;

    char p[2][8] = {{0, 1, 2, 3, 0,1,2,3},{0}};
    char key[4] = {4,5,6,7};
    bool flag = 0;
    __u64 ans = 0;
    begin = start_rdtsc();

    for(int i=0; i<TEST; i++)
    {
        SWAN_Encrypt_64(p[flag], key, p[!flag]);
        flag = !flag;
    }
    end = end_rdtsc();
    ans += (end - begin);

    for(int i=0; i<8; i++)
    {
        printf("%02X", p[flag][i]&0XFF);
    }
    printf("\n");
    printf("pre SWAN cost %llu CPU cycles\n",(ans)/TEST);
    return 0;
    return 0;
}
