//
// Copyright (c) 2021 Nat!, Mulle kybernetiK
//
#include "MulleLayoutContent.h"

#include "include-private.h"

#include <stdlib.h>
#include <string.h>


//
// +++++
// +++++  ++++
//        ++++
//
// this will produce the amount to scale srcSize in both dimensions
// so that everything remains visible in the same aspect
//
CGFloat   CGSizeGetAspectFitFactor( CGSize dstSize, CGSize srcSize)
{
   CGFloat   srcAspectScale;
   CGFloat   dstAspectScale;

   srcAspectScale = srcSize.width / srcSize.height;
   dstAspectScale = dstSize.width / dstSize.height;

   if( srcAspectScale < dstAspectScale)
      return( dstSize.height / srcSize.height);
   return( dstSize.width / srcSize.width);
}


CGFloat   CGSizeGetAspectFillFactor( CGSize dstSize, CGSize srcSize)
{
   CGFloat   srcAspectScale;
   CGFloat   dstAspectScale;

   srcAspectScale = srcSize.width / srcSize.height;
   dstAspectScale = dstSize.width / dstSize.height;

   if( srcAspectScale > dstAspectScale)
      return( dstSize.height / srcSize.height);
   return( dstSize.width / srcSize.width);
}


CGSize  CGSizeAspectFitToSize( CGSize size, CGSize containerSize)
{
   CGFloat  aspectScale;

   if( size.height == 0.0 || size.width == 0.0  || containerSize.height == 0.0)
      return( CGSizeZero);

   aspectScale = CGSizeGetAspectFitFactor(containerSize, size);

   return( CGSizeMake( size.width * aspectScale, size.height * aspectScale));
}


CGSize  CGSizeAspectFillToSize( CGSize size, CGSize containerSize)
{
   CGFloat  aspectScale;

   if( size.height == 0.0 || size.width == 0.0  || containerSize.height == 0.0)
      return( CGSizeZero);

   aspectScale = CGSizeGetAspectFillFactor( containerSize, size);

   return( CGSizeMake( size.width * aspectScale, size.height * aspectScale));
}



CGRect  CGSizePlaceInRectWithAspectCorrection( CGSize size,
                                               CGRect rect,
                                               int fill)
{
   CGFloat  aspectScale;
   CGRect   result;

   if( size.height == 0.0 || size.width == 0.0  || rect.size.height == 0.0)
      return( CGRectZero);

   if( fill)
      aspectScale = CGSizeGetAspectFillFactor( rect.size, size);
   else
      aspectScale = CGSizeGetAspectFitFactor( rect.size, size);

   result.size.width  = size.width * aspectScale;
   result.size.height = size.height * aspectScale;
   // now center
   result.origin.x = CGRectGetMidX( rect) - result.size.width / 2.0;
   result.origin.y = CGRectGetMidY( rect) - result.size.height / 2.0;
   return( result);
}


CGRect   CGSizePlaceInRect( CGSize size,
                            CGRect rect,
                            CGContentMode contentMode)
{
   CGRect   result;

   switch( contentMode)
   {
   case CGContentModeScaleToFill :
      return( rect);

   case CGContentModeScaleAspectFit :
      return( CGSizePlaceInRectWithAspectCorrection( size, rect, 0));

   case CGContentModeScaleAspectFill :
      return( CGSizePlaceInRectWithAspectCorrection( size, rect, 1));

   case CGContentModeCenterPoint :
      result.origin.x = CGRectGetMidX( rect) - size.width / 2.0;
      result.origin.y = CGRectGetMidY( rect) - size.height / 2.0;
      result.size     = size;
      return( result);

   case CGContentModeTop :
      result.origin.x = CGRectGetMidX( rect) - size.width / 2.0;
      result.origin.y = rect.origin.y;
      result.size     = size;
      return( result);

   case CGContentModeBottom :
      result.origin.x = CGRectGetMidX( rect) - size.width / 2.0;
      result.origin.y = CGRectGetMaxY( rect) - size.height;
      result.size     = size;
      return( result);

   case CGContentModeLeft :
      result.origin.x = rect.origin.x;
      result.origin.y = CGRectGetMidY( rect) - size.height / 2.0;
      result.size     = size;
      return( result);

   case CGContentModeRight :
      result.origin.x = CGRectGetMaxX( rect) - size.width;
      result.origin.y = CGRectGetMidY( rect) - size.height / 2.0;
      result.size     = size;
      return( result);

   case CGContentModeTopLeft :
      result.origin = rect.origin;
      result.size   = size;
      return( result);

   case CGContentModeTopRight :
      result.origin.x = CGRectGetMaxX( rect) - size.width;
      result.origin.y = rect.origin.y;
      result.size     = size;
      return( result);

   case CGContentModeBottomLeft :
      result.origin.x = rect.origin.x;
      result.origin.y = CGRectGetMaxY( rect) - size.height;
      result.size     = size;
      return( result);

   case CGContentModeBottomRight :
      result.origin.x = CGRectGetMaxX( rect) - size.width;
      result.origin.y = CGRectGetMaxY( rect) - size.height;
      result.size     = size;
      return( result);
   }
   abort();
}


char  *CGContentModeNames[] =
{
   "ScaleToFill",
   "ScaleAspectFit",
   "ScaleAspectFill",
//   "CGContentModeRedraw",

   "Top",
   "TopRight",
   "Right",
   "BottomRight",
   "Bottom",
   "BottomLeft",
   "Left",
   "TopLeft",

   "Center"
};


char  *CGContentModeUTF8String( CGContentMode mode)
{
   assert( mode >= CGContentModeScaleToFill && mode <= CGContentModeCenterPoint);
   return( CGContentModeNames[ mode]);
}


CGContentMode  CGContentModeParseCString( char *s)
{
   char   **sentinel;
   char   **p;
   char   *prefix;

   prefix = strstr( s, "ContentMode");
   if( prefix)
   {
      s = &s[ prefix - s];
      s = &s[ strlen( "ContentMode")];
   }

   p        = CGContentModeNames;
   sentinel = &p[ N_CGContentModes];
   while( p < sentinel)
   {
      if( ! strcmp( s, *p))
         return( (int) (p - CGContentModeNames));
      ++p;
   }
   return( -1);
}
