#include <stdio.h>
#include <unistd.h>

int main() {
    fork();
    fork();
    printf("hello from %d! my parent is %d.\n", getpid(), getppid());
}
