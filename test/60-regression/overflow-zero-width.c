#include <mulle-layout/mulle-layout.h>

#include <stdio.h>


int   main( void)
{
   struct MulleLayout   layout;
   CGRect              rect;

   // A zero-width item is legitimate (e.g. a separator).
   // _MulleLayoutAddToRowWithOverflow must NOT treat it as failure
   // and trigger a spurious NewRow.
   MulleLayoutInit( &layout, CGRectMake( 0, 0, 200, 100), MulleEdgeInsetsZero);

   rect = MulleLayoutAddToRowWithOverflow( &layout,
                                           CGSizeMake( 0, 20),
                                           MulleEdgeInsetsZero);
   mulle_printf( "zero-w placed: null=%d y=%.1f\n",
                  CGRectIsNull( rect), rect.origin.y);

   // second item should still be on the same row (y=0)
   rect = MulleLayoutAddToRowWithOverflow( &layout,
                                           CGSizeMake( 50, 20),
                                           MulleEdgeInsetsZero);
   mulle_printf( "next placed: null=%d y=%.1f\n",
                  CGRectIsNull( rect), rect.origin.y);

   return( 0);
}
