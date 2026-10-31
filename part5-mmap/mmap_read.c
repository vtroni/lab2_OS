/* Готовий приклад: рахує рядки у файлі через відображення його в пам'ять.
 * Запуск: ./mmap_read <файл> */
#define _GNU_SOURCE
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <unistd.h>

static void print_faults(const char *step, struct rusage before)
{
    struct rusage now;
    getrusage(RUSAGE_SELF, &now);

    printf("%-22s minor faults: %+6ld   major faults: %+6ld\n", step,
           now.ru_minflt - before.ru_minflt, now.ru_majflt - before.ru_majflt);
}

static void print_mapping_of(const char *path)
{
    char resolved[PATH_MAX];

    if (realpath(path, resolved) == NULL)
        return;

    FILE *f = fopen("/proc/self/maps", "r");
    if (f == NULL)
        return;
    char line[PATH_MAX + 128];

    while (fgets(line, sizeof line, f) != NULL)
        if (strstr(line, resolved) != NULL)
            fputs(line, stdout);
    fclose(f);
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "usage: %s <file>\n", argv[0]);
        return 2;
    }

    /* 1. Відкрити файл: отримуємо дескриптор, а не дані. */
    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    /* 2. Дізнатися розмір файлу: mmap потребує довжину відображення. */
    struct stat st;
    if (fstat(fd, &st) == -1) {
        perror("fstat");
        return 1;
    }
    if (st.st_size == 0) {
        printf("empty file\n");
        return 0;
    }

    /* 3. Відобразити файл: ядро резервує діапазон віртуальних адрес і запам'ятовує,
     *    що за ним стоїть цей файл. Жоден байт ще не прочитано. */
    char *data = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (data == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    /* 4. Дескриптор більше не потрібен: відображення посилається на сам файл. */
    close(fd);

    // Виводимо інформацію про файл
    printf("file: %s, %lld bytes, %lld pages\n\n", argv[1], (long long)st.st_size,
           (long long)(st.st_size + 4095) / 4096);
    
    print_mapping_of(argv[1]);
    printf("\n");

    /* 5. Читаємо файл як звичайний масив. Перший доступ до кожної сторінки
     *    спричиняє page fault, і ядро підкладає сторінку з page cache або з диска. */
    struct rusage before;
    getrusage(RUSAGE_SELF, &before);

    long lines = 0;

    // Читаємо файл по одному байту та рахуємо символи переходу на новий рядок
    for (off_t i = 0; i < st.st_size; i++)
        if (data[i] == '\n')
            lines++;

    print_faults("counting lines", before);
    printf("\nlines: %ld\n", lines);

    /* 6. Зняти відображення. */
    munmap(data, st.st_size);
    return 0;
}
