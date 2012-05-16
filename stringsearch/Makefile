FILE1 = bmhasrch.c  bmhisrch.c  bmhsrch.c  parallel_stringsearch.c 

all: parallel_stringsearch

parallel_stringsearch: ${FILE1} Makefile
	gcc -static ${FILE1} -O3 -o parallel_stringsearch -lpthread

clean:
	rm -rf parallel_stringsearch output*
