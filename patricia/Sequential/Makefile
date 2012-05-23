CC := $(CROSS_COMPILE)gcc
CFLAGS := -static -O2

OBJS := patricia

all: $(OBJS)

patricia: patricia.c patricia_test.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -rf $(OBJS) output*

.PHONY: clean all
