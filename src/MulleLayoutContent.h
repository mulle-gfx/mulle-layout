//
//  MulleLayoutContent.h
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
#ifndef mulle_layout_content_h__
#define mulle_layout_content_h__

#include "include.h"

#include <stdint.h>


//
// Layout content is concerned about placing a rectangle of some size
// inside another rectangle of known size.
//
//  .-------------.
//  |             |
//  |    .---.    |
//  |    |   |    |
//  |    '---'    |
//  |             |
//  '-------------'
//

// TODO: make    CADirectionUp down ?


enum CGContentMode
{
   CGContentModeDefault         = 0x0, // the default is CGContentModeScaleToFill

   //   CGContentModeRedraw,  // not supported
   // you can _not_ OR CGContentModeTop and CGContentModeBottom to get "middle"
   //
   CGContentModeTop             = CADirectionTop,
   CGContentModeTopRight        = CADirectionTop | CADirectionRight,
   CGContentModeRight           = CADirectionRight,
   CGContentModeBottomRight     = CADirectionBottom | CADirectionRight,
   CGContentModeBottom          = CADirectionBottom,
   CGContentModeBottomLeft      = CADirectionBottom | CADirectionLeft,
   CGContentModeLeft            = CADirectionLeft,
   CGContentModeTopLeft         = CADirectionTop | CADirectionLeft,
   CGContentModeCenterPoint     = CADirectionTop
                                 | CADirectionLeft
                                 | CADirectionBottom
                                 | CADirectionRight,

   // CGContentModeScaleToFillHorizontal and CGContentModeScaleToFillVertical
   // can be mixed with above bits
   CGContentModeScaleToFillHorizontal    = 0x10,
   CGContentModeScaleToFillVertical      = 0x20,
   CGContentModeScaleToFill              = 0x30,  // same as CGContentModeDefault
   CGContentModeScaleAspectFill          = 0x40,
   CGContentModeScaleAspectFit           = 0x80,
   CGContentModeScaleAspect              = 0xC0   // same as CGContentModeScaleAspectFit
};

typedef uintptr_t    CGContentMode;


// backwards compatibility
#define CGContentModeCenter      CGContentModeCenterPoint



static inline int   CGContentModeIsScaling( CGContentMode mode)
{
   return( mode == CGContentModeDefault || mode >= CGContentModeScaleToFillHorizontal);
}


static inline int   CGContentModeIsValid( CGContentMode mode)
{
   if( (mode & 0xF) != CGContentModeCenterPoint)
   {
      if( (mode & (CGContentModeRight|CGContentModeLeft)) == (CGContentModeRight|CGContentModeLeft))
         return( 0);
      if( (mode & (CGContentModeTop|CGContentModeBottom)) == (CGContentModeTop|CGContentModeBottom))
         return( 0);
   }

   if( (mode & (CGContentModeScaleAspectFit|CGContentModeScaleAspectFill)) && (mode & CGContentModeScaleToFill))
      return( 0);
   if( (mode & (CGContentModeScaleAspectFit|CGContentModeScaleAspectFill)) == (CGContentModeScaleAspectFit|CGContentModeScaleAspectFill))
      return( 0);
   return( 1);
}


static inline void   CGContentModeAssert( CGContentMode mode)
{
   assert( CGContentModeIsValid( mode));
}



MULLE__LAYOUT_GLOBAL
CGFloat   CGSizeGetAspectFitFactor( CGSize dstSize, CGSize srcSize);

MULLE__LAYOUT_GLOBAL
CGFloat   CGSizeGetAspectFillFactor( CGSize dstSize, CGSize srcSize);

MULLE__LAYOUT_GLOBAL
CGSize  CGSizeAspectFitToSize( CGSize size, CGSize containerSize);

MULLE__LAYOUT_GLOBAL
CGSize  CGSizeAspectFillToSize( CGSize size, CGSize containerSize);


MULLE__LAYOUT_GLOBAL
CGRect    CGSizePlaceInRectWithAspectCorrection( CGSize size, CGRect rect, int fill);


static inline CGRect  CGSizePlaceInRectWithAspectFit( CGSize size, CGRect rect)
{
   return( CGSizePlaceInRectWithAspectCorrection( size, rect, 0));
}


static inline CGRect  CGSizePlaceInRectWithAspectFill( CGSize size, CGRect rect)
{
   return( CGSizePlaceInRectWithAspectCorrection( size, rect, 1));
}


//
// a rect of a certain size is positioned and possibly stretched into a
// given frame using contentMode. If the size is larger than the frame
// it may overlap (or shrink)
//
MULLE__LAYOUT_GLOBAL
CGRect   CGSizePlaceInRect( CGSize size, CGRect r, CGContentMode contentMode);


// here we only look at the lower 4 bits, a 0 means no change in 'r'
MULLE__LAYOUT_GLOBAL
CGRect   CGRectPositionInRect( CGRect r1, CGRect r2, CGContentMode contentMode);

// place r1 in r2, preserve x or y if just filling in one dimension
static inline
CGRect   CGRectPlaceInRect( CGRect r1, CGRect r2, CGContentMode contentMode)
{
   CGRect   result;

   result = CGSizePlaceInRect( r1.size, r2, contentMode);
   switch( contentMode)
   {
   case CGContentModeScaleToFillHorizontal :
      result.origin.y  = r1.origin.y;
      break;

   case CGContentModeScaleToFillVertical :
      result.origin.x  = r1.origin.x;
      break;
   }
   return( result);
}


static inline CGPoint   CGSizeGetCenterInRect( CGSize size, CGRect rect)
{
   CGPoint   center;

   center.x = CGRectGetMidX( rect) - size.width / 2.0;
   center.y = CGRectGetMidY( rect) - size.height / 2.0;

   return( center);
}

//void   CGRectSplitVertically( CGRect frame, CGRect *frame1, CGRect *frame2)
//void   CGRectSplitHorizontally( CGRect frame, CGRect *frame1, CGRect *frame2);

static inline CGRect   CGRectCenterInRect( CGRect rect, CGRect bounds)
{
   rect.origin = CGSizeGetCenterInRect( rect.size, bounds);
   return( rect);
}


static inline CGRect   CGSizeCenterInRect( CGSize size, CGRect bounds)
{
   CGRect   rect;

   rect.origin = CGSizeGetCenterInRect( size, bounds);
   rect.size   = size;
   return( rect);
}


static inline CGRect   CGRectCenterAtPoint( CGRect rect, CGPoint point)
{
   rect.origin.x = point.x - rect.size.width / 2.0;
   rect.origin.y = point.y - rect.size.height / 2.0;
   return( rect);
}


// Find the size of the square that fits into rect.
//
//  .-------------.
//  |    .---.    |
//  |    |   |    |
//  |    '---'    |
//  '-------------'
//
static inline CGSize   CGRectGetLargestSquareSize( CGRect rect)
{
   CGSize   square;

   if( rect.size.width > rect.size.height)
      square.width = rect.size.height;
   else
      square.width = rect.size.width;
   square.height   = square.width;
   return( square);
}


// Mirror a rect within a rect, the rect contents aren't flipped,
// (which would happen if the size becomes negative).
//
//  .-----------.     .-----------.
//  |    .----X |     |           |
//  |    |rect| |     | Z----.    |  
//  |    Y----' |     | |rect|    |  
//  |           |     | '----'    |
//  '-----------'     '-----------'
//  container        container
static inline CGRect   CGRectMirrorInRectH( CGRect rect, CGRect container)
{
   // flip (X->Z): see pic
   rect.origin.x = CGRectGetMinX( container) + (CGRectGetMaxX( container) - CGRectGetMaxX( rect));
   return( rect);
}


static inline CGPoint   CGPointMirrorInRectH( CGPoint origin, CGRect container)
{
   // flip (X->Z): see pic
   origin.x = CGRectGetMinX( container) + (CGRectGetMaxX( container) - origin.x);
   return( origin);
}


static inline CGRect   CGRectMirrorInRectV( CGRect rect, CGRect container)
{
   // flip (Y->Z)
   rect.origin.y = CGRectGetMinY( container) + (CGRectGetMaxY( container) - CGRectGetMaxY( rect));
   return( rect);
}


static inline CGPoint   CGPointMirrorInRectV( CGPoint origin, CGRect container)
{
   // flip (Y->Z)
   origin.y = CGRectGetMinY( container) + (CGRectGetMaxY( container) - origin.y);
   return( origin);
}


MULLE__LAYOUT_GLOBAL
CGAffineTransform   CGAffineTransformMakeWithContentMode( CGContentMode contentMode,
                                                          CGRect sourceRect,
                                                          CARotation sourceRotation,
                                                          CGRect destinationRect,
                                                          CARotation destinationRotation);

//
// char  *CGContentModeUTF8String( CGContentMode mode);
//
// Defined downstream in MulleUIBase (CGContentMode+UTF8String.h) since it
// depends on MulleObjC_asprintf for compound mode string formatting.
//

#endif
