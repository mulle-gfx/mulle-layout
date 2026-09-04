#include <mulle-layout/mulle-layout.h>

#include <stdio.h>


int   main()
{
   CGRect   rect;
   CGRect   result;

   rect = CGRectMake( 100, 200, 50, 20);

   // CGRectAlignLeft should set origin.x to the given value
   result = CGRectAlignLeft( rect, 0);
   mulle_printf( "AlignLeft x=%.1f y=%.1f w=%.1f h=%.1f\n",
                  result.origin.x, result.origin.y,
                  result.size.width, result.size.height);

   // CGRectAlignRight should set origin.x = value - width
   result = CGRectAlignRight( rect, 200);
   mulle_printf( "AlignRight x=%.1f y=%.1f w=%.1f h=%.1f\n",
                  result.origin.x, result.origin.y,
                  result.size.width, result.size.height);

   return( 0);
}
