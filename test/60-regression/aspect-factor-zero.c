#include <mulle-layout/mulle-layout.h>

#include <stdio.h>
#include <math.h>


int   main( void)
{
   CGFloat   factor;

   // zero src → should return 0, not inf/NaN
   factor = CGSizeGetAspectFitFactor( CGSizeMake( 100, 100), CGSizeMake( 0, 0));
   mulle_printf( "fit zero-src: %.1f\n", factor);

   // zero dst → should return 0
   factor = CGSizeGetAspectFitFactor( CGSizeMake( 0, 0), CGSizeMake( 100, 100));
   mulle_printf( "fit zero-dst: %.1f\n", factor);

   // zero height src → should return 0
   factor = CGSizeGetAspectFillFactor( CGSizeMake( 100, 200), CGSizeMake( 50, 0));
   mulle_printf( "fill zero-src-h: %.1f\n", factor);

   // zero width dst → should return 0
   factor = CGSizeGetAspectFillFactor( CGSizeMake( 0, 100), CGSizeMake( 50, 50));
   mulle_printf( "fill zero-dst-w: %.1f\n", factor);

   // normal case still works
   factor = CGSizeGetAspectFitFactor( CGSizeMake( 200, 100), CGSizeMake( 100, 100));
   mulle_printf( "fit normal: %.1f\n", factor);

   return( 0);
}
