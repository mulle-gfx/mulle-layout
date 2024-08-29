//
// Copyright (c) 2021 Nat!, Mulle kybernetiK
//
#ifndef mulle_layout_placement_h__
#define mulle_layout_placement_h__

#include "include.h"

#include <stdint.h>



// ### Alignment

//         : x        :x  
// .-------:-.        .---------.
// |       : |        |         |
// |  rect : |   ->   | result  |
// |       : |        |         |
// '-------:-'        '---------'
//         :          :
// adjust rect, so that left point is on x
static inline CGRect   CGRectAlignLeft( CGRect rect, CGFloat y)
{
   rect.origin.y = y;
   return( rect);
}


//         : x             : x  
// .-------:-.        .---------.
// |       : |        |    :    |
// |  rect : |   ->   | result  |
// |       : |        |    :    |
// '-------:-'        '---------'
//         :               :
// adjust rect, so that center point is on x
static inline CGRect   CGRectAlignCenter( CGRect rect, CGFloat x)
{
   rect.origin.x = x - rect.size.width / 2.0f;
   return( rect);
}

//         : x                  : x
// .-------:-.        .---------.
// |       : |        |         |
// |  rect : |   ->   | result  |
// |       : |        |         |
// '-------:-'        '---------'
//         :                    :
// adjust rect, so that right point is on x
static inline CGRect   CGRectAlignRight( CGRect rect, CGFloat y)
{
   rect.origin.y = y - rect.size.width;
   return( rect);
}


// ............. y            
// .----------.       ..----------.. y
// |rect      |        |result    |
// |          |   ->   |          |
// |          |        |          |
// '----------'        '----------'
//                          
// adjust rect, so that centerpoint is on y
static inline CGRect   CGRectAlignTop( CGRect rect, CGFloat y)
{
   rect.origin.y = y;
   return( rect);
}



// ............. y            
// .----------.        .----------.
// |rect      |        |result    |
// |          |   ->  .|..........|.y
// |          |        |          |
// '----------'        '----------'
//                          
// adjust rect, so that centerpoint is on y
static inline CGRect   CGRectAlignMiddle( CGRect rect, CGFloat y)
{
   rect.origin.y = y - rect.size.height / 2.0f;
   return( rect);
}


// ............. y            
// .----------.        .----------.
// |rect      |        |result    |
// |          |   ->   |          |
// |          |        |          |
// '----------'       .'----------'. y
//                          
// adjust rect, so that bottom is on y
static inline CGRect   CGRectAlignBottom( CGRect rect, CGFloat y)
{
   rect.origin.y = y - rect.size.height;
   return( rect);
}


// ### Simple placement

// .--------. .------.
// |        |s|      |
// | result |p| rect |
// |        |c|      |
// '--------' '------'
//    width
static inline CGRect   CGRectPlaceBeforeRect( CGRect rect, CGFloat spacing, CGFloat width)
{
   rect.origin.x   = CGRectGetMinX( rect) - width - spacing;
   rect.size.width = width;
   return( rect);
}


// .--------. .-----------.
// |        |s|           |
// |  rect  |p|  result   |
// |        |c|           |
// '--------' '-----------'
//                width
static inline CGRect   CGRectPlaceAfterRect( CGRect rect, CGFloat spacing, CGFloat width)
{
   rect.origin.x   = CGRectGetMaxX( rect) + spacing;
   rect.size.width = width;
   return( rect);
}


// .--------. 
// |        |
// | result | height
// |        |
// '--------'
//  spacing
// .--------. 
// |        |
// |        |
// |  rect  |
// |        |
// |        | 
// '--------' 
static inline CGRect   CGRectPlaceAboveRect( CGRect rect, CGFloat spacing, CGFloat height)
{
   rect.origin.y    = CGRectGetMinY( rect)  - height - spacing;
   rect.size.height = height;
   return( rect);
}


// .--------. 
// |        |
// |        |
// |  rect  | 
// |        |
// |        |
// '--------'
// .--------. 
// |        |
// | result | height
// |        | 
// '--------' 

static inline CGRect   CGRectPlaceBelowRect( CGRect rect, CGFloat spacing, CGFloat height)
{
   rect.origin.y    = CGRectGetMaxY( rect) + spacing;
   rect.size.height = height;
   return( rect);
}



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
   MulleLayoutPlacementBottomAligned = 5     // aligns bottom of rect with other bottom
};

typedef uintptr_t    MulleLayoutVerticalPlacement;

enum MulleLayoutHorizontalPlacement
{
   MulleLayoutPlacementKeepX         = 0,    // aligns bottom of rect with other top
   MulleLayoutPlacementLeft          = 1,    // aligns right edge of rect with other left
   MulleLayoutPlacementRight         = 2,    // aligns left edge of rect with other right
   MulleLayoutPlacementCenter        = 3,    // aligns center (V) of rect with other center
   MulleLayoutPlacementLeftAligned   = 4,    // aligns left edge of rect with other left
   MulleLayoutPlacementRightAligned  = 5     // aligns right edge of rect with other right
};

typedef uintptr_t    MulleLayoutHorizontalPlacement;


// for static init
#define MulleLayoutPlacementData( h, v)   (((h) << 4) | (v))


static inline  MulleLayoutPlacement
   MulleLayoutPlacementMake( MulleLayoutHorizontalPlacement h,
                             MulleLayoutVerticalPlacement v)
{
   return( (h << 4) | v);
}


static inline MulleLayoutVerticalPlacement
   MulleLayoutPlacementGetVerticalPlacement( MulleLayoutPlacement value)
{
   return( (MulleLayoutVerticalPlacement) value & 0xF);
}


static inline MulleLayoutHorizontalPlacement
   MulleLayoutPlacementGetHorizontalPlacement( MulleLayoutPlacement value)
{
   return( (MulleLayoutHorizontalPlacement) value >> 4);
}



MULLE__LAYOUT_GLOBAL
CGPoint   CGRectPlaceNextToRect( CGRect rect,
                                 CGRect other,
                                 MulleLayoutPlacement placement);

//
// position rect at either side of other, if space fits.
// Gives a list of MulleLayoutPlacement and they are tried until something
// fits.
//
MULLE__LAYOUT_GLOBAL
int  CGRectFindBestPlacementNextToRectInSpace( CGRect rect,
                                               CGRect other,
                                               CGRect space,
                                               MulleLayoutPlacement *preferences,
                                               unsigned int n_preferences);

MULLE__LAYOUT_GLOBAL
CGPoint  CGRectPlaceNextToRectInSpace( CGRect rect,
                                       CGRect other,
                                       CGRect space,
                                       MulleLayoutPlacement *preferences,
                                       unsigned int n_preferences);

#endif
