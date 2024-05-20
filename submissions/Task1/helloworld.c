#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  write(1, "Hello World xv6", 15);
  exit(0);
}