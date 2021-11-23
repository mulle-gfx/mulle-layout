//
// Copyright (c) 2021 Nat!, Mulle kybernetiK
//
#ifndef mulle_layout_h__
#define mulle_layout_h__

#include "include.h"

#include "MulleEdgeInsets.h"

//
// MulleLayout supports layouting of other rectangle in rows and columns.
// Rectangles are added to a row, until its filled. Then a new row is used.
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
// except we don't do padding (which is insets used by UIView for the content)
// The margin handling is different, in that we merge margins.
// i.e. ---| |----   will be a distance of 20 and not of 30 like in Yoga
//       10   20
//

// keep this simple.. any additional layouting like centering or justifying
// should be done in a second pass

struct MulleLayout
{
   CGRect            originalRect;
   MulleEdgeInsets   originalMargins;

   CGRect            rect;      // growing rect of the current row
   MulleEdgeInsets   margins;   // growing margins of the current row

   CGFloat           maxMarginY;
};


static inline void  _MulleLayoutInit( struct MulleLayout *p,
                                      CGRect rect,
                                      MulleEdgeInsets margins)
{
   // read only (in algorithm)
   p->originalRect    = rect;
   p->originalMargins = margins;

   p->rect.origin     = rect.origin;
   p->rect.size       = CGSizeMake( 0, 0);
   p->margins         = margins;
}


static inline void  MulleLayoutInit( struct MulleLayout *p,
                                     CGRect rect,
                                     MulleEdgeInsets margins)
{
   if( p)
      _MulleLayoutInit( p, rect, margins);
}


//
// currently there is nothing to do here, but in the future this
// might free some stuff in the struct
//
static inline void  _MulleLayoutDone( struct MulleLayout *p)
{
}


static inline void  MulleLayoutDone( struct MulleLayout *p)
{
   if( p)
      _MulleLayoutDone( p);
}


//
// +++.......
// + +   ++ .
// +++   ++ .
// .........+
//
// ###
// # #
// ###

static inline CGSize  _MulleLayoutGetRemainingRowSize( struct MulleLayout *p)
{
   CGSize   size;

   size.width  = CGRectGetMaxX( p->originalRect) - (CGRectGetMaxX( p->rect) + p->margins.right);
   size.height = CGRectGetMaxY( p->originalRect) - (CGRectGetMaxY( p->rect) + p->margins.bottom);

   return( size);
}


static inline CGSize  MulleLayoutGetRemainingRowSize( struct MulleLayout *p)
{
   if( ! p)
      return( CGSizeZero);
   return( _MulleLayoutGetRemainingRowSize( p));
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



// Each row starts at 0,0. The origin is added to the returned rect on return.
// When a row is done, the height of the row is added to the origin.
//
// +-------------------+
// |                   |
// +-------------------+
//
void   _MulleLayoutNewRow( struct MulleLayout *p);


static inline void   MulleLayoutNewRow( struct MulleLayout *p)
{
   if( p)
      _MulleLayoutNewRow( p);
}


//
// Straightforward, except when space runs out. The margin handling is also
// a bit complicated. Size doesn't fit, if there isn't room enough for margins,
// which is also the right edge of the space. The larger margin wins.
// Returns CGRectNull rect, if it doesn't fit. Does not attempt to change 'y'
//
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
