#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, int *argv[]){
    
    char p[32];

    if(fork() == 0) {
        exit(0, "Goodbye World xv6");
    }
    else {
        wait(0, p);
        printf("%s\n", p);
    }
    exit(0, "Goodbye World xv6");
}