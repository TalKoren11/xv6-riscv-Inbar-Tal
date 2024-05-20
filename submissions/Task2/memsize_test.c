#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    printf("Before alloc: %d\n", memsize());

    // Allocate memory
    void * malToCheck  = malloc(20000);

    printf("After alloc before free: %d\n", memsize());

    free(malToCheck);

    printf("After free: %d\n", memsize());

    exit(0);
}