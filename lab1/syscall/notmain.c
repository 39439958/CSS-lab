#include "mini-lib.h"

struct timespec ts = {.tv_sec = 1, .tv_nsec = 0};

int notmain(){
    alarm(5);
   
 #ifdef DEBUG  
   int n = 3;
   char c = '*';
   char *s = "abcdef";
   printf("print int : %d\n", n);
   printf("print char : %c\n", c);
   printf("print string : %s\n", s);
#endif

    for(;;) {
        puts("hello, world!\n");
        sleep(&ts, 0);
    }

    exit(0);
}


