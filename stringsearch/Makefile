PROCESSORS ?= 4

CC := $(CROSS_COMPILE)gcc
CFLAGS := -lpthread -O2

PARAMS := params.h
OBJS := parallel_stringsearch

SRCS := bmhasrch.c  bmhisrch.c  bmhsrch.c  parallel_stringsearch.c

all: $(OBJS)

parallel_stringsearch: $(SRCS) $(PARAMS)
	$(CC) $(CFLAGS) $(SRCS) -o $@

$(PARAMS):
	./gen_defs.sh $(PARAMS) $(PROCESSORS)

clean:
	rm -rf $(OBJS) output* $(PARAMS)

.PHONY: all clean $(PARAMS)
