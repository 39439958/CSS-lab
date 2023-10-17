#include "mini-lib.h"

// see:
//https://blog.rchapman.org/posts/Linux_System_Call_Table_for_x86_64/

// syscall number for x86-64, in rax
#define SYS_WRITE 1
#define SYS_SLEEP 35
#define SYS_ALARM 37
#define SYS_EXIT 60

// arguments: rdi, rsi, rdx, r10, r8, r9


#define CALL2(n) "movq $"#n", %%rax\n"
#define CALL(n) CALL2(n)

int strlen(char *s){
    int n = 0;
    while(*s++)
        n++;
    return n;
}

int printchar(char c) {
    long r;
    char *s = &c;
    asm(CALL(SYS_WRITE)
       "movq $1, %%rdi\n"
       "movq %1, %%rsi\n"
       "movq $1, %%rdx\n"
       "syscall\n"
       "movq %%rax, %0\n"
       : "=r"(r)
       : "r"(s)
       : "%rax", "%rdi", "%rsi", "%rdx");
    return (int)r;
}

int printint(int val) {
    int x = 0, bit = 0, i = 0, j = 0;
    char buf[100];

    if (val < 0) {
        x = -val;
        printchar('-');
    } else 
        x = val;

    while (x != 0) {
        bit = x % 10;
        buf[i++] = bit + 48;
        x /= 10;
    }
    for (j = i - 1; j >= 0; j--)
        printchar(buf[j]);
    return 0;
}

int printf(char *s, ...){
    // only achieve %d %s %c
    va_list ap;
    va_start(ap, s);

    while (*s != '\0') {
        if (*s == '%') {
            s++;
            if (*s == 'd') {
                printint(va_arg(ap, int));
            } else if (*s == 's') {
                puts(va_arg(ap, char*));
            } else if (*s == 'c') {
                printchar(va_arg(ap, int));
            }
        } else {
            printchar(*s);
        }
        s++;
    }
    return 0;
}

int puts(char *s) {
    long n = strlen(s);
    long r;
    asm(CALL(SYS_WRITE)
       "movq $1, %%rdi\n"
       "movq %1, %%rsi\n"
       "movq %2, %%rdx\n"
       "syscall\n"
       "movq %%rax, %0\n"
       : "=r"(r)
       : "r"(s), "r"(n)
       : "%rax", "%rdi", "%rsi", "%rdx");
    return (int)r;
}


void exit(int status){
    long n = (long)status;

    asm(CALL(SYS_EXIT)
        "movq %0, %%rdi\n"
        "syscall\n"
        : 
        : "r"(n)
        : "%rax", "%rdi");
}

int alarm(unsigned int seconds) {
    long r = 0;
    unsigned long n = (unsigned long)seconds;

    asm(CALL(SYS_ALARM)
       "movq %1, %%rdi\n" 
       "syscall\n"
       "movq %%rax, %0\n"
       : "=r"(r)
       : "r"(n)
       : "%rax", "%rdi");

    return (int)r;
}


int sleep(struct timespec *ts, struct timespec *rem){
    
    long r = 0;
    asm(CALL(SYS_SLEEP)
       "movq %1, %%rdi\n"
       "movq %2, %%rsi\n"
       "syscall\n"
       "movq %%rax, %0\n"
       : "=r"(r)
       : "r"(ts), "r"(rem)
       : "%rax", "%rdi", "%rsi");
    return (int)r;
}



