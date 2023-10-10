#include <mulle-layout/mulle-layout.h>

#include <stdio.h>


static CGSize   sizes[] =
{
   { 0, 0  },
   { 1, 1  },
   { 10, 1 },
   { 1, 10 }
};

#define n_sizes  (sizeof( sizes) / sizeof( sizes[ 0]))


static CGRect   rects[] =
{
   { 0, 0, 0, 0  },
   { 1, 1, 1, 1  },
   { 10, 10, 10, 10 }
};

#define n_rects  (sizeof( rects) / sizeof( rects[ 0]))


static CGContentMode  modes[] =
{
   CGContentModeDefault,
   CGContentModeTop,
   CGContentModeTopRight,
   CGContentModeRight,
   CGContentModeBottomRight,
   CGContentModeBottom,
   CGContentModeBottomLeft,
   CGContentModeLeft,
   CGContentModeTopLeft,
   CGContentModeCenterPoint,
   CGContentModeScaleToFillHorizontal,
   CGContentModeScaleToFillVertical,
   CGContentModeScaleToFill,
   CGContentModeScaleAspectFill,
   CGContentModeScaleAspectFit
};


int   main()
{
   CGSize          size;
   CGRect          rect;
   CGRect          result;
   unsigned int    i, j, k;

   for( i = 0; i < n_sizes; i++)
   {
      size = sizes[ i];
      for( j = 0; j < n_rects; j++)
      {
         rect = rects[ j];
         for( k = 0; k < sizeof( modes) / sizeof( modes[ 0]); k++)
         {
            printf( "CGSizePlaceInRect( { %.1f %.1f }, { %.1f %.1f %.1f %.1f }, %s) -> ", 
                        size.width, size.height,
                        rect.origin.x, rect.origin.y, 
                        rect.size.width, rect.size.height,
                        CGContentModeUTF8String( modes[ k]));
            result = CGSizePlaceInRect( size, rect, modes[ k]);
            printf( "%.1f %.1f %.1f %.1f\n", 
                        result.origin.x, result.origin.y,
                        result.size.width, result.size.height);
         }
      }
   }
}
