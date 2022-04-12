#include <bits/stdint-uintn.h>
#include <stdint.h>
#include <stdio.h>
#include <memory.h>
#include <sys/time.h>
#include <stdlib.h>
#include <math.h>


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

int gcd(int a, int h)
{
    int temp;
    while(1)
    {
        temp = a%h;
        if(temp==0)
        return h;
        a = h;
        h = temp;
    }
}


void heavyCalc(uint64_t input){
    for(int i; i<100; ++i){
        double p = 3;
        double q = 7;
        double n=p*q;
        double count;
        double totient = (p-1)*(q-1);
    
        //public key
        //e stands for encrypt
        double e=2;
    
        //for checking co-prime which satisfies e>1
        while(e<totient){
        count = gcd(e,totient);
        if(count==1)
            break;
        else
            e++;
        }
    
        //private key
        //d stands for decrypt
        double d;
    
        //k can be any arbitrary value
        double k = 2;
    
        //choosing d such that it satisfies d*e = 1 + k * totient
        d = (1 + (k*totient))/e;
        double msg = (double)input;
        double c = pow(msg,e);
        double m = pow(c,d);
        c=fmod(c,n);
        m=fmod(m,n);
    }
}