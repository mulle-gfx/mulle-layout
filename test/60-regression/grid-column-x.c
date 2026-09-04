#include <mulle-layout/mulle-layout.h>

#include <stdio.h>


int   main()
{
   struct MulleGrid   grid;
   uintptr_t          state;

   // Canvas at (10,77), insets 5 all round, 50x50 items, spacing 10
   MulleGridInit( &grid, CGRectMake( 10, 77, 400, 300),
                         MulleEdgeInsetsMake( 5, 5, 5, 5),
                         CGSizeMake( 50, 50), 0, 0, CGSizeMake( 10, 10));
   MulleGridConstruct( &grid, 12, &state);

   // Column 0 x should be canvas.origin.x (10), not canvas.origin.y (77)
   mulle_printf( "ColumnX(0)=%.1f\n", MulleGridGetColumnX( &grid, 0));
   // Row 0 y should be canvas.origin.y (77)
   mulle_printf( "RowY(0)=%.1f\n", MulleGridGetRowY( &grid, 0));

   return( 0);
}
