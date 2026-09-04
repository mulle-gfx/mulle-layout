//
//  MulleEdgeInsets.h
//  mulle-layout
//
//  Copyright (c) 2020 Nat! - Mulle kybernetiK.
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
#ifndef mulle_edge_insets_h__
#define mulle_edge_insets_h__

#include "include.h"

#include <string.h>


//
// this field order actually produces slightly better code for
// MulleEdgeInsetsInsetRect (gcc 11. x86_64 -O3)
//
typedef struct MulleEdgeInsets
{
   CGFloat   left;
   CGFloat   top;
   CGFloat   right;
   CGFloat   bottom;
} MulleEdgeInsets;


//
// Bare initializer list (no compound literal cast) so callers can cast it to
// either MulleEdgeInsets or UIEdgeInsets. Uses designated initializers so the
// field order above can change without silently transposing values, and
// sanitizes -0.0 away because MulleEdgeInsetsEqualToEdgeInsets is memcmp based.
//
#define MulleEdgeInsetsData( t, l, b, r)   \
   {                                       \
      .left   = (l) + 0.0f,                \
      .top    = (t) + 0.0f,                \
      .right  = (r) + 0.0f,                \
      .bottom = (b) + 0.0f                 \
   }

#define MulleEdgeInsetsZero   ((MulleEdgeInsets) MulleEdgeInsetsData( 0.0f, 0.0f, 0.0f, 0.0f))


//
// TLBR (counter clockwise) isn't the worst order imaginable, that would be IMO
// RTBL, but it's a second or third. We use it because that's how Apple did it.
// Probably imagining this as a top/left and a bottom/right point, but it makes
// little sense with insets to me.
//
static inline MulleEdgeInsets   MulleEdgeInsetsMake( CGFloat top,
                                                     CGFloat left,
                                                     CGFloat bottom,
                                                     CGFloat right)
{
   assert( ! isnan( top));
   assert( ! isnan( left));
   assert( ! isnan( bottom));
   assert( ! isnan( right));

   return( (MulleEdgeInsets){ .top    = top + 0.0f,
                              .left   = left + 0.0f,
                              .bottom = bottom + 0.0f,
                              .right  = right + 0.0f });
}

static inline MulleEdgeInsets   MulleEdgeInsetsMakeTLBR( CGFloat top,
                                                         CGFloat left,
                                                         CGFloat bottom,
                                                         CGFloat right)
{
   return( MulleEdgeInsetsMake( top, left, bottom, right));
}


static inline MulleEdgeInsets   MulleEdgeInsetsMakeCounterClockwise( CGFloat top,
                                                                     CGFloat left,
                                                                     CGFloat bottom,
                                                                     CGFloat right)
{
   return( MulleEdgeInsetsMake( top, left, bottom, right));
}


// this is probably the most common way, looking like CSS dox
static inline MulleEdgeInsets   MulleEdgeInsetsMakeClockwise( CGFloat top,
                                                              CGFloat right,
                                                              CGFloat bottom,
                                                              CGFloat left)
{
   return( MulleEdgeInsetsMake( top, left, bottom, right));
}

// this keeps related values together best
static inline MulleEdgeInsets   MulleEdgeInsetsMakeHV( CGFloat left,
                                                       CGFloat right,
                                                       CGFloat top,
                                                       CGFloat bottom)
{
   return( MulleEdgeInsetsMake( top, left, bottom, right));
}

// order of atari joystick bits, should have been made into law really
// this is little endian. This could also be MulleEdgeInsetsMakeVH
static inline MulleEdgeInsets   MulleEdgeInsetsMakeAtariLE( CGFloat top,
                                                            CGFloat bottom,
                                                            CGFloat left,
                                                            CGFloat right)
{
   return( MulleEdgeInsetsMake( top, left, bottom, right));
}

// this is big endian, which is my actual mind model
static inline MulleEdgeInsets   MulleEdgeInsetsMakeAtariBE( CGFloat right,
                                                            CGFloat left,
                                                            CGFloat bottom,
                                                            CGFloat top)
{
   return( MulleEdgeInsetsMake( top, left, bottom, right));
}



// This is what feels natural to me, looking at a rectangle and knowing its
// not HV :) This order also performs the best. There is probably a connection
// there...
static inline MulleEdgeInsets   MulleEdgeInsetsMakeNatural( CGFloat left,
                                                            CGFloat top,
                                                            CGFloat right,
                                                            CGFloat bottom)
{
   return( MulleEdgeInsetsMake( top, left, bottom, right));
}


//
// Left Right only is fairly often used, avoids T/L coordinates
//
static inline MulleEdgeInsets   MulleEdgeInsetsMakeLR( CGFloat left,
                                                       CGFloat right)
{
   return( MulleEdgeInsetsMake( 0, left, 0, right));
}


static inline MulleEdgeInsets   MulleEdgeInsetsMakeTB( CGFloat top,
                                                       CGFloat bottom)
{
   return( MulleEdgeInsetsMake( top, 0, bottom, 0));
}


static inline MulleEdgeInsets   MulleEdgeInsetsMakeU( CGFloat topLeft,
                                                      CGFloat leftTop,
                                                      CGFloat bottomRight,
                                                      CGFloat rightBottom,
                                                      unsigned int index)
{
   if( index == 0)
      return( MulleEdgeInsetsMake( topLeft, leftTop, bottomRight, rightBottom));

   return( MulleEdgeInsetsMake( leftTop, topLeft, rightBottom, bottomRight));
}



// Equality truth table (under -0.0 sanitization contract):
//
//   Case                 Result   Reason
//   +0.0 == +0.0         YES     memcmp: identical bits
//   +0.0 == -0.0         n/a     -0.0 cannot exist (Make/Data sanitize)
//   NaN  == NaN          YES     memcmp: same bit pattern
//   NaN  == finite       NO      memcmp: different bits
//   +INF == +INF         YES     memcmp: same bit pattern
//   +INF == -INF         NO      memcmp: different sign bit
//
static inline int   MulleEdgeInsetsEqualToEdgeInsets( MulleEdgeInsets insets1,
                                                      MulleEdgeInsets insets2)
{
   return( ! memcmp( &insets1, &insets2, sizeof( MulleEdgeInsets)));
}


static inline CGPoint  MulleEdgeInsetsInsetPoint( MulleEdgeInsets insets,
                                                  CGPoint origin)
{
   origin.x += insets.left;
   origin.y += insets.top;
   return( origin);
}


static inline CGPoint  MulleEdgeInsetsExtrudePoint( MulleEdgeInsets insets,
                                                    CGPoint origin)
{
   origin.x -= insets.left;
   origin.y -= insets.top;
   return( origin);
}


static inline CGSize   MulleEdgeInsetsInsetSize( MulleEdgeInsets insets,
                                                 CGSize size)
{
   size.width  -= insets.right + insets.left;
   size.height -= insets.top + insets.bottom;
   return( size);
}


static inline CGSize   MulleEdgeInsetsExtrudeSize( MulleEdgeInsets insets,
                                                   CGSize size)
{
   size.width  += insets.right + insets.left;
   size.height += insets.top + insets.bottom;
   return( size);
}



static inline CGRect   MulleEdgeInsetsInsetRect( MulleEdgeInsets insets,
                                                 CGRect rect)
{
   rect.origin = MulleEdgeInsetsInsetPoint( insets, rect.origin);
   rect.size   = MulleEdgeInsetsInsetSize( insets, rect.size);
   return( rect);
}


static inline CGRect   MulleEdgeInsetsExtrudeRect( MulleEdgeInsets insets,
                                                   CGRect rect)
{
   rect.origin = MulleEdgeInsetsExtrudePoint( insets, rect.origin);
   rect.size   = MulleEdgeInsetsExtrudeSize( insets, rect.size);
   return( rect);
}




// possibly too large for inlining usefully
// this computes the minimum margins, we keep the verb like CGRect
static inline MulleEdgeInsets   MulleEdgeInsetsIntersect( MulleEdgeInsets a,
                                                          MulleEdgeInsets b)
{
   return( MulleEdgeInsetsMakeAtariBE( a.right < b.right ? a.right : b.right,
                                       a.left < b.left ? a.left : b.left,
                                       a.bottom < b.bottom ? a.bottom : b.bottom,
                                       a.top < b.top ? a.top : b.top));
}

static inline MulleEdgeInsets   MulleEdgeInsetsUnion( MulleEdgeInsets a,
                                                      MulleEdgeInsets b)
{
   return( MulleEdgeInsetsMakeAtariBE( a.right > b.right ? a.right : b.right,
                                       a.left > b.left ? a.left : b.left,
                                       a.bottom > b.bottom ? a.bottom : b.bottom,
                                       a.top > b.top ? a.top : b.top));
}


static inline CGRect  CGRectInsetUsingEdgeInsets( CGRect rect, MulleEdgeInsets insets)
{
   return( MulleEdgeInsetsInsetRect( insets, rect));
}


static inline CGRect  CGRectExtrudeUsingEdgeInsets( CGRect rect, MulleEdgeInsets insets)
{
   return( MulleEdgeInsetsExtrudeRect( insets, rect));
}




static inline MulleEdgeInsets   CGRectEdgeInsetsInRect( CGRect rect, CGRect outer)
{
   return( MulleEdgeInsetsMakeTLBR( CGRectGetMinY( rect) - CGRectGetMinY( outer),
                                    CGRectGetMinX( rect) - CGRectGetMinX( outer),
                                    CGRectGetMaxY( outer) - CGRectGetMaxY( rect),
                                    CGRectGetMaxX( outer) - CGRectGetMaxX( rect)));
}


#endif
