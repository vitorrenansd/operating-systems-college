#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// Returns process PID
int main() {
    printf("PID: %d\n", getpid());

    int fd = open("/tmp/test_file", O_CREAT | O_RDWR, 0644);
    write(fd, "Hello Syscall", 13);
    close(fd);

    getchar();
    return 0;
}

/*
# Analyze
sudo perf trace -e raw_syscalls:sys_enter,raw_syscalls:sys_exit ./syscall_test
*/