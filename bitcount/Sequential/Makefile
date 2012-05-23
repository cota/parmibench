CC := $(CROSS_COMPILE)gcc
CFLAGS := -static -O2

OBJS := bitcnts

FILE := bitcnt_1.c bitcnt_2.c bitcnt_3.c bitcnt_4.c bitcnts.c \
	bitfiles.c bitstrng.c bstr_i.c

all: $(OBJS)

bitcnts: $(FILE)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -rf $(OBJS) output*

.PHONY: clean all
