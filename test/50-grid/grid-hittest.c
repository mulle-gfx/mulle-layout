#include <mulle-layout/mulle-layout.h>

#include <stdio.h>


int   main( void)
{
   struct MulleGrid   grid;
   uintptr_t          state;
   intptr_t           index;

   //
   // 3x3 grid on 100x100 canvas, no insets, 10px spacing, items 26.67x26.67
   //
   MulleGridInit( &grid, CGRectMake( 0, 0, 100, 100),
                  MulleEdgeInsetsZero,
                  CGSizeZero,
                  3,
                  3,
                  CGSizeMake( 10, 10));
   MulleGridConstruct( &grid, 9, &state);

   mulle_printf( "grid: %ux%u item=%.2fx%.2f\n",
                  (unsigned) grid.columns, (unsigned) grid.rows,
                  grid.itemSize.width, grid.itemSize.height);

   // hit center of item [0,0]
   index = MulleGridGetIndexAtPoint( &grid, CGPointMake( 5, 5), 9);
   mulle_printf( "center(0,0): %ld\n", (long) index);

   // hit center of item [1,2] (row 1, col 2)
   // col2 x = 2*(itemW+spacing) + itemW/2
   {
      CGRect  r = MulleGridGetItemRect( &grid, 1, 2);
      index = MulleGridGetIndexAtPoint( &grid, 
                  CGPointMake( CGRectGetMidX( r), CGRectGetMidY( r)), 9);
      mulle_printf( "center(1,2): %ld\n", (long) index);
   }

   // hit center of item [2,2] (last item, index 8)
   {
      CGRect  r = MulleGridGetItemRect( &grid, 2, 2);
      index = MulleGridGetIndexAtPoint( &grid,
                  CGPointMake( CGRectGetMidX( r), CGRectGetMidY( r)), 9);
      mulle_printf( "center(2,2): %ld\n", (long) index);
   }

   // hit in horizontal spacing (between col 0 and col 1)
   {
      CGFloat  gapX = grid.itemSize.width + grid.spacing.width / 2.0f;
      index = MulleGridGetIndexAtPoint( &grid, CGPointMake( gapX, 5), 9);
      mulle_printf( "h_spacing: %ld\n", (long) index);
   }

   // hit in vertical spacing (between row 0 and row 1)
   {
      CGFloat  gapY = grid.itemSize.height + grid.spacing.height / 2.0f;
      index = MulleGridGetIndexAtPoint( &grid, CGPointMake( 5, gapY), 9);
      mulle_printf( "v_spacing: %ld\n", (long) index);
   }

   // hit outside canvas
   index = MulleGridGetIndexAtPoint( &grid, CGPointMake( -1, 5), 9);
   mulle_printf( "outside_left: %ld\n", (long) index);

   index = MulleGridGetIndexAtPoint( &grid, CGPointMake( 5, 200), 9);
   mulle_printf( "outside_bottom: %ld\n", (long) index);

   // hit valid cell but numberOfItems limits it (only 7 items, hit index 8)
   {
      CGRect  r = MulleGridGetItemRect( &grid, 2, 2);
      index = MulleGridGetIndexAtPoint( &grid,
                  CGPointMake( CGRectGetMidX( r), CGRectGetMidY( r)), 7);
      mulle_printf( "beyond_n_items: %ld\n", (long) index);
   }

   // numberOfItems = 0 means no limit
   {
      CGRect  r = MulleGridGetItemRect( &grid, 2, 2);
      index = MulleGridGetIndexAtPoint( &grid,
                  CGPointMake( CGRectGetMidX( r), CGRectGetMidY( r)), 0);
      mulle_printf( "no_limit: %ld\n", (long) index);
   }

   // NULL grid
   index = MulleGridGetIndexAtPoint( NULL, CGPointMake( 5, 5), 9);
   mulle_printf( "null_grid: %ld\n", (long) index);

   //
   // Grid with insets
   //
   MulleGridInit( &grid, CGRectMake( 10, 20, 200, 200),
                  MulleEdgeInsetsMake( 5, 5, 5, 5),
                  CGSizeMake( 40, 40),
                  0,
                  0,
                  CGSizeMake( 10, 10));
   MulleGridConstruct( &grid, 16, &state);

   mulle_printf( "\ninset_grid: %ux%u\n", (unsigned) grid.columns, (unsigned) grid.rows);

   // first item top-left corner should be at (10+5, 20+5) = (15, 25)
   index = MulleGridGetIndexAtPoint( &grid, CGPointMake( 15, 25), 16);
   mulle_printf( "inset_topleft: %ld\n", (long) index);

   // just inside the inset but before first item
   index = MulleGridGetIndexAtPoint( &grid, CGPointMake( 12, 22), 16);
   mulle_printf( "inset_before_item: %ld\n", (long) index);

   // test MulleGridGetRowColumnAtPoint
   {
      struct MulleGridPosition  pos;
      CGRect     r = MulleGridGetItemRect( &grid, 2, 3);
      pos = MulleGridGetPositionAtPoint( &grid,
                  CGPointMake( CGRectGetMidX( r), CGRectGetMidY( r)), 16);
      mulle_printf( "rowcol: row=%ld col=%ld\n",
                     (long) pos.row, (long) pos.column);
   }

   return( 0);
}
