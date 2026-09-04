#include <mulle-layout/mulle-layout.h>

#include <stdio.h>


int   main()
{
   CGRect   slice;
   CGRect   remainder;

   //
   // In Apple's CoreGraphics, CGRectMinXEdge = 0.
   // Our header defines CGRectMinXEdge = CADirectionLeft = 4.
   // Ported code may pass literal 0 for the left edge divide.
   // The function should either handle 0 gracefully or the enum
   // should match Apple's values. Currently it calls abort().
   //
   // We test with the library's own CGRectMinXEdge (= 4) to show
   // a left-edge divide. If the library later unifies with Apple's
   // values (0), this test still validates the divide logic.
   //
   CGRectDivide( CGRectMake( 0, 0, 100, 50),
                 &slice, &remainder, 30, CGRectMinXEdge);

   mulle_printf( "slice x=%.1f y=%.1f w=%.1f h=%.1f\n",
                  slice.origin.x, slice.origin.y,
                  slice.size.width, slice.size.height);
   mulle_printf( "remainder x=%.1f y=%.1f w=%.1f h=%.1f\n",
                  remainder.origin.x, remainder.origin.y,
                  remainder.size.width, remainder.size.height);

   // Now test that passing 0 does not crash (regression for Apple compat)
   // If the fix makes 0 mean "left edge", this output should be identical
   CGRectDivide( CGRectMake( 0, 0, 100, 50),
                 &slice, &remainder, 30, 0);

   mulle_printf( "edge0 slice x=%.1f y=%.1f w=%.1f h=%.1f\n",
                  slice.origin.x, slice.origin.y,
                  slice.size.width, slice.size.height);
   mulle_printf( "edge0 remainder x=%.1f y=%.1f w=%.1f h=%.1f\n",
                  remainder.origin.x, remainder.origin.y,
                  remainder.size.width, remainder.size.height);

   return( 0);
}
