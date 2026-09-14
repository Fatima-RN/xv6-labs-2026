#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"

void
find(char *path, char *filename, int has_exec, char *exec_argv[], int exec_argc)
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
      fprintf(2, "find: path too long\n");
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
        fprintf(2, "find: cannot stat %s\n", buf);
        continue;
      }
      
      if(strcmp(de.name, filename) == 0){
        if(has_exec){
          // Build new argument list: exec_argv + filename (buf)
          char *args[MAXARG];
          int i;
          for(i = 0; i < exec_argc; i++){
            args[i] = exec_argv[i];
          }
          args[exec_argc] = buf;
          args[exec_argc + 1] = 0;

          int pid = fork();
          if(pid < 0){
            fprintf(2, "find: fork failed\n");
            close(fd);
            return;
          }
          if(pid == 0){
            exec(args[0], args);
            fprintf(2, "find: exec %s failed\n", args[0]);
            exit(1);
          }
          wait(0);
        } else {
          printf("%s\n", buf);
        }
      }
      
      if(st.type == T_DIR){
        find(buf, filename, has_exec, exec_argv, exec_argc);
      }
    }
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  struct stat st;
  int has_exec = 0;
  int exec_start = 0;

  if(argc < 3){
    fprintf(2, "Usage: find <path> <filename> [-exec <cmd...>]\n");
    exit(1);
  }

  for(int i = 3; i < argc; i++){
    if(strcmp(argv[i], "-exec") == 0){
      has_exec = 1;
      exec_start = i + 1;
      break;
    }
  }

  // Check if starting path itself matches
  if(stat(argv[1], &st) >= 0){
    char *p;
    for(p = argv[1] + strlen(argv[1]); p >= argv[1] && *p != '/'; p--)
      ;
    p++;
    if(strcmp(p, argv[2]) == 0){
      if(has_exec){
        char *args[MAXARG];
        int j, count = 0;
        for(j = exec_start; j < argc && count < MAXARG - 2; j++, count++){
          args[count] = argv[j];
        }
        args[count++] = argv[1];
        args[count] = 0;

        int pid = fork();
        if(pid == 0){
          exec(args[0], args);
          fprintf(2, "find: exec %s failed\n", args[0]);
          exit(1);
        }
        wait(0);
      } else {
        printf("%s\n", argv[1]);
      }
    }
  }

  find(argv[1], argv[2], has_exec, &argv[exec_start], argc - exec_start);
  exit(0);
}
