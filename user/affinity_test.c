#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, int *argv[]){
    
    set_affinity_mask(5); //5 - 101

    for (;;) {
        printf("PID: %d\n", getpid());
    }

    exit(0, 0);
}