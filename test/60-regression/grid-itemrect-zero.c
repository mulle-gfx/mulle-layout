#include <mulle-layout/mulle-layout.h>

#include <stdio.h>


int   main( void)
{
   struct MulleGrid   grid;
   CGRect             rect;

   // An unconstructed grid has columns=0.
   // MulleGridGetItemRectAtIndex must not divide by zero.
   memset( &grid, 0, sizeof( grid));

   rect = MulleGridGetItemRectAtIndex( &grid, 0);
   mulle_printf( "null: %d\n", CGRectIsNull( rect));

   rect = MulleGridGetItemRectAtIndex( &grid, 5);
   mulle_printf( "null5: %d\n", CGRectIsNull( rect));

   return( 0);
}
