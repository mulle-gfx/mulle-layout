#include <mulle-layout/mulle-layout.h>

#include <stdio.h>


static void  CGRectPrint( CGRect rect)
{
   printf( "{ .x = %.2f, .y = %.2f, .w = %.2f, .h = %.2f }",
           rect.origin.x,
           rect.origin.y,
           rect.size.width,
           rect.size.height);
}


// this is a test i wrote, because I forgot that
// CGContentModeScaleAspectFill can exceed the contentRect (by design)

int   main()
{
   CGRect   screen;
   CGRect   contentRect;
   CGRect   rect;
   CGRect   placed;

   screen      = CGRectMake( 0, 0, 10, 10);
   contentRect = CGRectInset( screen, 1, 1);
   rect        = CGRectMake( 4, 3, 3, 4);

   placed      = CGRectPlaceInRect( rect, contentRect, CGContentModeScaleAspectFill|CGContentModeCenterPoint);

   printf( "contentRect : "); CGRectPrint( contentRect); printf( "\n");
   printf( "rect        : "); CGRectPrint( rect);        printf( "\n");
   printf( "placed      : "); CGRectPrint( placed);      printf( "\n");

   return( 0);
}
