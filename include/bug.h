#ifndef BUG_H
#define BUG_H

#include <stdint.h>

const int STACK_SHIFT = 5*8;


//pay attention to the intel syntax or AT&T syntax
// const char* bofRegAsm = "push %rdi\n\t" //here use ; or \n\t to sepetate lines in inline assembly, pay attention to the multiple line string writen
//                 "push %rsi\n\t"
//                 "push %rdx\n\t"
//                 "push %rcx\n\t"
//                 "push %rax\n\t"
//                 "call timestamp\n\t"
//                 "push %%%s\n\t"
//                 "mov %rsp, %rdi\n\t"
//                 "mov %d, %rsi\n\t"
//                 "mov %d, %rdx\n\t"
//                 "mov %d, %rcx\n\t"
//                 "mov %d, %r8\n\t"
//                 "mov %d, %r9\n\t"
//                 "call getNull\n\t"
//                 "pop %rdi\n\t"
//                 "mov %rax, %rdi\n\t"
//                 "call bof\n\t"
//                 "pop %rax\n\t"
//                 "pop %rcx\n\t"
//                 "pop %rdx\n\t"
//                 "pop %rsi\n\t"
//                 "pop %rdi\n\t";

//use intel syntax to solve the bug
const char* bofRegAsm = "push rdi\n\t" //here use ; or \n\t to sepetate lines in inline assembly, pay attention to the multiple line string writen
                "push rsi\n\t"
                "push rdx\n\t"
                "push rcx\n\t"
                "push rax\n\t"
                "push rbx\n\t"
                "push rbp\n\t"
                "push r8\n\t"
                "push r9\n\t"
                "push r10\n\t"
                "push r11\n\t"
                "push r12\n\t"
                "push r13\n\t"
                "push r14\n\t"
                "push r15\n\t"
                "call timestamp\n\t"
                "push %s\n\t"
                "mov rdi, rsp\n\t"
                "mov rsi, %d\n\t"
                "mov rdx, %d\n\t"
                "mov rcx, %d\n\t"
                "mov r8, %d\n\t"
                "mov r9, %d\n\t"
                "call getNull\n\t"
                "pop rdi\n\t"
                "mov rdi, rax\n\t"
                "call bof\n\t"
                "pop r15\n\t"
                "pop r14\n\t"
                "pop r13\n\t"
                "pop r12\n\t"
                "pop r11\n\t"
                "pop r10\n\t"
                "pop r9\n\t"
                "pop r8\n\t"
                "pop rbp\n\t"
                "pop rbx\n\t"
                "pop rax\n\t"
                "pop rcx\n\t"
                "pop rdx\n\t"
                "pop rsi\n\t"
                "pop rdi\n\t";

// const char* bofStackAsm = "push %rdi\n\t" //here use ; or \n\t to sepetate lines in inline assembly, pay attention to the multiple line string writen
//                 "push %rsi\n\t"
//                 "push %rdx\n\t"
//                 "push %rcx\n\t"
//                 "push %rax\n\t"
//                 "call timestamp\n\t"
//                 "lea [rsp+%d], %rdi\n\t"
//                 "mov $%d, %rsi\n\t"
//                 "mov $%d, %rdx\n\t"
//                 "mov $%d, %rcx\n\t"
//                 "mov $%d, %r8\n\t"
//                 "mov $%d, %r9\n\t"
//                 "call getNull\n\t"
//                 "mov %rax, %rdi\n\t"
//                 "call bof\n\t"
//                 "pop %rax\n\t"
//                 "pop %rcx\n\t"
//                 "pop %rdx\n\t"
//                 "pop %rsi\n\t"
//                 "pop %rdi\n\t";

const char* bofStackAsm = "push rdi\n\t" //here use ; or \n\t to sepetate lines in inline assembly, pay attention to the multiple line string writen
                "push rsi\n\t"
                "push rdx\n\t"
                "push rcx\n\t"
                "push rax\n\t"
                "push rbx\n\t"
                "push rbp\n\t"
                "push r8\n\t"
                "push r9\n\t"
                "push r10\n\t"
                "push r11\n\t"
                "push r12\n\t"
                "push r13\n\t"
                "push r14\n\t"
                "push r15\n\t"
                "call timestamp\n\t"
                "lea %rdi, [rsp+%d]\n\t"
                "mov rsi, %d\n\t"
                "mov rdx, %d\n\t"
                "mov rcx, %d\n\t"
                "mov r8, %d\n\t"
                "mov r9, %d\n\t"
                "call getNull\n\t"
                "pop rdi\n\t"
                "mov rdi, rax\n\t"
                "call bof\n\t"
                "pop r15\n\t"
                "pop r14\n\t"
                "pop r13\n\t"
                "pop r12\n\t"
                "pop r11\n\t"
                "pop r10\n\t"
                "pop r9\n\t"
                "pop r8\n\t"
                "pop rbp\n\t"
                "pop rbx\n\t"
                "pop rax\n\t"
                "pop rcx\n\t"
                "pop rdx\n\t"
                "pop rsi\n\t"
                "pop rdi\n\t";


const char* heavyCalcAsm = "push rdi\n\t" //here use ; or \n\t to sepetate lines in inline assembly, pay attention to the multiple line string writen
                "push rsi\n\t"
                "push rdx\n\t"
                "push rcx\n\t"
                "push rax\n\t"
                "push rbx\n\t"
                "push rbp\n\t"
                "push r8\n\t"
                "push r9\n\t"
                "push r10\n\t"
                "push r11\n\t"
                "push r12\n\t"
                "push r13\n\t"
                "push r14\n\t"
                "push r15\n\t"
                "call heavyCalc\n\t" //here means automaticaly get the context
                "pop r15\n\t"
                "pop r14\n\t"
                "pop r13\n\t"
                "pop r12\n\t"
                "pop r11\n\t"
                "pop r10\n\t"
                "pop r9\n\t"
                "pop r8\n\t"
                "pop rbp\n\t"
                "pop rbx\n\t"
                "pop rax\n\t"
                "pop rcx\n\t"
                "pop rdx\n\t"
                "pop rsi\n\t"
                "pop rdi\n\t";


void bof(int num);
void uaf(int num);
void markBBL(int a);
void timeStamp();
void heavyCalc(uint64_t num);



#endif // BUG_H