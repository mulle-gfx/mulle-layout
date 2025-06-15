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

   case CGContentModeCenter :
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



CGAffineTransform   CGAffineTransformMakeWithContentMode( CGContentMode contentMode,
                                                          CGRect sourceRect,
                                                          CARotation sourceRotation,
                                                          CGRect destinationRect,
                                                          CARotation destinationRotation)
{
     CGPoint      adjustedTranslate;
     CGFloat      cosTheta;
     CGVector     finalScale;
     CGFloat      radians;
     CGFloat      scale;
     CGPoint      rotatedSourceOffset;
     CGSize       scaledSize;
     CGVector     scaleRatio;
     CGFloat      sinTheta;
     CARotation   totalRotation;
     CGPoint      translate;

     scaleRatio = CGVectorMake( destinationRect.size.width / sourceRect.size.width,
                                destinationRect.size.height / sourceRect.size.height);

     finalScale = CGVectorMake( 1.0, 1.0);
     translate  = CGPointMake( 0.0, 0.0);

     switch( contentMode)
     {
     case CGContentModeScaleToFill:
        finalScale = scaleRatio;
        translate  = destinationRect.origin;
        break;

     case CGContentModeScaleAspectFit:
        scale      = CGFloatMinimum( scaleRatio.dx, scaleRatio.dy);
        finalScale = CGVectorMake( scale, scale);

        scaledSize = CGSizeMake( sourceRect.size.width * scale,
                                 sourceRect.size.height * scale);

        translate = CGPointMake(
           destinationRect.origin.x + (destinationRect.size.width - scaledSize.width) / 2.0,
           destinationRect.origin.y + (destinationRect.size.height - scaledSize.height) / 2.0
        );
        break;

     case CGContentModeScaleAspectFill:
        scale      = CGFloatMaximum( scaleRatio.dx, scaleRatio.dy);
        finalScale = CGVectorMake( scale, scale);

        scaledSize = CGSizeMake( sourceRect.size.width * scale,
                                 sourceRect.size.height * scale);

        translate = CGPointMake(
           destinationRect.origin.x + (destinationRect.size.width - scaledSize.width) / 2.0,
           destinationRect.origin.y + (destinationRect.size.height - scaledSize.height) / 2.0
        );
        break;

     case CGContentModeCenter:
        translate = CGPointMake(
           destinationRect.origin.x + (destinationRect.size.width - sourceRect.size.width) / 2.0,
           destinationRect.origin.y + (destinationRect.size.height - sourceRect.size.height) / 2.0
        );
        break;

     case CGContentModeTop:
        translate = CGPointMake(
           destinationRect.origin.x + (destinationRect.size.width - sourceRect.size.width) / 2.0,
           destinationRect.origin.y
        );
        break;

     case CGContentModeBottom:
        translate = CGPointMake(
           destinationRect.origin.x + (destinationRect.size.width - sourceRect.size.width) / 2.0,
           destinationRect.origin.y + destinationRect.size.height - sourceRect.size.height
        );
        break;

     case CGContentModeLeft:
        translate = CGPointMake(
           destinationRect.origin.x,
           destinationRect.origin.y + (destinationRect.size.height - sourceRect.size.height) / 2.0
        );
        break;

     case CGContentModeRight:
        translate = CGPointMake(
           destinationRect.origin.x + destinationRect.size.width - sourceRect.size.width,
           destinationRect.origin.y + (destinationRect.size.height - sourceRect.size.height) / 2.0
        );
        break;

     case CGContentModeTopLeft:
        translate = destinationRect.origin;
        break;

     case CGContentModeTopRight:
        translate = CGPointMake(
           destinationRect.origin.x + destinationRect.size.width - sourceRect.size.width,
           destinationRect.origin.y
        );
        break;

     case CGContentModeBottomLeft:
        translate = CGPointMake(
           destinationRect.origin.x,
           destinationRect.origin.y + destinationRect.size.height - sourceRect.size.height
        );
        break;

     case CGContentModeBottomRight:
        translate = CGPointMake(
           destinationRect.origin.x + destinationRect.size.width - sourceRect.size.width,
           destinationRect.origin.y + destinationRect.size.height - sourceRect.size.height
        );
        break;
     }

     // Calculate combined rotation (source rotation + destination rotation)
     totalRotation = sourceRotation + destinationRotation;
     radians       = CARotationToRadiansAsRotation( totalRotation);
     cosTheta      = cosf( radians);
     sinTheta      = sinf( radians);

     // Build transform with rotation matrix entries
     // Account for the source rect origin offset with rotation
     rotatedSourceOffset = CGPointMake(
        sourceRect.origin.x * cosTheta - sourceRect.origin.y * sinTheta,
        sourceRect.origin.x * sinTheta + sourceRect.origin.y * cosTheta
     );

     adjustedTranslate = CGPointMake(
        translate.x - rotatedSourceOffset.x * finalScale.dx,
        translate.y - rotatedSourceOffset.y * finalScale.dy
     );

     // Create the combined transform with proper rotation matrix entries
     // Matrix form: [a  b  tx]  where a = cosθ*sx, b = sinθ*sx
     //              [c  d  ty]        c = -sinθ*sy, d = cosθ*sy
     //              [0  0  1 ]        tx, ty = translation
     return( CGAffineTransformMake(
        cosTheta * finalScale.dx,   // a = cos(θ) * scaleX
        sinTheta * finalScale.dx,   // b = sin(θ) * scaleX
        -sinTheta * finalScale.dy,  // c = -sin(θ) * scaleY
        cosTheta * finalScale.dy,   // d = cos(θ) * scaleY
        adjustedTranslate.x,        // tx
        adjustedTranslate.y         // ty
     ));
}

