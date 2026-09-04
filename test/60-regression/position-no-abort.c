#include <mulle-layout/mulle-layout.h>

#include <stdio.h>


int   main()
{
   CGRect   result;

   // Top|Bottom = 0x3, an invalid nibble
   // The function should handle this gracefully (e.g. return unchanged rect)
   // instead of calling abort()
   // We pass an "impossible" positioning mode and expect it to be
   // treated as no-op (keep position unchanged)
   result = CGRectPositionInRect( CGRectMake( 10, 20, 30, 40),
                                  CGRectMake( 0, 0, 100, 100),
                                  CGContentModeTop | CGContentModeBottom);
   mulle_printf( "x=%.1f y=%.1f w=%.1f h=%.1f\n",
                  result.origin.x, result.origin.y,
                  result.size.width, result.size.height);

   return( 0);
}
