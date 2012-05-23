PROCESSORS ?= 4

CC := $(CROSS_COMPILE)gcc
CFLAGS := -lpthread -lm -O2

PARAMS := params.h
OBJS := susan_parallel

all: $(OBJS)

susan_parallel: susan_parallel.c $(PARAMS)
	$(CC) -static $< -o $@ $(CFLAGS)

$(PARAMS):
	./gen_defs.sh $(PARAMS) $(PROCESSORS)

clean:
	rm -rf $(OBJS) output* $(PARAMS)

.PHONY: all clean $(PARAMS)
