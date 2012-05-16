FILE1 = parallel_basicmath.c   rad2deg.c  cubic.c   isqrt.c

all: parallel_basicmath

parallel_basicmath: ${FILE1} Makefile
	gcc -static -O3 ${FILE1} -o parallel_basicmath -lm -lpthread

clean:
	rm -rf parallel_basicmath output*