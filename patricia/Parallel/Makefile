PROCESSORS ?= 4

CC := $(CROSS_COMPILE)gcc
CFLAGS := -lpthread -O2

PARAMS := params.h
OBJS := patricia_parallel

SRCS := patricia_tool.c patricia_parallel.c patricia_test.c

all: $(OBJS)

patricia_parallel: $(SRCS) $(PARAMS)
	$(CC) $(CFLAGS) $(SRCS) -o $@

$(PARAMS):
	./gen_defs.sh $(PARAMS) $(PROCESSORS)

clean:
	rm -rf $(OBJS) output* $(PARAMS)

.PHONY: all clean $(PARAMS)
