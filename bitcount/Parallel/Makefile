PROCESSORS ?= 4

CC := $(CROSS_COMPILE)gcc
CFLAGS := -lpthread -O2

PARAMS := params.h
OBJS := bitcnts_parallel

FILE := pt.c bitcnt_1_parallel.c bitcnt_2.c bitcnt_3.c bitcnt_4.c \
	bitcnts_parallel.c bitfiles.c bitstring_parallel_spf.c \
	bitstring_partition_spf.c bstr_i.c

all: $(OBJS)

bitcnts_parallel: $(FILE) $(PARAMS)
	$(CC) $(CFLAGS) $(FILE) -o $@

$(PARAMS):
	./gen_defs.sh $(PARAMS) $(PROCESSORS)

clean:
	rm -rf bitcnts_parallel output* $(PARAMS)

.PHONY: all clean $(PARAMS)
