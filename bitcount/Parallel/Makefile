FILE = pt.c bitcnt_1_parallel.c bitcnt_2.c bitcnt_3.c bitcnt_4.c bitcnts_parallel.c bitfiles.c bitstring_parallel_spf.c bitstring_partition_spf.c bstr_i.c 

bitcnts: ${FILE} Makefile
	gcc -static ${FILE} -O3 -o bitcnts_parallel -lpthread

clean:
	rm -rf bitcnts_parallel output*
