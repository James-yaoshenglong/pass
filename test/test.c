#include <stdio.h>
#include <sys/types.h>
#include <time.h>



// void print(int a){
//     struct timespec point;
//     clock_gettime(CLOCK_MONOTONIC_RAW, &point);
//     // long us = point.tv_sec * 1000000 + point.tv_nsec / 1000+a;
//     long us = a;
//     printf("%ld\n", us);
// }


void doSomething(int a){
    if(a > 0){
        printf("%d\n",a);
    }
    else{
        printf("jjj");
    }
}

void idJump(int a){
    union { long int address ; void (*func)(int) ; }  x ; 
    
    // Set the "address" to an arbitrary address in memory: 
    
    x . address = (long)doSomething + a ; 
    
    
    x . func (a) ;  //  Jump to that address! 
}


int main(){
    int a;
    scanf("%d", &a);
    doSomething(a);
    if(a==0){
        printf("hhh");
    }
    // idJump(a);
    int b = a+1;
    printf("%d", b);
    printf("hello world");
    return 0;
}