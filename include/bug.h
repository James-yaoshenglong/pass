#ifndef BUG_H
#define BUG_H

//pay attention to the intel syntax or AT&T syntax
const char* bofRegAsm = "push %rdi\n\t" //here use ; or \n\t to sepetate lines in inline assembly, pay attention to the multiple line string writen
                "push %rsi\n\t"
                "push %rdx\n\t"
                "push %rcx\n\t"
                "push %rax\n\t"
                "call timestamp\n\t"
                "push %%%s\n\t"
                "mov %rsp, %rdi\n\t"
                "mov %d, %rsi\n\t"
                "mov %d, %rdx\n\t"
                "mov %d, %rcx\n\t"
                "mov %d, %r8\n\t"
                "mov %d, %r9\n\t"
                "call getNull\n\t"
                "pop %rdi\n\t"
                "mov %rax, %rdi\n\t"
                "call bof\n\t"
                "pop %rax\n\t"
                "pop %rcx\n\t"
                "pop %rdx\n\t"
                "pop %rsi\n\t"
                "pop %rdi\n\t";

const char* bofStackAsm = "push %rdi\n\t" //here use ; or \n\t to sepetate lines in inline assembly, pay attention to the multiple line string writen
                "push %rsi\n\t"
                "push %rdx\n\t"
                "push %rcx\n\t"
                "push %rax\n\t"
                "call timestamp\n\t"
                "lea [rsp+%d], %rdi\n\t"
                "mov $%d, %rsi\n\t"
                "mov $%d, %rdx\n\t"
                "mov $%d, %rcx\n\t"
                "mov $%d, %r8\n\t"
                "mov $%d, %r9\n\t"
                "call getNull\n\t"
                "mov %rax, %rdi\n\t"
                "call bof\n\t"
                "pop %rax\n\t"
                "pop %rcx\n\t"
                "pop %rdx\n\t"
                "pop %rsi\n\t"
                "pop %rdi\n\t";

void bof(int num);
void uaf(int num);
void markBBL(int a);
void timeStamp();



#endif // BUG_H