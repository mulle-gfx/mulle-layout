#include <mulle-layout/mulle-layout.h>

#include <stdio.h>


int   main()
{
   struct MulleLayout   layout;
   CGRect              result;

   //
   // Frame 0,0 200x30 with bottom inset of 20. Usable height = 10.
   // A 50x25 item should NOT fit vertically (25 > 10).
   // Bug: the vertical fit check only tests against originalRect maxY,
   //       ignoring originalMargins.bottom.
   //
   MulleLayoutInit( &layout, CGRectMake( 0, 0, 200, 30),
                             MulleEdgeInsetsMake( 0, 0, 20, 0));

   result = MulleLayoutAddToRow( &layout,
                                 CGSizeMake( 50, 25),
                                 MulleEdgeInsetsZero);

   mulle_printf( "rejected=%d\n", CGRectIsNull( result) ? 1 : 0);

   return( 0);
}
