/* +++Date last modified: 05-Jul-1997 */

/*
**  Bit counter by Ratko Tomic
*/

#include "bitops.h"

#define LENGTH 2000000

int CDECL bit_count(long x)
{
        int n = 0;
/*
** The loop will execute once for each bit of x set, this is in average
** twice as fast as the shift/test method.
*/
        if (x) do{
              n++;
        }while (0 != (x = x&(x-1))) ;
        return(n);
}

//#ifdef TEST

#include <stdlib.h>
#include "snip_str.h"               /* For plural_text() macro    */

main(int argc, char *argv[])
{
	/*
      long n;
      while(--argc)
      	{
            int i;
            n = atol(*++argv);
            i = bit_count(n);
            
      	}
	*/
	long inputs[LENGTH];
	long sums[LENGTH];
	int i=0;
	int sum=0;
	long n = atol(*++argv);
	for(i=0;i<LENGTH;i++){
		inputs[i] = n;
	}
	for(i=0;i<LENGTH;i++){
            sum+= bit_count(inputs[i]);
	}
	printf("%d times %ld contains %d bit%s set\n", LENGTH, n, sum, plural_text(sum));
      return 0;
}

//#endif /* TEST */
