#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int global_initialized = 42;
int global_uninitialized;

static void print_maps(void) {
  FILE *f = fopen("/proc/self/maps", "r");
  if (f == NULL) {
    perror("fopen /proc/self/maps");
    exit(1);
  }
  char line[512];
  while (fgets(line, sizeof line, f) != NULL)
    fputs(line, stdout);
  fclose(f);
}

static void nested(void) {
  int local_in_nested = 2;
  /* TODO 3: надрукувати адресу local_in_nested. Вона вища чи нижча за local у
   * main? */
  printf("local_in_nested  = %p\n", (void *)&local_in_nested);
  (void)local_in_nested;
}

int main(void) {
  int local = 1;
  const char *literal = "string literal";
  char *small = malloc(100);
  char *big = malloc(4 * 1024 * 1024);
  if (small == NULL || big == NULL) {
    perror("malloc");
    return 1;
  }

  printf("pid                 = %d\n", getpid());
  printf("main (code)         = %p\n", (void *)main);
  printf("global_initialized  = %p\n", (void *)&global_initialized);

  /* TODO 1: надрукувати в тому ж форматі адреси:
   *   global_uninitialized, literal (сам рядок, не змінна literal),
   *   local, small, big.
   * Адресу змінної беруть оператором &. literal, small і big — покажчики:
   * друкуйте їхнє значення без &, інакше отримаєте адресу самої змінної на стеку.
   * Формат %p приймає void *, тому потрібне явне приведення (void *). */

  printf("global_uninitialized  = %p\n", (void *)&global_uninitialized);
  printf("literal               = %p\n", (const void *)literal);
  printf("local                 = %p\n", (void *)&local);
  printf("small                 = %p\n", (void *)small);
  printf("big                   = %p\n", (void *)big);

  printf("\n--- /proc/self/maps ---\n");
  print_maps();

  /* TODO 2: в answers.md зіставити кожну адресу з рядком maps і назвати область
   * (код програми, дані, bss, [heap], [stack], анонімне відображення, libc). */

  nested();

  free(small);
  free(big);
  (void)local;
  (void)literal;
  return 0;
}