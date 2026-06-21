#include <mulle-layout/mulle-layout.h>


#include <stdio.h>


void   CGRectPrint( CGRect rect)
{
   mulle_printf( "{ %.2f %.2f %.2f %.2f }",
            rect.origin.x,
            rect.origin.y,
            rect.size.width,
            rect.size.height);
}


static void   _test( CGRect rect1, CGRect rect2)
{
   CGRect         rects[ 4];
   unsigned int   i, n;

   n = CGRectSubdivide( rect1, rect2, rects);

   mulle_printf( "a=");
   CGRectPrint( rect1);
   mulle_printf( " b=");
   CGRectPrint( rect2);
   for( i = 0; i < n; i++)
   {
      mulle_printf( " s[%u]=", i);
      CGRectPrint( rects[ i]);
   }
   mulle_printf( "\n");
}


static void   test( CGRect rect1, CGRect rect2)
{
   _test( rect1, rect2);
   _test( rect2, rect1);
}


int   main( void)
{
#if 1
   // these just don't intersect
   test( CGRectMake( 1, 1, 2, 2), CGRectMake( 0, 0, 0, 0));
   test( CGRectMake( 1, 1, 2, 2), CGRectMake( 4, 4, 2, 2));

   // these completely overlap
   test( CGRectMake( 0, 0, 1, 1), CGRectMake( 0, 0, 1, 1));

   // cover of some part
   test( CGRectMake( 0, 0, 2, 2), CGRectMake(  1,  0, 2, 2));
   test( CGRectMake( 0, 0, 2, 2), CGRectMake(  0,  1, 2, 2));
   test( CGRectMake( 0, 0, 2, 2), CGRectMake( -1,  0, 2, 2));
   test( CGRectMake( 0, 0, 2, 2), CGRectMake(  0, -1, 2, 2));

   // cover in center
   test( CGRectMake( 0, 0, 3, 3), CGRectMake(  1, 1, 1, 1));

   // obscure in v
   test( CGRectMake( 0, 0, 3, 3), CGRectMake(  1, -1, 1, 5));

   // obscure in h
   test( CGRectMake( 0, 0, 3, 3), CGRectMake(  -1, 1, 5, 1));
#endif

   _test( CGRectMake( 0.00, 196.00, 300.00, 4.00),
          CGRectMake( -23.00, 62.67, 340.00, 66.67));

   return( 0);
}

