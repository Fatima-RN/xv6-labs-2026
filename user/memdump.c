#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

void memdump(char *fmt, char *data, int len);

int
main(int argc, char *argv[])
{
  if (argc == 1) {
    printf("Example 1:\n");
    int a[2] = {61810, 2026};
    memdump("ii", (char *)a, sizeof(a));

    printf("Example 2:\n");
    memdump("S", "a string", sizeof("a string"));

    printf("Example 3:\n");
    char *s = "another";
    memdump("s", (char *)&s, sizeof(s));

    struct sss {
      char *ptr;
      int num1;
      short num2;
      char byte;
      char bytes[8];
    } example;

    example.ptr = "hello";
    example.num1 = 1819438967;
    example.num2 = 100;
    example.byte = 'z';
    strcpy(example.bytes, "xyzzy");

    printf("Example 4:\n");
    memdump("pihcS", (char *)&example, sizeof(example));

    printf("Example 5:\n");
    memdump("sccccc", (char *)&example, sizeof(example));
  } else if (argc == 2) {
    char data[512];
    int n = 0;
    memset(data, '\0', sizeof(data));
    while (n < sizeof(data)) {
      int nn = read(0, data + n, sizeof(data) - n);
      if (nn <= 0)
        break;
      n += nn;
    }
    memdump(argv[1], data, n);
  } else {
    printf("Usage: memdump [format]\n");
    exit(1);
  }
  exit(0);
}

void
memdump(char *fmt, char *data, int len)
{
  char *ptr = data;
  char *end = data + len;

  for(int i = 0; fmt[i] != '\0'; i++){
    char f = fmt[i];
    switch(f){
    case 'i':
      if(ptr + 4 > end){
        printf("memdump: not enough data for 'i'\n");
        return;
      }
      printf("%d\n", *(int*)ptr);
      ptr += 4;
      break;
    case 'p':
      if(ptr + 8 > end){
        printf("memdump: not enough data for 'p'\n");
        return;
      }
      {
        uint64 val = *(uint64*)ptr;
        for(int j = 15; j >= 0; j--){
          int nibble = (val >> (j * 4)) & 0xf;
          printf("%x", nibble);
        }
        printf("\n");
      }
      ptr += 8;
      break;
    case 'h':
      if(ptr + 2 > end){
        printf("memdump: not enough data for 'h'\n");
        return;
      }
      printf("%d\n", *(short*)ptr);
      ptr += 2;
      break;
    case 'c':
      if(ptr + 1 > end){
        printf("memdump: not enough data for 'c'\n");
        return;
      }
      printf("%c\n", *ptr);
      ptr += 1;
      break;
    case 's':
      if(ptr + 8 > end){
        printf("memdump: not enough data for 's'\n");
        return;
      }
      {
        char *s = *(char**)ptr;
        printf("%s\n", s);
      }
      ptr += 8;
      break;
    case 'S':
      {
        for(char *p = ptr; p < end; p++){
          if(*p == '\0')
            break;
          printf("%c", *p);
        }
        printf("\n");
        ptr = end;
      }
      break;
    }
  }
}
