#include <stdio.h>
#include <sys/resource.h>
#include <errno.h>
#include <string.h>

int main() {
    struct rlimit lim;
    getrlimit(RLIMIT_STACK, &lim);
    printf("stack size: %ld\n", lim.rlim_cur);
    int ret = getrlimit(RLIMIT_CPU, &lim);
    printf("process limit: %lu\n", lim.rlim_cur);
    printf("RLIM_INFINITY: %lu\n", RLIM_INFINITY);
    printf("process limit: ret: %d\n", ret);
    printf("process limit: errno: %s\n", strerror(errno));
    getrlimit(RLIMIT_OFILE, &lim);
    printf("max file descriptors: %ld\n", lim.rlim_cur);
    return 0;
}
