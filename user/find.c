#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void
find(char *path, char *filename)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  if(st.type == T_DIR){
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("find: path too long\n");
      close(fd);
      return;
    }
    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      if(strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf("find: cannot stat %s\n", buf);
        continue;
      }
      
      if(strcmp(de.name, filename) == 0){
        printf("%s\n", buf);
      }
      
      if(st.type == T_DIR){
        find(buf, filename);
      }
    }
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  struct stat st;

  if(argc != 3){
    fprintf(2, "Usage: find <path> <filename>\n");
    exit(1);
  }

  // Check if the starting path itself matches the target name
  if(stat(argv[1], &st) >= 0){
    char *p;
    for(p = argv[1] + strlen(argv[1]); p >= argv[1] && *p != '/'; p--)
      ;
    p++;
    if(strcmp(p, argv[2]) == 0){
      printf("%s\n", argv[1]);
    }
  }

  find(argv[1], argv[2]);
  exit(0);
}
