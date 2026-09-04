#include <mulle-layout/mulle-layout.h>

#include <stdio.h>


//
// This must compile. If MulleEdgeInsetsZero is not a compile-time
// constant, the compiler will error out here.
// Bug: MulleEdgeInsetsZero expands to MulleEdgeInsetsMake(0,0,0,0)
// which is not a constant expression in C.
//
static MulleEdgeInsets   s_insets = MulleEdgeInsetsZero;


int   main()
{
   mulle_printf( "top=%.1f left=%.1f bottom=%.1f right=%.1f\n",
                  s_insets.top, s_insets.left,
                  s_insets.bottom, s_insets.right);

   return( 0);
}
