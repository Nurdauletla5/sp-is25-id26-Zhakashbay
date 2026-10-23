#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>

static int is_number(const char *s)
{
    if (!*s) return 0;
    for (; *s; s++)
        if (!isdigit((unsigned char)*s)) return 0;
    return 1;
}

int main(void)
{
    DIR *d = opendir("/proc");
    if (!d) {
        perror("opendir /proc");
        return 1;
    }

    printf("%7s %7s %5s  %s\n", "PID", "PPID", "STATE", "NAME");

    struct dirent *e;
    while ((e = readdir(d)) != NULL) {
        if (!is_number(e->d_name))
            continue;

        char path[300];
        snprintf(path, sizeof path, "/proc/%s/stat", e->d_name);

        FILE *f = fopen(path, "r");
        if (!f)
            continue;

        char buf[1024];
        size_t n = fread(buf, 1, sizeof buf - 1, f);
        fclose(f);
        if (n == 0)
            continue;
        buf[n] = '\0';

        char *l = strchr(buf, '(');
        char *r = strrchr(buf, ')');
        if (!l || !r || r < l)
            continue;
        *r = '\0';

        char state;
        int ppid;
        if (sscanf(r + 1, " %c %d", &state, &ppid) != 2)
            continue;

        printf("%7s %7d %5c  %s\n", e->d_name, ppid, state, l + 1);
    }

    closedir(d);
    return 0;
}
