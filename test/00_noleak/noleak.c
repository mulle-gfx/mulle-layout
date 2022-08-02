#include <mulle-layout/mulle-layout.h>


int  main( int argc, char **argv)
{
   struct MulleLayout  layout;

   MulleLayoutInit( &layout, CGRectMake( 0, 0, 100, 100), MulleEdgeInsetsZero);
   MulleLayoutDone( &layout);
   return( 0);
}

