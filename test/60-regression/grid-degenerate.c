#include <mulle-layout/mulle-layout.h>

#include <stdio.h>
#include <math.h>


int   main( void)
{
   struct MulleGrid   grid;
   uintptr_t          state;

   //
   // Bug 2: insets larger than canvas produce negative dimensions.
   // The search must not pick a negative itemSize as "best".
   //
   MulleGridInit( &grid, CGRectMake( 0, 0, 100, 100), MulleEdgeInsetsMake( 60, 60, 60, 60),
                         CGSizeMake( 0, 0), 0, 0, CGSizeMake( 10, 10));
   MulleGridConstruct( &grid, 4, &state);

   mulle_printf( "overinset: rows=%tu columns=%tu itemW=%.1f itemH=%.1f\n",
                  grid.rows, grid.columns,
                  grid.itemSize.width, grid.itemSize.height);

   //
   // Bug 3: INFINITY canvas should not produce NaN/UB via sqrt(NaN).
   //
   MulleGridInit( &grid, CGRectMake( 0, 0, INFINITY, 100), MulleEdgeInsetsZero,
                         CGSizeMake( 0, 0), 0, 0, CGSizeMake( 0, 0));
   MulleGridConstruct( &grid, 4, &state);

   mulle_printf( "inf-canvas: rows=%tu columns=%tu\n",
                  grid.rows, grid.columns);

   //
   // Bug 4: negative insetCanvas.size with specified itemSize should
   // produce columns=0 (not wrap around via UB cast).
   //
   MulleGridInit( &grid, CGRectMake( 0, 0, 50, 50), MulleEdgeInsetsMake( 40, 40, 40, 40),
                         CGSizeMake( 30, 30), 0, 0, CGSizeMake( 0, 0));
   MulleGridConstruct( &grid, 4, &state);

   mulle_printf( "neg-floor: rows=%tu columns=%tu\n",
                  grid.rows, grid.columns);

   return( 0);
}
