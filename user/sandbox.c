#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  printf("--- Sandbox Test Initialized ---\n");
  int fd = open("README", O_RDONLY);
  if (fd < 0) {
    printf("sandbox: open blocked or failed as expected.\n");
  } else {
    printf("sandbox: open succeeded with fd=%d\n", fd);
    close(fd);
  }

  exit(0);
}
