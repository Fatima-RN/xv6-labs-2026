#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"
#include "kernel/syscall.h"

int main(int argc, char *argv[]) {
    printf("Starting interpose test...\n");

    int mask = (1 << SYS_open);
    
    if (interpose(mask, "/allowed") < 0) {
        printf("interpose failed\n");
        exit(1);
    }

    int fd = open("/README", O_RDONLY);
    if (fd < 0) {
        printf("SUCCESS: Blocked open outside sandbox (/README)\n");
    } else {
        printf("FAIL: Allowed open outside sandbox!\n");
        close(fd);
    }

    exit(0);
}
