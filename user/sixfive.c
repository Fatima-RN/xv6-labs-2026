#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

char separators[] = " -\r\t\n./,";

void process(int fd) {
  char buf[1];
  char numbuf[32];
  int nidx = 0;
  int in_number = 0;
  int valid_digits = 1;

  while(read(fd, buf, 1) > 0){
    char c = buf[0];
    if(strchr(separators, c)){
      if(in_number && valid_digits && nidx > 0){
        numbuf[nidx] = '\0';
        int val = atoi(numbuf);
        if(val % 5 == 0 || val % 6 == 0){
          printf("%d\n", val);
        }
      }
      nidx = 0;
      in_number = 0;
      valid_digits = 1;
    } else if(c >= '0' && c <= '9'){
      if(!in_number){
        in_number = 1;
        nidx = 0;
        valid_digits = 1;
      }
      if(nidx < (int)sizeof(numbuf) - 1){
        numbuf[nidx++] = c;
      }
    } else {
      valid_digits = 0;
      in_number = 1;
    }
  }
  if(in_number && valid_digits && nidx > 0){
    numbuf[nidx] = '\0';
    int val = atoi(numbuf);
    if(val % 5 == 0 || val % 6 == 0){
      printf("%d\n", val);
    }
  }
}

int main(int argc, char *argv[]) {
  int i, fd;

  if(argc <= 1){
    process(0);
    exit(0);
  }

  for(i = 1; i < argc; i++){
    if((fd = open(argv[i], 0)) < 0){
      fprintf(2, "sixfive: cannot open %s\n", argv[i]);
      exit(1);
    }
    process(fd);
    close(fd);
  }
  exit(0);
}
