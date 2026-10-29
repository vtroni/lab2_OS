CC     = gcc
CFLAGS = -std=c11 -Wall -Wextra -g -O0

PROGRAMS = part1-maps/maps part4-pagefaults/pagefaults part5-mmap/mmap_read bonus-access/access

all: $(PROGRAMS)
	$(MAKE) -C part2-asan

bonus-access/access: CFLAGS += -O2

%: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(PROGRAMS)
	$(MAKE) -C part2-asan clean

.PHONY: all clean
