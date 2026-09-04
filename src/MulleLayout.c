//
//  MulleLayout.c
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
#include "MulleLayout.h"

#include "MulleLayoutContent.h"

#include "include-private.h"


void   _MulleLayoutNewRow( struct MulleLayout *p)
{
   p->margins.top    = p->originalMargins.top > p->margins.bottom
                         ? p->originalMargins.top
                         : p->margins.bottom;
   p->margins.bottom = p->originalMargins.bottom;
   p->margins.left   = p->originalMargins.left;
   p->margins.right  = p->originalMargins.right;

   p->rect.origin.x  = p->originalRect.origin.x;
   p->rect.origin.y += p->rect.size.height;
   p->rect.size      = CGSizeZero;
}


CGRect   _MulleLayoutAddToRow( struct MulleLayout *p,
                               CGSize size,
                               MulleEdgeInsets margins)
{
   CGRect   rect;

// 
//   if( size.width == 0.0)  // don't want to dirty rect.origin.y then ...
//      return( CGRectNull);

   if( p->vertical)
   {
      size    = CGSizeMake( size.height, size.width);
      margins = MulleEdgeInsetsMake( margins.left, margins.top,
                                     margins.right, margins.bottom);
   }

   if( p->reverse)
      margins = MulleEdgeInsetsMake( margins.top, 
                                     margins.right, 
                                     margins.bottom, 
                                     margins.left);
   rect.size     = size;
   // check that size wold fit with top margins added 
   margins.top   = margins.top > p->margins.top
                      ? margins.top
                      : p->margins.top;
   rect.origin.y = CGRectGetMinY( p->rect) + margins.top;
   if( CGRectGetMaxY( rect) > CGRectGetMaxY( p->originalRect) - p->originalMargins.bottom)
      return( CGRectNull);

   // use larger margin for horizontal check
   rect.origin.x = CGRectGetMaxX( p->rect) + (margins.left > p->margins.left
                                                ? margins.left
                                                : p->margins.left);

   //
   // check that we don't extend over space, we only check against original
   // margins here, so that we can use the full "usable" width.
   // The right margin of the item is intentionally not checked here — only
   // the frame's right inset limits placement.  This is asymmetric but
   // deliberate: the item's right margin only contributes to the gap before
   // the next item, not to the boundary check.
   //
   if( CGRectGetMaxX( rect) > CGRectGetMaxX( p->originalRect) - p->originalMargins.right)
      return( CGRectNull);

   //
   // if size is empty, we don't touch the margins or rect of the original
   //
   if( size.width != 0.0)
   {
      // change remaining margin to what was given as margin right
      p->margins.left = margins.right;

      // we keep track of the maximum Y extents of the margins,
      // NOT the largest margin per se
      if( CGRectGetMaxY( rect) + margins.bottom > CGRectGetMaxY( p->rect) + p->margins.bottom)
         p->margins.bottom = margins.bottom;

      p->rect = CGRectUnion( p->rect, rect);
   }

   if( p->reverse)
      rect = CGRectMirrorInRectH( rect, p->originalRect);
   if( p->vertical)
      rect = CGRectMake( rect.origin.y, rect.origin.x,
                         rect.size.height, rect.size.width);
   return( rect);
}


CGRect   _MulleLayoutAddToRowWithOverflow( struct MulleLayout *p,
                                           CGSize size,
                                           MulleEdgeInsets margins)
{
   CGRect   rect;

   rect = _MulleLayoutAddToRow( p, size, margins);
   if( ! CGRectIsNull( rect))
      return( rect);

   _MulleLayoutNewRow( p);
   rect = _MulleLayoutAddToRow( p, size, margins);
   return( rect);
}


CGRect   _MulleLayoutAddRowWithHeight( struct MulleLayout *p,
                                       CGFloat h,
                                       MulleEdgeInsets margins)
{
   CGFloat           w;
   CGFloat           hMargin;
   MulleEdgeInsets   actualMargins;
   CGRect            rect;

   if( ! _MulleLayoutIsEmptyRow( p))
      _MulleLayoutNewRow( p);

   actualMargins = MulleEdgeInsetsUnion( margins, p->originalMargins);
   hMargin       = actualMargins.left + actualMargins.right;
   w             = p->originalRect.size.width - hMargin;
   if( w <= 0.0)
      return( CGRectNull);

   rect = _MulleLayoutAddToRow( p, CGSizeMake( w, h), margins);

   // immediately add a fresh new row
   _MulleLayoutNewRow( p);
   return( rect);
}

// TODO: would be nice to have but haven't used it anywhere
#if 0
CGRect   _MulleLayoutGetRowContentRect( struct MulleLayout *p)
{
   CGRect   rect;

   rect.origin = MulleEdgeInsetsInsetPoint( p->originalMargins, p->originalRect.origin);
   rect.size   = CGSizeZero;

   if( p->rect.size.width != 0.0)
   {
      // if only one row
      if( p->originalRect.origin.y == p->rect.origin.y)
      {
         rect.size = p->rect.size;
      }
      else
      {
         rect.size.width  = (CGRectGetMaxX( p->originalRect) - p->originalMargins.right);
         rect.size.height = CGRectGetMaxY( p->rect);
      }
   }

   if( p->reverse)
      rect = CGRectMirrorInRectH( rect, p->originalRect);
   return( rect);
}
#endif


CGRect   _MulleLayoutGetRowContentRect( struct MulleLayout *p)
{
   CGRect   rect;

   rect = p->rect;
   if( p->reverse)
      rect = CGRectMirrorInRectH( rect, p->originalRect);
   if( p->vertical)
      rect = CGRectMake( rect.origin.y, rect.origin.x,
                         rect.size.height, rect.size.width);
   return( rect);
}



CGRect   _MulleLayoutGetRemainingRowContentRect( struct MulleLayout *p)
{
   CGRect   rect;

   if( p->rect.size.width != 0.0)
   {
      rect.origin.x    = CGRectGetMaxX( p->rect) + p->margins.left;
      rect.origin.y    = p->originalRect.origin.y + p->originalMargins.top;
      rect.size.width  = (CGRectGetMaxX( p->originalRect) - p->originalMargins.right)
                         - rect.origin.x;
      rect.size.height = (CGRectGetMaxY( p->originalRect) - p->originalMargins.bottom)
                         - (CGRectGetMinY( p->originalRect) + p->originalMargins.top);
   }
   else
   {
      rect.origin = MulleEdgeInsetsInsetPoint( p->originalMargins, p->originalRect.origin);
      rect.size   = MulleEdgeInsetsInsetSize( p->originalMargins, p->originalRect.size);
   }

   // this can happen when we have leftover margins and the rect is exhausted
   // (also if edgeInsets are degenerate)
   if( rect.size.width < 0.0)
      rect.size.width = 0.0;
   if( rect.size.height < 0.0)
      rect.size.height = 0.0;

   if( p->reverse)
      rect = CGRectMirrorInRectH( rect, p->originalRect);
   if( p->vertical)
      rect = CGRectMake( rect.origin.y, rect.origin.x,
                         rect.size.height, rect.size.width);
   return( rect);
}



CGRect   _MulleLayoutGetUsedRect( struct MulleLayout *p)
{
   CGRect    rect;
   CGFloat   contentX;
   CGFloat   contentWidth;
   CGFloat   maxY;

   contentX     = p->originalRect.origin.x + p->originalMargins.left;
   contentWidth = p->originalRect.size.width
                  - p->originalMargins.left
                  - p->originalMargins.right;

   // p->rect.origin.y accumulates completed rows; adding size.height
   // gives the bottom of the current row's items
   maxY = CGRectGetMaxY( p->rect);

   // nothing placed yet
   if( p->rect.size.width == 0.0 && p->rect.origin.y == p->originalRect.origin.y)
      return( CGRectZero);

   rect.origin.x    = contentX;
   rect.origin.y    = p->originalRect.origin.y + p->originalMargins.top;
   rect.size.width  = contentWidth;
   rect.size.height = maxY - rect.origin.y;

   if( rect.size.width < 0.0)
      rect.size.width = 0.0;
   if( rect.size.height < 0.0)
      rect.size.height = 0.0;

   if( p->reverse)
      rect = CGRectMirrorInRectH( rect, p->originalRect);
   if( p->vertical)
      rect = CGRectMake( rect.origin.y, rect.origin.x,
                         rect.size.height, rect.size.width);
   return( rect);
}
