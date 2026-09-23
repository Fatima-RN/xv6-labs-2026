#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int bytes = freemem();
  printf("Free memory available: %d bytes (%d KB)\n", bytes, bytes / 1024);
  exit(0);
}
