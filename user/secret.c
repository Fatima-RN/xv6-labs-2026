#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if(argc < 2){
    printf("Usage: secret <token>\n");
    exit(1);
  }

  int fd = open("secret.txt", O_CREATE | O_WRONLY);
  if(fd < 0){
    printf("secret: failed to create secret.txt\n");
    exit(1);
  }

  write(fd, argv[1], strlen(argv[1]));
  write(fd, "\n", 1);
  close(fd);
  exit(0);
}
