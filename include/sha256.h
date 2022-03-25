#ifndef SHA256_H
#define SHA256_H

#include <stdio.h>
#include <stdint.h>

void getNullPosition(const char message[], int len, FILE* output);

uint8_t getNull(const char message[], int len, int chunkNum, int loop, int _p, int _q);

#endif // SHA256_H