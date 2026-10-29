CC      = gcc
CFLAGS  = -std=c11 -Wall -Wextra -g -O0
ASAN    = -fsanitize=address -fno-omit-frame-pointer
BUGS    = bug1 bug2 bug3 bug4 bug5 bug6

all: $(BUGS)

%: %.c
	$(CC) $(CFLAGS) $(ASAN) $< -o $@

%-plain: %.c
	$(CC) $(CFLAGS) $< -o $@

plain: $(addsuffix -plain,$(BUGS))

run: all
	@status=0; for b in $(BUGS); do echo "=== $$b"; ./$$b || status=1; echo; done; exit $$status

clean:
	rm -f $(BUGS) $(addsuffix -plain,$(BUGS))

.PHONY: all plain run clean
