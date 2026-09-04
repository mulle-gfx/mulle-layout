#include <mulle-layout/mulle-layout.h>

#include <stdio.h>


int   main()
{
   CGAffineTransform   t;

   //
   // CGContentModeDefault == 0 means ScaleToFill per the header.
   // Source 10x10 at (0,0) -> Destination 100x200 at (50,50).
   // Expected transform: scale 10x, 20x, translate to (50,50).
   // Bug: the switch in CGAffineTransformMakeWithContentMode does not
   //      handle Default (0) and falls through with identity.
   //
   t = CGAffineTransformMakeWithContentMode( CGContentModeDefault,
                                             CGRectMake( 0, 0, 10, 10), 0,
                                             CGRectMake( 50, 50, 100, 200), 0);

   mulle_printf( "a=%.1f d=%.1f tx=%.1f ty=%.1f\n", t.a, t.d, t.tx, t.ty);

   return( 0);
}
