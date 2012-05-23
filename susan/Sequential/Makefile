CC := $(CROSS_COMPILE)gcc
CFLAGS := -O2 -lm

OBJS := susan

all: $(OBJS)

susan: susan.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -rf $(OBJS) output*

.PHONY: clean all
