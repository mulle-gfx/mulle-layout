#include <mulle-layout/mulle-layout.h>

#include <stdio.h>


int   main()
{
   CGRect   rects[ 16];

   // columnSize = 0 should be handled gracefully (no-op or early return)
   // not segfault due to unsigned underflow in buffer indexing
   CGRectPartitionIntoRowsAndColumns( CGRectMake( 0, 0, 100, 100),
                                      CGSizeMake( 0, 0),
                                      2, 0,
                                      rects);
   mulle_printf( "survived columnSize=0\n");

   // rowSize = 0 should also be safe
   CGRectPartitionIntoRowsAndColumns( CGRectMake( 0, 0, 100, 100),
                                      CGSizeMake( 0, 0),
                                      0, 3,
                                      rects);
   mulle_printf( "survived rowSize=0\n");

   return( 0);
}
