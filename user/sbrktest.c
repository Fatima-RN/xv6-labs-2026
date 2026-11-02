#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    printf("Before sbrk(1): brk = %p\n", sbrk(0));
    char *p = sbrk(1);
    printf("After sbrk(1): returned p = %p, brk = %p\n", p, sbrk(0));
    
    // Memory access test
    *p = 'A';
    printf("Successfully wrote 'A' at %p\n", p);
    
    exit(0);
}
