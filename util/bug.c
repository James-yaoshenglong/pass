#include <stdint.h>
#include <stdio.h>
#include <memory.h>
#include <sys/time.h>
#include <stdlib.h>


void useless(){
    return;
}

typedef struct{
    uint64_t buffer[254]; //use long to keep alignment
    void (*fun_ptr)(void);
} Bug;


void bof(int num){
    Bug target;
    uint64_t source[256] = {0};
    uint8_t size = num+(uint8_t)255;
    printf("size: %d\n", size);
    memcpy(target.buffer, source, size*sizeof(uint64_t));
    target.fun_ptr();
}

void uaf(int num){
    Bug* target = (Bug*)malloc(256*sizeof(uint64_t));
    uint64_t source[256] = {0};
    uint8_t size = num+(uint8_t)255;
    printf("size: %d\n", size);
    memcpy(target->buffer, source, size*sizeof(uint64_t));
    free(target);
    target->fun_ptr();
}

void markBBL(int a){
    return;
}

void timestamp(){
    long long time = 0;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    time = tv.tv_sec*1000000+tv.tv_usec;
    // printf("start %lld", time);
    FILE* f = NULL;
    f = fopen("../data/timestamp.txt", "r");
    char buffer [256];
    if(f){
        fgets(buffer, 255, f); //don't know why fscanf not work here
        time = atoll(buffer);
        // printf("the number is %lld\n", time);
        fclose(f);
        if(time == 0){
            f = fopen("../data/timestamp.txt", "w");
            struct timeval tv;
            gettimeofday(&tv, NULL);
            time = tv.tv_sec*1000000+tv.tv_usec;
            fprintf(f, "%lld", time);
            fclose(f);
        }
    }
    gettimeofday(&tv, NULL);
    time = tv.tv_sec*1000000+tv.tv_usec;
    // printf("end   %lld\n", time);
}