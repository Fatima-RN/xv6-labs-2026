#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  char buf[64];
  int fd = open("secret.txt", O_RDONLY);
  if(fd < 0){
    printf("attack: access denied / open failed\n");
    exit(1);
  }

  int n = read(fd, buf, sizeof(buf) - 1);
  if(n > 0){
    buf[n] = '\0';
    printf("%s", buf);
  }
  close(fd);
  exit(0);
}
