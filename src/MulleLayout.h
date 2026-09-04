//
//  MulleLayout.h
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
#ifndef mulle_layout_h__
#define mulle_layout_h__

#include "include.h"

#include "MulleEdgeInsets.h"

#include "MulleLayoutContent.h"


//
// MulleLayout supports layouting of rectangles in rows and columns.
// Rectangles are added left to right to a row, until its filled. 
// Then a new row should be used.
// If you need right to left filling, do a left to right fill with
// inverse margins and then change the coordinates.
//
// Layout state is kept in "struct MulleLayout"
//
//    .-----------------------.
//    | .-------. .---. .---. |
//    | |       | |   | |   | |
//    | |       | '---' '---' |
//    | '-------'             |
//    | .---. .-------------. |
//    | |   | |             | |
//    | '---' |             | |
//    |       '-------------' |
//    '-----------------------'
//
// Basically follow the definitions set in here
// https://yogalayout.com/
// except we don't do padding (which is insets used by UIView for the content).
//
// The margin handling is different, in that we merge margins.
// i.e. ---| |----   will be a distance of 20 and not of 30 like in Yoga
//       10   20
//

// keep this simple.. any additional layouting like centering or justifying
// should be done in a second pass.
// Distribution modes (fill, equal spacing, proportional, etc.) and cross-axis
// alignment live in UIStackView (MulleUIWidget), which operates on views
// with autoresizing masks and flexible sizes — not pure rect geometry.

struct MulleLayout
{
   CGRect            originalRect;
   MulleEdgeInsets   originalMargins;

   CGRect            rect;      // growing rect of the current row
   MulleEdgeInsets   margins;   // growing margins of the current row

   int               reverse;
   int               vertical;  // when set, layout fills columns instead of rows
};


//
// Initial margins are inside the rect!
//
//         rect
// +-------------------+
// | l      top      r |
// | e +-----------+ i |
// | f |    row    | g |
// | t +-----------+ h |
// |       bottom    t |
// +-------------------+
//
static inline void   _MulleLayoutInit( struct MulleLayout *p,
                                       CGRect rect,
                                       MulleEdgeInsets margins)
{
   p->originalRect    = rect;
   p->originalMargins = margins;

   p->rect.origin     = rect.origin;
   p->rect.size       = CGSizeMake( 0, 0);
   p->margins         = margins;

   p->reverse         = 0;
   p->vertical        = 0;
}


static inline void   _MulleLayoutInitReverse( struct MulleLayout *p,
                                              CGRect rect,
                                              MulleEdgeInsets margins)
{
   // read only (in algorithm)
   margins = MulleEdgeInsetsMake( margins.top, 
                                  margins.right, 
                                  margins.bottom, 
                                  margins.left);
   p->originalRect    = rect;
   p->originalMargins = margins;

   p->rect.origin     = rect.origin;
   p->rect.size       = CGSizeMake( 0, 0);
   p->margins         = margins;

   p->reverse         = 1;
   p->vertical        = 0;
}


static inline void   MulleLayoutInit( struct MulleLayout *p,
                                      CGRect rect,
                                      MulleEdgeInsets margins)
{
   if( p)
      _MulleLayoutInit( p, rect, margins);
}


static inline void   MulleLayoutInitReverse( struct MulleLayout *p,
                                             CGRect rect,
                                             MulleEdgeInsets margins)
{
   if( p)
      _MulleLayoutInitReverse( p, rect, margins);
}


/// Initialize a vertical layout. Items fill top-to-bottom, wrapping to
/// a new column when the height is exhausted. Internally the frame is
/// transposed so the horizontal row logic operates on the vertical axis.
static inline void   MulleLayoutInitVertical( struct MulleLayout *p,
                                              CGRect rect,
                                              MulleEdgeInsets margins)
{
   if( p)
   {
      _MulleLayoutInit( p,
                        CGRectMake( rect.origin.y, rect.origin.x,
                                    rect.size.height, rect.size.width),
                        MulleEdgeInsetsMake( margins.left, margins.top,
                                            margins.right, margins.bottom));
      p->vertical = 1;
   }
}


//
// currently there is nothing to do here, but in the future this
// might free some stuff in the struct
//
static inline void   _MulleLayoutDone( struct MulleLayout *p)
{
   MULLE_C_UNUSED( p);
}


static inline void   MulleLayoutDone( struct MulleLayout *p)
{
   if( p)
      _MulleLayoutDone( p);
}


static inline CGRect   MulleLayoutGetContentRect( struct MulleLayout *p)
{
   CGRect   rect;

   if( ! p)
      return( CGRectZero);
   rect = MulleEdgeInsetsInsetRect( p->originalMargins, p->originalRect);
   if( p->reverse)
      rect = CGRectMirrorInRectH( rect, p->originalRect);
   if( p->vertical)
      rect = CGRectMake( rect.origin.y, rect.origin.x,
                         rect.size.height, rect.size.width);
   return( rect);
}


//
// Two rects have been placed into a row. The "envelope" is the rowContent
//
//     c = rowContentRect
// +++------|.....
// +1+   +++|    .
// +++ c +2+|    .
// ------+++|    .
// ,        .    .
// ...............
//
MULLE__LAYOUT_GLOBAL
CGRect   _MulleLayoutGetRowContentRect( struct MulleLayout *p);


static inline CGPoint   MulleLayoutGetRowContentOrigin( struct MulleLayout *p)
{
   if( ! p)
      return( CGPointZero);
   return( _MulleLayoutGetRowContentRect( p).origin);
}


static inline CGSize   MulleLayoutGetRowContentSize( struct MulleLayout *p)
{
   if( ! p)
      return( CGSizeZero);
   return( _MulleLayoutGetRowContentRect( p).size);
}


static inline CGRect   MulleLayoutGetRowContentRect( struct MulleLayout *p)
{
   if( ! p)
      return( CGRectZero);
   return( _MulleLayoutGetRowContentRect( p));
}



//
// Two rects have been placed into a row. The remaining vertical and horizontal
// space is the remainingRowContentRect
//
//            r = remainingRowContentRect
// +++......|-----
// +1+   +++|     |
// +++   +2+|     |
// .     +++| r   |
// .        |     |
// .........|-----
//
MULLE__LAYOUT_GLOBAL
CGRect   _MulleLayoutGetRemainingRowContentRect( struct MulleLayout *p);

static inline CGPoint   MulleLayoutGetRemainingRowContentOrigin( struct MulleLayout *p)
{
   if( ! p)
      return( CGPointZero);
   return( _MulleLayoutGetRemainingRowContentRect( p).origin);
}


static inline CGSize   MulleLayoutGetRemainingRowContentSize( struct MulleLayout *p)
{
   if( ! p)
      return( CGSizeZero);
   return( _MulleLayoutGetRemainingRowContentRect( p).size);
}


static inline CGRect   MulleLayoutGetRemainingRowContentRect( struct MulleLayout *p)
{
   if( ! p)
      return( CGRectZero);
   return( _MulleLayoutGetRemainingRowContentRect( p));
}


//
// Total used rect across all rows: the bounding box of all placed items.
// Width spans the full content width (frame minus left/right insets).
// Height covers from the content top to the bottom of the current row's items.
// Returns CGRectZero if nothing has been placed.
// Use this for scroll view content size or sizeToFit calculations.
//
MULLE__LAYOUT_GLOBAL
CGRect   _MulleLayoutGetUsedRect( struct MulleLayout *p);


static inline CGRect   MulleLayoutGetUsedRect( struct MulleLayout *p)
{
   if( ! p)
      return( CGRectZero);
   return( _MulleLayoutGetUsedRect( p));
}


static inline CGSize   MulleLayoutGetUsedSize( struct MulleLayout *p)
{
   return( MulleLayoutGetUsedRect( p).size);
}


static inline int   _MulleLayoutIsEmptyRow( struct MulleLayout *p)
{
   return( p->rect.size.width == 0.0);
}


static inline int   MulleLayoutIsEmptyRow(  struct MulleLayout *p)
{
   if( ! p)
      return( 0);
   return( _MulleLayoutIsEmptyRow( p));
}


//
// Each row starts at 0,0. The origin is added to the returned rect on return.
// When a row is done, the height of the row is added to the origin. This works
// stricly left to right.
//
// +-------------------+
// |                   |
// +-------------------+
//
MULLE__LAYOUT_GLOBAL
void   _MulleLayoutNewRow( struct MulleLayout *p);


static inline void   MulleLayoutNewRow( struct MulleLayout *p)
{
   if( p)
      _MulleLayoutNewRow( p);
}


//
// Straightforward. Request a placement for a rectangle of "size" with 
// distance to other rectangles in "margins". The returned rectangle 
// can be further away from other rectangles but not nearer.
// Use the returned CGRect as the layouted rect.
// 
//
// The given margins are outside of the rect!
//
//          row
// +-------------------+
// | l      top      r |
// | e +-----------+ i |
// | f |    size   | g |
// | t +-----------+ h |
// |       bottom    t |
// +-------------------+
//
// The margin handling is a bit complicated. Size doesn't fit, if there isn't 
// room enough for margins, which is also the right edge of the space. 
// The larger margin wins.
//
// Returns the layouted rectangle, for a rect of size "size", which has been
// placed into the current row, if space is available.
// Returns CGRectNull rect, if it doesn't fit. If it doesn't fit, its
// time for a new row. 
//
MULLE__LAYOUT_GLOBAL
CGRect   _MulleLayoutAddToRow( struct MulleLayout *p,
                               CGSize size,
                               MulleEdgeInsets margins);


static inline CGRect   MulleLayoutAddToRow( struct MulleLayout *p,
                                            CGSize size,
                                            MulleEdgeInsets margins)
{
   if( ! p)
      return( CGRectNull);
   return( _MulleLayoutAddToRow( p, size, margins));
}


// an existing row will be preempted, a new row of height h will be
// returned ( if space is available)
MULLE__LAYOUT_GLOBAL
CGRect   _MulleLayoutAddRowWithHeight( struct MulleLayout *p,
                                       CGFloat h,
                                       MulleEdgeInsets margins);

static inline CGRect   MulleLayoutAddRowWithHeight( struct MulleLayout *p,
                                                    CGFloat h,
                                                    MulleEdgeInsets margins)
{
   if( ! p)
      return( CGRectNull);
   return( _MulleLayoutAddRowWithHeight( p, h, margins));
}


MULLE__LAYOUT_GLOBAL
CGRect   _MulleLayoutAddToRowWithOverflow( struct MulleLayout *p,
                                           CGSize size,
                                           MulleEdgeInsets margins);


static inline CGRect   MulleLayoutAddToRowWithOverflow( struct MulleLayout *p,
                                                        CGSize size,
                                                        MulleEdgeInsets margins)
{
   if( ! p)
      return( CGRectNull);
   return( _MulleLayoutAddToRowWithOverflow( p, size, margins));
}

// TODO: more to come...
#endif
