#include <mulle-layout/mulle-layout.h>

#include <stdio.h>


int   main()
{
   struct MulleLayout   layout;
   CGRect              result;

   //
   // Frame 50 wide. margins left=40 right=40 means usable width = -30.
   // AddRowWithHeight should detect this and return CGRectNull,
   // not a rect with negative width.
   //
   MulleLayoutInit( &layout, CGRectMake( 0, 0, 50, 100), MulleEdgeInsetsZero);

   result = MulleLayoutAddRowWithHeight( &layout, 10,
                                         MulleEdgeInsetsMake( 0, 40, 0, 40));

   mulle_printf( "null=%d\n", CGRectIsNull( result) ? 1 : 0);

   return( 0);
}
