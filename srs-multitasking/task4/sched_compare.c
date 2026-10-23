#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <sys/wait.h>

void worker(const char *name)
{
    long counter = 0;

    while (counter < 1000000000)
    {
        counter++;

        if (counter % 100000000 == 0)
        {
            printf("%s: %ld\n", name, counter);
            fflush(stdout);
        }
    }
}

int main()
{
    pid_t pid = fork();

    if (pid == 0)
    {
        struct sched_param param;
        param.sched_priority = 50;

        if (sched_setscheduler(0, SCHED_FIFO, &param) == -1)
        {
            perror("sched_setscheduler");
            exit(1);
        }

        worker("FIFO");
    }
    else
    {
        worker("OTHER");
        wait(NULL);
    }

    return 0;
}
