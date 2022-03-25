#include <stdint.h>
#include <stdio.h>
#include <memory.h>
#include "sha256.h"

uint32_t add_constant_[64] = {
    0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
    0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
    0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
    0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
    0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
    0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
    0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
    0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};

extern inline uint32_t ch(uint32_t x, uint32_t y, uint32_t z){
    return (x & y) ^ ((~x) & z);
}

extern inline uint32_t maj(uint32_t x, uint32_t y, uint32_t z){
    return (x & y) ^ (x & z) ^ (y & z);
}

extern inline uint32_t big_sigma0(uint32_t x){
    return (x >> 2 | x << 30) ^ (x >> 13 | x << 19) ^ (x >> 22 | x << 10);
}

extern inline uint32_t big_sigma1(uint32_t x){
    return (x >> 6 | x << 26) ^ (x >> 11 | x << 21) ^ (x >> 25 | x << 7);
}

extern inline uint32_t small_sigma0(uint32_t x){
    return (x >> 7 | x << 25) ^ (x >> 18 | x << 14) ^ (x >> 3);
}

extern inline uint32_t small_sigma1(uint32_t x){
    return (x >> 17 | x << 15) ^ (x >> 19 | x << 13) ^ (x >> 10);
}

void getNullPosition(const char message[], int len, FILE* output){
    //len should less than 56 byte to simplify the hash process
    uint8_t chunk[64];
    memset(chunk, 0, 64);
    memcpy(chunk, message, len);
    memset(chunk+len, 0x80, 1);
    memset(chunk+63, len*8, 1);

    fprintf(output, "chunk: %d\n", 0);
    uint32_t words [64];
    for (int i = 0; i < 16; ++i){
        words[i] = (uint32_t)(chunk[i * 4]) << 24 | (uint32_t)(chunk[i * 4 + 1]) << 16 | (uint32_t)(chunk[i * 4 + 2]) << 8 | (uint32_t)(chunk[i * 4 + 3]);
    }
    for (int i = 16; i < 64; ++i){
        words[i] = small_sigma1(words[i - 2])
                    + words[i - 7]
                    + small_sigma0(words[i - 15])
                    + words[i - 16];
    }

    uint32_t d[8] = {0};
    d[0] = 0x6a09e667;
	d[1] = 0xbb67ae85;
	d[2] = 0x3c6ef372;
	d[3] = 0xa54ff53a;
	d[4] = 0x510e527f;
	d[5] = 0x9b05688c;
	d[6] = 0x1f83d9ab;
	d[7] = 0x5be0cd19;
    for (int i = 0; i < 64; ++i){
        // for(int j = 0; j< 8; j++){
        //     *out << std::hex<< d[j] << " " <<std::dec;
        // }

        uint32_t temp1 = d[7] + big_sigma1(d[4]) + ch(d[4], d[5], d[6]) + add_constant_[i] + words[i];
        uint32_t temp2 = big_sigma0(d[0]) + maj(d[0], d[1], d[2]);
        
        d[7] = d[6];
        d[6] = d[5];
        d[5] = d[4];
        d[4] = d[3] + temp1;
        d[3] = d[2];
        d[2] = d[1];
        d[1] = d[0];
        d[0] = temp1 + temp2;
        for(int p=0; p<8; p++){
            for(int q=0; q<4; q++){
                if(!(d[p] & (0xff<<(q*8)))){
                    // std::printf("the position is at loop %d pos %d %d, the num is %x\n", i, p, q, d[p]);
                    fprintf(output, "the position is at loop %d pos %d %d , the num is 0x%x\n", i, p, q, d[p]);
                }
            }
        }
    }
}

uint8_t getNull(const char message[], int len, int chunkNum, int loop, int _p, int _q){
    //len should less than 56 byte to simplify the hash process
    uint8_t chunk[64];
    memset(chunk, 0, 64);
    memcpy(chunk, message, len);
    memset(chunk+len, 0x80, 1);
    memset(chunk+63, len*8, 1);

    uint32_t words [64];
    for (int i = 0; i < 16; ++i){
        words[i] = (uint32_t)(chunk[i * 4]) << 24 | (uint32_t)(chunk[i * 4 + 1]) << 16 | (uint32_t)(chunk[i * 4 + 2]) << 8 | (uint32_t)(chunk[i * 4 + 3]);
    }
    for (int i = 16; i < 64; ++i){
        words[i] = small_sigma1(words[i - 2])
                    + words[i - 7]
                    + small_sigma0(words[i - 15])
                    + words[i - 16];
    }

    uint32_t d[8] = {0};
    d[0] = 0x6a09e667;
	d[1] = 0xbb67ae85;
	d[2] = 0x3c6ef372;
	d[3] = 0xa54ff53a;
	d[4] = 0x510e527f;
	d[5] = 0x9b05688c;
	d[6] = 0x1f83d9ab;
	d[7] = 0x5be0cd19;
    for (int i = 0; i <=loop; ++i){
        uint32_t temp1 = d[7] + big_sigma1(d[4]) + ch(d[4], d[5], d[6]) + add_constant_[i] + words[i];
        uint32_t temp2 = big_sigma0(d[0]) + maj(d[0], d[1], d[2]);
        
        d[7] = d[6];
        d[6] = d[5];
        d[5] = d[4];
        d[4] = d[3] + temp1;
        d[3] = d[2];
        d[2] = d[1];
        d[1] = d[0];
        d[0] = temp1 + temp2;
    }
    printf("%d", (d[_p]>>(_q*8)) & 0xff);
    return (d[_p]>>(_q*8)) & 0xff;
}

