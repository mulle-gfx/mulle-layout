//
//  MulleLayoutContent.c
//  mulle-layout
//
//  Copyright (c) 2021 Nat! - Mulle kybernetiK.
//  All rights reserved.
//
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
//
//  Redistributions of source code must retain the above copyright notice, this
//  list of conditions and the following disclaimer.
//
//  Redistributions in binary form must reproduce the above copyright notice,
//  this list of conditions and the following disclaimer in the documentation
//  and/or other materials provided with the distribution.
//
//  Neither the name of Mulle kybernetiK nor the names of its contributors
//  may be used to endorse or promote products derived from this software
//  without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
//  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
//  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
//  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
//  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
//  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
//  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
//  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
//  POSSIBILITY OF SUCH DAMAGE.
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

   if( srcSize.width <= 0.0 || srcSize.height <= 0.0)
      return( 0.0);
   if( dstSize.width <= 0.0 || dstSize.height <= 0.0)
      return( 0.0);

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

   if( srcSize.width <= 0.0 || srcSize.height <= 0.0)
      return( 0.0);
   if( dstSize.width <= 0.0 || dstSize.height <= 0.0)
      return( 0.0);

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
   contentMode &= 0xF;

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

   default :
      return( result);
   }
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
   CGRect       placed;
   CGFloat      radians;
   CGPoint      rotatedSourceOffset;
   CGFloat      sinTheta;
   CARotation   totalRotation;
   CGPoint      translate;

   CGContentModeAssert( contentMode);

   //
   // Use CGSizePlaceInRect to compute the placed rect, which handles all
   // valid CGContentMode combinations (positioning + scaling).
   //
   placed = CGSizePlaceInRect( sourceRect.size, destinationRect, contentMode);

   // Derive scale from source size to placed size
   if( sourceRect.size.width > 0.0 && sourceRect.size.height > 0.0)
   {
      finalScale = CGVectorMake( placed.size.width / sourceRect.size.width,
                                 placed.size.height / sourceRect.size.height);
   }
   else
      finalScale = CGVectorMake( 1.0, 1.0);

   translate = placed.origin;

   // Calculate combined rotation (source rotation + destination rotation)
   totalRotation = sourceRotation + destinationRotation;
   radians       = CARotationToRadiansAsRotation( totalRotation);
   cosTheta      = cosf( radians);
   sinTheta      = sinf( radians);

   // Account for the source rect origin offset with rotation
   rotatedSourceOffset = CGPointMake(
      sourceRect.origin.x * cosTheta - sourceRect.origin.y * sinTheta,
      sourceRect.origin.x * sinTheta + sourceRect.origin.y * cosTheta
   );

   adjustedTranslate = CGPointMake(
      translate.x - rotatedSourceOffset.x * finalScale.dx,
      translate.y - rotatedSourceOffset.y * finalScale.dy
   );

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

