PROCESSORS ?= 4

CC := $(CROSS_COMPILE)gcc
CFLAGS := -lpthread -lm -O2

PARAMS := params.h
FILE1 = parallel_basicmath.c   rad2deg.c  cubic.c   isqrt.c

OBJS := parallel_basicmath

all: $(OBJS)

parallel_basicmath: $(FILE1) $(PARAMS)
	$(CC) $(CFLAGS) $(FILE1) -o $@

$(PARAMS):
	./gen_defs.sh $(PARAMS) $(PROCESSORS)

clean:
	rm -rf $(OBJS) output* $(PARAMS)

.PHONY: all clean $(PARAMS)
