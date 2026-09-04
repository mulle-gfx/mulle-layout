#include <mulle-layout/mulle-layout.h>

#include <stdio.h>


int   main()
{
   CGRect   rects[ 3];

   //
   // Frame 10 wide, spacing 20, n=3.
   // width = (10 - 2*20) / 3 = -10 ... negative.
   // The function should clamp to 0 or return degenerate rects
   // with non-negative width, not produce width = -10.
   //
   CGRectPartitionHorizontally( CGRectMake( 0, 0, 10, 10), 20, 3, rects);

   mulle_printf( "w0=%.1f\n", rects[0].size.width >= 0.0 ? rects[0].size.width : 0.0f);
   mulle_printf( "w_nonneg=%d\n", rects[0].size.width >= 0.0 ? 1 : 0);

   return( 0);
}
