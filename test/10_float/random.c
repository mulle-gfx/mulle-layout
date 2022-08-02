#include <mulle-layout/mulle-layout.h>

#include <stdlib.h>


//static inline CGFloat   CGFloatMakeRandom( void)
//{
//   return( (CGFloat) (rand() * 1.0 / RAND_MAX));
//}
//
//
int   main()
{
   long      i;
   long      hi, lo;
   CGFloat   x;

   /* check that we get only values from 0.0 to 1.0
      and that the area is covered
   */
   hi = 0;
   lo = 0;
   for( i = 0; i < 1000000; i++)
   {
      x = CGFloatMakeRandom();
      if( x < 0.01)
         ++lo;
      if( x > 0.99)
         ++hi;
      if( x < 0.0)
         return(1);
      if( x > 1.0)
         return( 2);
   }
   if( ! lo)
      return( 3);
   if( ! hi)
      return( 4);
   return( 0);
}
