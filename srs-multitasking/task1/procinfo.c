#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/resource.h>

int main(void)
{
    printf("PID   : %d\n", getpid());
    printf("PPID  : %d\n", getppid());
    printf("UID   : %u\n", getuid());
    printf("GID   : %u\n", getgid());
    printf("PGRP  : %d\n", getpgrp());
    printf("SID   : %d\n", getsid(0));

    errno = 0;
    int nice_val = getpriority(PRIO_PROCESS, 0);
    if (nice_val == -1 && errno != 0) {
        perror("getpriority");
        return 1;
    }
    printf("nice  : %d\n", nice_val);

    FILE *f = fopen("/proc/self/status", "r");
    if (!f) {
        perror("fopen /proc/self/status");
        return 1;
    }
    char line[256];
    while (fgets(line, sizeof line, f)) {
        if (strncmp(line, "State:", 6) == 0 || strstr(line, "ctxt_switches"))
            fputs(line, stdout);
    }
    fclose(f);
    return 0;
}
