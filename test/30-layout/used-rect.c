#include <mulle-layout/mulle-layout.h>

#include <stdio.h>


int   main( void)
{
   struct MulleLayout  layout;
   CGRect              used;

   //
   // Empty layout — used rect should be zero
   //
   MulleLayoutInit( &layout, CGRectMake( 0, 0, 200, 300),
                    MulleEdgeInsetsMake( 5, 10, 5, 10));

   used = MulleLayoutGetUsedRect( &layout);
   mulle_printf( "empty: %g %g %g %g\n",
                  used.origin.x, used.origin.y,
                  used.size.width, used.size.height);

   //
   // One item — used rect covers just that item's row
   //
   MulleLayoutAddToRow( &layout, CGSizeMake( 80, 30),
                        MulleEdgeInsetsMake( 0, 0, 0, 0));
   used = MulleLayoutGetUsedRect( &layout);
   mulle_printf( "one_item: %g %g %g %g\n",
                  used.origin.x, used.origin.y,
                  used.size.width, used.size.height);

   //
   // Two items same row
   //
   MulleLayoutAddToRow( &layout, CGSizeMake( 80, 50),
                        MulleEdgeInsetsMake( 0, 0, 0, 0));
   used = MulleLayoutGetUsedRect( &layout);
   mulle_printf( "two_items: %g %g %g %g\n",
                  used.origin.x, used.origin.y,
                  used.size.width, used.size.height);

   //
   // New row + item — height grows
   //
   MulleLayoutNewRow( &layout);
   MulleLayoutAddToRow( &layout, CGSizeMake( 100, 25),
                        MulleEdgeInsetsMake( 0, 0, 0, 0));
   used = MulleLayoutGetUsedRect( &layout);
   mulle_printf( "second_row: %g %g %g %g\n",
                  used.origin.x, used.origin.y,
                  used.size.width, used.size.height);

   //
   // Third row
   //
   MulleLayoutNewRow( &layout);
   MulleLayoutAddToRow( &layout, CGSizeMake( 60, 40),
                        MulleEdgeInsetsMake( 0, 0, 8, 0));
   used = MulleLayoutGetUsedRect( &layout);
   mulle_printf( "third_row: %g %g %g %g\n",
                  used.origin.x, used.origin.y,
                  used.size.width, used.size.height);

   MulleLayoutDone( &layout);

   //
   // NULL safety
   //
   used = MulleLayoutGetUsedRect( NULL);
   mulle_printf( "null: %g %g %g %g\n",
                  used.origin.x, used.origin.y,
                  used.size.width, used.size.height);

   //
   // Reverse layout
   //
   MulleLayoutInitReverse( &layout, CGRectMake( 0, 0, 200, 100),
                           MulleEdgeInsetsMake( 5, 10, 5, 10));
   MulleLayoutAddToRow( &layout, CGSizeMake( 50, 30),
                        MulleEdgeInsetsMake( 0, 0, 0, 0));
   MulleLayoutNewRow( &layout);
   MulleLayoutAddToRow( &layout, CGSizeMake( 50, 20),
                        MulleEdgeInsetsMake( 0, 0, 0, 0));
   used = MulleLayoutGetUsedRect( &layout);
   mulle_printf( "reverse: %g %g %g %g\n",
                  used.origin.x, used.origin.y,
                  used.size.width, used.size.height);

   MulleLayoutDone( &layout);

   return( 0);
}
