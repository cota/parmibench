/* +++Date last modified: 05-Jul-1997 */

/*
**  Make an ascii binary string into an integer.
**
**  Public domain by Bob Stout
*/

#include <string.h>
#include "bitops.h"

unsigned int bstr_i(char *cptr)
{
      unsigned int i, j = 0;
	int x=0;
	/* It can't be parallelized because j is in aboslute sequency.
	 An used parallelization strategy: bstr_i(111 111 111) = bstr_i(111 000 000) + bstr_i(111 000) + bstr_i(111), it dosen't work. 

	 */
      while (cptr && *cptr && strchr("01", *cptr))
      {
            i = *cptr++ - '0';
            j <<= 1;
            j |= (i & 0x01);
		x++;
      }
	printf("x=%d\n",x);
      return(j);
}

#ifdef TEST

#include <stdlib.h>

int main(int argc, char *argv[])
{
      char *arg;
      unsigned int x;

      while (--argc)
      {
            x = bstr_i(arg = *++argv);
            printf("Binary %s = %d = %04Xh\n", arg, x, x);
      }
      return EXIT_SUCCESS;
}

#endif /* TEST */
