#define _GNU_SOURCE
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include <unistd.h>

struct snapshot {
    long minor_faults;
    long rss_kib;
};

static struct snapshot take_snapshot(void)
{
    struct rusage ru;
    long total_pages = 0, resident_pages = 0;

    FILE *f = fopen("/proc/self/statm", "r");

    if (f == NULL || fscanf(f, "%ld %ld", &total_pages, &resident_pages) != 2) {
        perror("/proc/self/statm");
        exit(1);
    }
    fclose(f);

    struct snapshot s = {
        .minor_faults = ru.ru_minflt,
        .rss_kib = resident_pages * sysconf(_SC_PAGESIZE) / 1024,
    };
    return s;
}

static void report(const char *step, struct snapshot before, struct snapshot after)
{
    printf("%-26s faults: %+7ld   RSS: %7ld KiB (%+ld KiB)\n", step,
           after.minor_faults - before.minor_faults, after.rss_kib,
           after.rss_kib - before.rss_kib);
}

int main(int argc, char **argv)
{
    if (argc > 2) {
        fprintf(stderr, "usage: %s [pages]\n", argv[0]);
        return 2;
    }

    long page_size = sysconf(_SC_PAGESIZE);
    if (page_size <= 0) {
        fprintf(stderr, "cannot determine page size\n");
        return 1;
    }

    long pages = 1024;

    if (argc == 2) {
        char *end;
        errno = 0;
        pages = strtol(argv[1], &end, 10);

        if (errno != 0 || end == argv[1] || *end != '\0' || pages <= 0 ||
            (uintmax_t)pages > SIZE_MAX / (uintmax_t)page_size) {
            fprintf(stderr, "invalid page count: %s (expected a positive integer without size overflow)\n", argv[1]);
            return 2;
        }
    }

    size_t length = (size_t)pages * (size_t)page_size;
    printf("pages: %ld, page size: %ld B, region: %zu KiB\n\n", pages, page_size, length / 1024);

    take_snapshot();
    struct snapshot s0 = take_snapshot();

    char *region = mmap(NULL, length, PROT_READ | PROT_WRITE,
                        MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (region == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    struct snapshot s1 = take_snapshot();
    report("mmap", s0, s1);

    /* TODO 1: прочитати по одному байту з кожної сторінки (крок page_size),
     * накопичуючи суму в sum, щоб компілятор не викинув цикл. */
    volatile long sum = 0;
    for (long i = 0; i < pages; i++)
        sum += region[i * page_size];

    struct snapshot s2 = take_snapshot();
    report("read every page", s1, s2);

    /* TODO 2: записати по одному байту в кожну сторінку. */
    for (long i = 0; i < pages; i++)
        region[i * page_size] = 1;

    struct snapshot s3 = take_snapshot();
    report("first write to every page", s2, s3);

    /* TODO 3: записати ще раз по одному байту в кожну сторінку. */
    for (long i = 0; i < pages; i++)
        region[i * page_size] = 2;

    struct snapshot s4 = take_snapshot();
    report("second write", s3, s4);

    /* TODO 4: викликати madvise(region, length, MADV_DONTNEED),
     * потім знову записати по одному байту в кожну сторінку. */
    if (madvise(region, length, MADV_DONTNEED) != 0) {
        perror("madvise");
        return 1;
    }
    for (long i = 0; i < pages; i++)
        region[i * page_size] = 3;

    struct snapshot s5 = take_snapshot();
    report("write after MADV_DONTNEED", s4, s5);

    munmap(region, length);
    (void)sum;
    return 0;
}
