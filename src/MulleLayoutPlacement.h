//
// Copyright (c) 2021 Nat!, Mulle kybernetiK
//
#ifndef mulle_layout_placement_h__
#define mulle_layout_placement_h__

#include "include.h"

#include <stdint.h>


//
// Layout placement is concerned about placing a rectangle in proximity of
// another rectangle.
//
// .-------------.
// |             |
// |             | .---.
// |             | |   |
// |             | '---'
// |             |
// '-------------'


// uintptr_t for comapatibility with NSUInteger, which we don't have
// here yet
typedef uintptr_t    MulleLayoutPlacement;

//
//    Up
//    Down
//    Left
//    Right
//
enum MulleLayoutVerticalPlacement
{
   MulleLayoutPlacementKeepY         = 0,    // aligns bottom of rect with other top
   MulleLayoutPlacementUp            = 1,    // aligns bottom of rect with other top
   MulleLayoutPlacementDown          = 2,    // aligns top of rect with other bottom
   MulleLayoutPlacementMiddle        = 3,    // aligns middle (H) of rect with other middle
   MulleLayoutPlacementTopAligned    = 4,    // aligns top of rect with other top
   MulleLayoutPlacementBottomAligned = 5     // aligns bottom of rect with other top
};

enum MulleLayoutHorizontalPlacement
{
   MulleLayoutPlacementKeepX         = 0,    // aligns bottom of rect with other top
   MulleLayoutPlacementLeft          = 1,    // aligns right edge of rect with other left
   MulleLayoutPlacementRight         = 2,    // aligns left edge of rect with other right
   MulleLayoutPlacementCenter        = 3,    // aligns center (V) of rect with other center
   MulleLayoutPlacementLeftAligned   = 4,    // aligns left edge of rect with other left
   MulleLayoutPlacementRightAligned  = 5     // aligns right edge of rect with other right
};


// for static init
#define _MulleLayoutPlacementMake( h, v)   (((h) << 4) | (v))


static inline  MulleLayoutPlacement
   MulleLayoutPlacementMake( enum MulleLayoutHorizontalPlacement h,
                             enum MulleLayoutVerticalPlacement v)
{
   return( (h << 4) | v);
}


static inline enum MulleLayoutVerticalPlacement
   MulleLayoutPlacementGetVerticalPlacement( MulleLayoutPlacement value)
{
   return( (enum MulleLayoutVerticalPlacement) value & 0xF);
}


static inline enum MulleLayoutHorizontalPlacement
   MulleLayoutPlacementGetHorizontalPlacement( MulleLayoutPlacement value)
{
   return( (enum MulleLayoutHorizontalPlacement) value >> 4);
}



CGPoint   CGRectPlaceNextToRect( CGRect rect,
                                 CGRect other,
                                 MulleLayoutPlacement placement);

//
// position rect at either side of other, if space fits.
// Gives a list of MulleLayoutPlacement and they are tried until something
// fits.
//
CGPoint   CGRectPlaceNextToRectInSpace( CGRect rect,
                                        CGRect other,
                                        CGRect space,
                                        uintptr_t n_preferences,
                                        unsigned char *preferences,
                                        intptr_t fallback);



// so adjust x, so that centers align
static inline CGRect   CGRectAlignHorizontally( CGRect rect, CGFloat x)
{
   rect.origin.x += x - CGRectGetMidX( rect);
   return( rect);
}


static inline CGRect   CGRectAlignVertically( CGRect rect, CGFloat y)
{
   rect.origin.y += y - CGRectGetMidY( rect);
   return( rect);
}

#endif
