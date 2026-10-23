
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define SIZE 10000000

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <N>\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);

    if (N <= 0) {
        fprintf(stderr, "N must be > 0\n");
        return 1;
    }

    long long *arr = malloc(SIZE * sizeof(long long));

    if (arr == NULL) {
        perror("malloc");
        return 1;
    }

    for (int i = 0; i < SIZE; i++) {
        arr[i] = 1;
    }

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    int pipes[N][2];

    for (int i = 0; i < N; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            return 1;
        }

        pid_t pid = fork();

        if (pid < 0) {
            perror("fork");
            return 1;
        }

        if (pid == 0) {
            close(pipes[i][0]);

            int begin = i * SIZE / N;
            int finish = (i + 1) * SIZE / N;

            long long partial_sum = 0;

            for (int j = begin; j < finish; j++) {
                partial_sum += arr[j];
            }

            write(pipes[i][1], &partial_sum, sizeof(partial_sum));

            close(pipes[i][1]);

            free(arr);

            exit(0);
        }

        close(pipes[i][1]);
    }

    long long total_sum = 0;

    for (int i = 0; i < N; i++) {
        long long partial_sum;

        read(pipes[i][0], &partial_sum, sizeof(partial_sum));

        total_sum += partial_sum;

        close(pipes[i][0]);
    }

    for (int i = 0; i < N; i++) {
        int status;
        wait(&status);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    double elapsed =
        (end.tv_sec - start.tv_sec) +
        (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("Processes: %d\n", N);
    printf("Total sum: %lld\n", total_sum);
    printf("Time: %.6f sec\n", elapsed);

    free(arr);

    return 0;
}
