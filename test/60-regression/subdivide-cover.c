#include <mulle-layout/mulle-layout.h>

#include <stdio.h>


int   main()
{
   CGRect         output[ 4];
   unsigned int   n;
   int            overlap;

   //
   // When 'other' fully covers 'rect', there is nothing left.
   // CGRectSubdivideIfOverlap should return 0 (fully covered).
   // CGRectSubdivide should return 0 rects (nothing remains).
   //
   // Bug: CGRectSubdivide treats return <= 0 from IfOverlap as "no overlap"
   // and returns the original rect, which is wrong for the fully-covered case.
   //
   overlap = CGRectSubdivideIfOverlap( CGRectMake( 0, 0, 100, 100),
                                       CGRectMake( -10, -10, 200, 200),
                                       output);
   mulle_printf( "fully_covered=%d\n", overlap);

   n = CGRectSubdivide( CGRectMake( 0, 0, 100, 100),
                        CGRectMake( -10, -10, 200, 200),
                        output);
   mulle_printf( "subdivide_n=%u\n", n);

   // For no-overlap case, should get 1 rect back (the original)
   n = CGRectSubdivide( CGRectMake( 0, 0, 10, 10),
                        CGRectMake( 500, 500, 10, 10),
                        output);
   mulle_printf( "no_overlap_n=%u x=%.1f y=%.1f\n",
                  n, output[0].origin.x, output[0].origin.y);

   return( 0);
}
