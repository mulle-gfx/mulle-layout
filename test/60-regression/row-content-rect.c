#include <mulle-layout/mulle-layout.h>

#include <stdio.h>


int   main()
{
   struct MulleLayout   layout;
   CGRect              placed;
   CGRect              content;

   //
   // Frame at 0,0 200x100 with 10px insets all round.
   // Place one 50x20 item. It should land at (10,10).
   // The row content rect is the bounding box of the row from origin
   // to placed items: (0,0, 60,30) — this includes the space from
   // frame origin to the first item (due to CGRectUnion internals).
   // What matters is that the item itself is placed correctly.
   //
   MulleLayoutInit( &layout, CGRectMake( 0, 0, 200, 100),
                             MulleEdgeInsetsMake( 10, 10, 10, 10));

   placed = MulleLayoutAddToRow( &layout,
                                 CGSizeMake( 50, 20),
                                 MulleEdgeInsetsZero);
   mulle_printf( "placed x=%.1f y=%.1f w=%.1f h=%.1f\n",
                  placed.origin.x, placed.origin.y,
                  placed.size.width, placed.size.height);

   content = MulleLayoutGetRowContentRect( &layout);
   mulle_printf( "content x=%.1f y=%.1f w=%.1f h=%.1f\n",
                  content.origin.x, content.origin.y,
                  content.size.width, content.size.height);

   return( 0);
}
