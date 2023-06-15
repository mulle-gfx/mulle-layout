//
// Copyright (c) 2021 Nat!, Mulle kybernetiK
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
   CGContentModeScaleToFill,
   CGContentModeScaleAspectFit,
   CGContentModeScaleAspectFill,

//   CGContentModeRedraw,  // not supported

   // circular arrangement, convenient for lazy animations
   CGContentModeTop,
   CGContentModeTopRight,
   CGContentModeRight,
   CGContentModeBottomRight,
   CGContentModeBottom,
   CGContentModeBottomLeft,
   CGContentModeLeft,
   CGContentModeTopLeft,

   // it's CenterPoint to disinguish from Center, which is used for the
   // Y axis (and Middle for the X axis)
   CGContentModeCenterPoint
};
#define N_CGContentModes 12

typedef uintptr_t    CGContentMode;


extern char    *CGContentModeNames[ N_CGContentModes];
char           *CGContentModeUTF8String( CGContentMode mode);
CGContentMode  CGContentModeParseCString( char *s);


static inline int   CGContentModeIsScaling( CGContentMode mode)
{
   return( mode >= CGContentModeScaleToFill &&
           mode <= CGContentModeScaleAspectFill);
}


CGFloat   CGSizeGetAspectFitFactor( CGSize dstSize, CGSize srcSize);
CGFloat   CGSizeGetAspectFillFactor( CGSize dstSize, CGSize srcSize);

CGSize  CGSizeAspectFitToSize( CGSize size, CGSize containerSize);
CGSize  CGSizeAspectFillToSize( CGSize size, CGSize containerSize);


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
CGRect   CGSizePlaceInRect( CGSize size,
                            CGRect rect,
                            CGContentMode contentMode);

static inline CGRect   CGRectPlaceInRect( CGRect r1,
                                          CGRect r2,
                                          CGContentMode contentMode)
{
   return( CGSizePlaceInRect( r1.size, r2, contentMode));
}


static inline CGPoint   CGSizeGetCenterPointInRect( CGSize size, CGRect rect)
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
   rect.origin = CGSizeGetCenterPointInRect( rect.size, bounds);
   return( rect);
}


static inline CGRect   CGSizeCenterInRect( CGSize size, CGRect bounds)
{
   CGRect   rect;

   rect.origin = CGSizeGetCenterPointInRect( size, bounds);
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


#endif
