SUBDIRS := Sequential Parallel

all:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir; \
	done

.PHONY: all clean

clean:
	@for dir in $(SUBDIRS); do \
		$(MAKE) clean -C $$dir; \
	done
