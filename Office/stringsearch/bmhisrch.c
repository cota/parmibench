
/*
*  		Case-Insensitive Boyer-Moore-Horspool pattern match
*		  (Public Domain version by Thad Smith 7/21/1992,based on a 7/92
*			 			public domain BMH version by Raymond Gardner.)
*
*  This program is written in ANSI C and inherits the compilers
*  ability (or lack thereof) to support non-"C" locales by use of
*  toupper() and tolower() to perform case conversions.
*  Limitation: pattern length + string length must be less than 32767.
*  10/21/93 rdg  Fixed bugs found by Jeff Dunlop
*/

#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef unsigned char uchar;
#define LARGE 340000000             /* flag for last character match    */

void bhmi_cleanup(char*);


char *bmhi_search(const char *string, const int stringlen,const char *pattern)
{
      int i, j, lastpatchar;;
      char *s;
      
      int patlen;              /* # chars in pattern               */
			int skip[UCHAR_MAX+1];   /* skip-ahead count for test chars  */
		  int skip2;               /* skip-ahead after non-match with
		  												 ** matching final character         */
			uchar *pat = NULL;       /* uppercase copy of pattern        */

     /*      BMHI Initialization           */
     	
     	patlen = strlen(pattern);

      /* Make uppercase copy of pattern */

      pat = realloc ((void*)pat, patlen);
      if (!pat)
            exit(1);
     // else  atexit(bhmi_cleanup(pat));
      for (i=0; i < patlen; i++)
            pat[i] = toupper(pattern[i]);

      /* initialize skip array */

      for ( i = 0; i <= UCHAR_MAX; ++i )                    /* rdg 10/93 */
            skip[i] = patlen;
      for ( i = 0; i < patlen - 1; ++i )
      {
            skip[        pat[i] ] = patlen - i - 1;
            skip[tolower(pat[i])] = patlen - i - 1;
      }
      lastpatchar = pat[patlen - 1];
      skip[        lastpatchar ] = LARGE;
      skip[tolower(lastpatchar)] = LARGE;
      skip2 = patlen;                     /* Horspool's fixed second shift */
      for (i = 0; i < patlen - 1; ++i)
      {
            if ( pat[i] == lastpatchar )
                  skip2 = patlen - i - 1;
      }
     
     
     /*----------------------------------*/


      i = patlen - 1 - stringlen;
      if (i >= 0)
            return NULL;
      string += stringlen;
      for ( ;; )
      {
            while ( (i += skip[((uchar *)string)[i]]) < 0 )
                  ;                           /* mighty fast inner loop */
            if (i < (LARGE - stringlen))
                  return NULL;
            i -= LARGE;
            j = patlen - 1;
            s = (char *)string + (i - j);
            while ( --j >= 0 && toupper(s[j]) == pat[j] )
                  ;
            if ( j < 0 )                                    /* rdg 10/93 */
                  return s;                                 /* rdg 10/93 */
            if ( (i += skip2) >= 0 )                        /* rdg 10/93 */
                  return NULL;                              /* rdg 10/93 */
      }
}

void bhmi_cleanup(char *pat)
{
      free(pat);
}
