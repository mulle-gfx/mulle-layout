//
// Copyright (c) 2021 Nat!, Mulle kybernetiK
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
   CGRect    rect;
   CGFloat   margin;

   if( ! p)
      return( CGRectNull);
//   if( size.width == 0.0)  // don't want to dirty rect.origin.y then ...
//      return( CGRectNull);

   if( p->reverse)
      margins = MulleEdgeInsetsMake( margins.top, 
                                    margins.right, 
                                    margins.bottom, 
                                    margins.left);
   rect.size   = size;

   // check that size wold fit with top margins added 
   margins.top = margins.top > p->margins.top
                     ? margins.top
                     : p->margins.top;

   rect.origin.y = CGRectGetMinY( p->rect) + margins.top;
   if( CGRectGetMaxY( rect) > CGRectGetMaxY( p->originalRect))
      return( CGRectNull);

   // use larger margin for horizontal check
   rect.origin.x = CGRectGetMaxX( p->rect) + (margins.left > p->margins.left
                                                ? margins.left
                                                : p->margins.left);

   //
   // check that we don't extend over space, we only check against original
   // margins here, so that we can use the full "usable" width
   // The right margin of the rect is ignored.... hmmmm
   //
   if( CGRectGetMaxX( rect) > CGRectGetMaxX( p->originalRect) - p->originalMargins.right)
      return( CGRectNull);

   // change remaining margin to what was given as margin right
   p->margins.left = margins.right;

   // we keep track of the maximum Y extents of the margins,
   // NOT the largest margin per se
   if( CGRectGetMaxY( rect) + margins.bottom > CGRectGetMaxY( p->rect) + p->margins.bottom)
      p->margins.bottom = margins.bottom;

   p->rect = CGRectUnion( p->rect, rect);

   if( p->reverse)
      rect = CGRectMirrorInRectH( rect, p->originalRect);
   return( rect);
}



CGRect   _MulleLayoutAddToRowWithOverflow( struct MulleLayout *p,
                                           CGSize size,
                                           MulleEdgeInsets margins)
{
   CGRect   rect;

   rect = MulleLayoutAddToRow( p, size, margins);
   if( rect.size.width != 0.0)
      return( rect);

   MulleLayoutNewRow( p);
   rect = MulleLayoutAddToRow( p, size, margins);
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

   rect = _MulleLayoutAddToRow( p, CGSizeMake( w, h), margins);

   // immediately add a fresh new row
   _MulleLayoutNewRow( p);
   return( rect);
}

