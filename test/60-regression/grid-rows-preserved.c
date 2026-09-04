#include <mulle-layout/mulle-layout.h>

#include <stdio.h>


int   main()
{
   struct MulleGrid   grid;
   uintptr_t          state;

   // Specify 4 rows but leave columns=0 (to be computed), with 10 items.
   // Since rows were caller-specified and columns are computed,
   // the grid should honor the caller's row count (4).
   // columns = ceil(10/4) = 3
   MulleGridInit( &grid, CGRectMake( 0, 0, 400, 300), MulleEdgeInsetsZero,
                         CGSizeMake( 50, 50), 4, 0, CGSizeMake( 0, 0));
   MulleGridConstruct( &grid, 10, &state);

   mulle_printf( "rows=%tu\n", grid.rows);
   mulle_printf( "columns=%tu\n", grid.columns);

   return( 0);
}
