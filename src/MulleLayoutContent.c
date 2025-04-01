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

   if( CGFloatIsZero( size.height) || CGFloatIsZero( size.width) || CGFloatIsZero( containerSize.height))
      return( CGSizeZero);

   aspectScale = CGSizeGetAspectFitFactor( containerSize, size);

   return( CGSizeMake( size.width * aspectScale, size.height * aspectScale));
}


CGSize  CGSizeAspectFillToSize( CGSize size, CGSize containerSize)
{
   CGFloat  aspectScale;

   if( CGFloatIsZero( size.height) || CGFloatIsZero( size.width) || CGFloatIsZero( containerSize.height))
      return( CGSizeZero);

   aspectScale = CGSizeGetAspectFillFactor( containerSize, size);

   return( CGSizeMake( size.width * aspectScale, size.height * aspectScale));
}



CGRect  CGSizePlaceInRectWithAspectCorrection( CGSize size, CGRect r, int fill)
{
   CGFloat  aspectScale;
   CGRect   result;

   if( CGFloatIsZero( size.height) || CGFloatIsZero( size.width) || CGFloatIsZero( r.size.width))
      return( CGRectZero);

   if( fill)
      aspectScale = CGSizeGetAspectFillFactor( r.size, size);
   else
      aspectScale = CGSizeGetAspectFitFactor( r.size, size);

   result.size.width  = size.width * aspectScale;
   result.size.height = size.height * aspectScale;
   // now center
   result.origin.x    = CGRectGetMidX( r) - result.size.width / 2.0;
   result.origin.y    = CGRectGetMidY( r) - result.size.height / 2.0;
   return( result);
}


CGRect   CGRectPositionInRect( CGRect result, CGRect r, CGContentMode contentMode)
{
   assert( contentMode < 0x10);

   switch( contentMode)
   {
   case CGContentModeDefault :
      return( result);

   case CGContentModeCenterPoint :
      result.origin.x = CGRectGetMidX( r) - result.size.width / 2.0;
      result.origin.y = CGRectGetMidY( r) - result.size.height / 2.0;
      return( result);

   case CGContentModeTop :
      result.origin.x = CGRectGetMidX( r) - result.size.width / 2.0;
      result.origin.y = r.origin.y;
      return( result);

   case CGContentModeBottom :
      result.origin.x = CGRectGetMidX( r) - result.size.width / 2.0;
      result.origin.y = CGRectGetMaxY( r) - result.size.height;
      return( result);

   case CGContentModeLeft :
      result.origin.x = r.origin.x;
      result.origin.y = CGRectGetMidY( r) - result.size.height / 2.0;
      return( result);

   case CGContentModeRight :
      result.origin.x = CGRectGetMaxX( r) - result.size.width;
      result.origin.y = CGRectGetMidY( r) - result.size.height / 2.0;
      return( result);

   case CGContentModeTopLeft :
      result.origin = r.origin;
      return( result);

   case CGContentModeTopRight :
      result.origin.x = CGRectGetMaxX( r) - result.size.width;
      result.origin.y = r.origin.y;
      return( result);

   case CGContentModeBottomLeft :
      result.origin.x = r.origin.x;
      result.origin.y = CGRectGetMaxY( r) - result.size.height;
      return( result);

   case CGContentModeBottomRight :
      result.origin.x = CGRectGetMaxX( r) - result.size.width;
      result.origin.y = CGRectGetMaxY( r) - result.size.height;
      return( result);
   }
   abort();
}


CGRect   CGSizePlaceInRect( CGSize size, CGRect r, CGContentMode contentMode)
{
   CGRect   result;

   // default
   contentMode = contentMode ? contentMode : CGContentModeScaleToFill;

   //
   // do fill first
   //
   switch( contentMode & (CGContentModeScaleAspectFit|CGContentModeScaleAspectFill))
   {
   case CGContentModeScaleAspectFit|CGContentModeScaleAspectFill :
   case CGContentModeScaleAspectFit :
      // you can't mix fill with aspect, aspect overrides
      assert( ! (contentMode & (CGContentModeScaleToFillVertical|CGContentModeScaleToFillHorizontal)));
      result = CGSizePlaceInRectWithAspectCorrection( size, r, 0);
      break;

   case CGContentModeScaleAspectFill :
      assert( ! (contentMode & (CGContentModeScaleToFillVertical|CGContentModeScaleToFillHorizontal)));
      result = CGSizePlaceInRectWithAspectCorrection( size, r, 1);
      break;

   default :
      result.origin      = r.origin;
      result.size.width  = (contentMode & CGContentModeScaleToFillHorizontal) ?  r.size.width : size.width;
      result.size.height = (contentMode & CGContentModeScaleToFillVertical) ?  r.size.height : size.height;
      break;
   }

   return( CGRectPositionInRect( result, r, contentMode & 0xF));
}


// CGContentMode  CGContentModeParseCString( char *s)
// {
//    char   **sentinel;
//    char   **p;
//    char   *prefix;
//
//    prefix = strstr( s, "ContentMode");
//    if( prefix)
//    {
//       s = &s[ prefix - s];
//       s = &s[ strlen( "ContentMode")];
//    }
//
//    p        = CGContentModeNames;
//    sentinel = &p[ S_CGContentModes];
//    while( p < sentinel)
//    {
//       if( ! strcmp( s, *p))
//          return( (int) (p - CGContentModeNames));
//       ++p;
//    }
//    return( -1);
// }
