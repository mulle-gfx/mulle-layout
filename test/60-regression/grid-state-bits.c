#include <mulle-layout/mulle-layout.h>

#include <stdio.h>


int   main()
{
   struct MulleGrid   grid;
   uintptr_t          state;

   // All values given except canvas size (which is 0,0 -> INFINITY -> computed)
   // Should compute canvas width (bit 0x10) and canvas height (bit 0x20)
   // Bug: CanvasHeight is 0x18, collides with ItemHeight|CanvasWidth
   MulleGridInit( &grid, CGRectMake( 0, 0, 0, 0), MulleEdgeInsetsZero,
                         CGSizeMake( 50, 50), 2, 3, CGSizeMake( 10, 10));
   MulleGridConstruct( &grid, 6, &state);

   // CanvasWidth should be separate bit from CanvasHeight
   mulle_printf( "canvas_width_computed=%d\n", (state & 0x10) != 0);
   mulle_printf( "canvas_height_computed=%d\n", (state & 0x20) != 0);
   mulle_printf( "item_height_computed=%d\n", (state & 0x8) != 0);

   return( 0);
}
