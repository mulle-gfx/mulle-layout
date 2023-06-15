//
// Copyright (c) 2020 Nat!, Mulle kybernetiK
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


#define MulleEdgeInsetsData( t, l, b, r)  { (l), (t), (r), (b) }

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
   return( (MulleEdgeInsets){ .top    = top,
                              .left   = left,
                              .bottom = bottom,
                              .right  = right });
}

static inline MulleEdgeInsets   MulleEdgeInsetsMakeTLBR( CGFloat top,
                                                         CGFloat left,
                                                         CGFloat bottom,
                                                         CGFloat right)
{
   return( (MulleEdgeInsets){ .top    = top,
                              .left   = left,
                              .bottom = bottom,
                              .right  = right });
}


static inline MulleEdgeInsets   MulleEdgeInsetsMakeCounterClockwise( CGFloat top,
                                                                     CGFloat left,
                                                                     CGFloat bottom,
                                                                     CGFloat right)
{
   return( (MulleEdgeInsets){ .top    = top,
                              .left   = left,
                              .bottom = bottom,
                              .right  = right });
}


// this is probably the most common way, looking like CSS dox
static inline MulleEdgeInsets   MulleEdgeInsetsMakeClockwise( CGFloat top,
                                                              CGFloat right,
                                                              CGFloat bottom,
                                                              CGFloat left)
{
   return( (MulleEdgeInsets){ .top    = top,
                              .left   = left,
                              .bottom = bottom,
                              .right  = right });
}

// this keeps related values together best
static inline MulleEdgeInsets   MulleEdgeInsetsMakeHV( CGFloat left,
                                                       CGFloat right,
                                                       CGFloat top,
                                                       CGFloat bottom)
{
   return( (MulleEdgeInsets){ .top    = top,
                              .left   = left,
                              .bottom = bottom,
                              .right  = right });
}

// order of atari joystick bits, should have been made into law really
// this is little endian. This could also be MulleEdgeInsetsMakeVH
static inline MulleEdgeInsets   MulleEdgeInsetsMakeAtariLE( CGFloat top,
                                                            CGFloat bottom,
                                                            CGFloat left,
                                                            CGFloat right)
{
   return( (MulleEdgeInsets){ .top    = top,
                              .left   = left,
                              .bottom = bottom,
                              .right  = right });
}

// this is big endian, which is my actual mind model
static inline MulleEdgeInsets   MulleEdgeInsetsMakeAtariBE( CGFloat right,
                                                            CGFloat left,
                                                            CGFloat bottom,
                                                            CGFloat top)
{
   return( (MulleEdgeInsets){ .top    = top,
                              .left   = left,
                              .bottom = bottom,
                              .right  = right });
}



// This is what feels natural to me, looking at a rectangle and knowing its
// not HV :) This order also performs the best. There is probably a connection
// there...
static inline MulleEdgeInsets   MulleEdgeInsetsMakeNatural( CGFloat left,
                                                            CGFloat top,
                                                            CGFloat right,
                                                            CGFloat bottom)
{
   return( (MulleEdgeInsets){ .top    = top,
                              .left   = left,
                              .bottom = bottom,
                              .right  = right });
}


//
// Left Right only is fairly often used, avoids T/L coordinates
//
static inline MulleEdgeInsets   MulleEdgeInsetsMakeLR( CGFloat left,
                                                       CGFloat right)
{
   return( (MulleEdgeInsets){ .top    = 0,
                              .left   = left,
                              .bottom = 0,
                              .right  = right });
}


static inline MulleEdgeInsets   MulleEdgeInsetsMakeTB( CGFloat top,
                                                       CGFloat bottom)
{
   return( (MulleEdgeInsets){ .top    = top,
                              .left   = 0,
                              .bottom = bottom,
                              .right  = 0 });
}


static inline MulleEdgeInsets   MulleEdgeInsetsMakeU( CGFloat topLeft,
                                                      CGFloat leftTop,
                                                      CGFloat bottomRight,
                                                      CGFloat rightBottom,
                                                      unsigned int index)
{
   if( index == 0)
      return( (MulleEdgeInsets){ .top    = topLeft,
                                 .left   = leftTop,
                                 .bottom = bottomRight,
                                 .right  = rightBottom });

   return( (MulleEdgeInsets){ .top    = leftTop,
                              .left   = topLeft,
                              .bottom = rightBottom,
                              .right  = bottomRight });
}



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


#define MulleEdgeInsetsZero      MulleEdgeInsetsMake( 0.0f, 0.0f, 0.0f, 0.0f)


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

#endif
