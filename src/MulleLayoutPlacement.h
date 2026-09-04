//
//  MulleLayoutPlacement.h
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
static inline CGRect   CGRectAlignLeft( CGRect rect, CGFloat x)
{
   rect.origin.x = x;
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
static inline CGRect   CGRectAlignRight( CGRect rect, CGFloat x)
{
   rect.origin.x = x - rect.size.width;
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


// uintptr_t for compatibility with NSUInteger, which we don't have
// here yet
//
// TODO: rename to MulleLayoutPlacement_t ?
//
typedef uintptr_t    MulleLayoutPlacement;

//
//    Up
//    Down
//    Left
//    Right
//
enum MulleLayoutVerticalPlacement
{
   MulleLayoutPlacementKeepY         = 0,    // keeps current y position unchanged
   MulleLayoutPlacementUp            = 1,    // aligns bottom of rect with other top
   MulleLayoutPlacementDown          = 2,    // aligns top of rect with other bottom
   MulleLayoutPlacementMiddle        = 3,    // aligns middle (V) of rect with other middle
   MulleLayoutPlacementTopAligned    = 4,    // aligns top of rect with other top
   MulleLayoutPlacementBottomAligned = 5     // aligns bottom of rect with other bottom
};

typedef uintptr_t    MulleLayoutVerticalPlacement;

enum MulleLayoutHorizontalPlacement
{
   MulleLayoutPlacementKeepX         = 0,    // keeps current x position unchanged
   MulleLayoutPlacementLeft          = 1,    // aligns right edge of rect with other left
   MulleLayoutPlacementRight         = 2,    // aligns left edge of rect with other right
   MulleLayoutPlacementCenter        = 3,    // aligns center (H) of rect with other center
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
// Position rect at either side of other, if space fits.
// Given a list of MulleLayoutPlacement and they are tried until something
// fits.
//
// If you pass NULL for preferences, it will use
// a default scheme. If you don't want any scheme set n_preferences to 0
// and pass something else than NULL for preferences.
//
MULLE__LAYOUT_GLOBAL
int  CGRectFindBestPlacementNextToRectInSpace( CGRect rect,
                                               CGRect other,
                                               CGRect space,
                                               MulleLayoutPlacement *preferences,
                                               unsigned int n_preferences);

// 
// If you pass NULL for preferences, it will use
// a default scheme. If you don't want any scheme set n_preferences to 0
// and pass something else than NULL for preferences.
//
MULLE__LAYOUT_GLOBAL
CGPoint  CGRectPlaceNextToRectInSpace( CGRect rect,
                                       CGRect other,
                                       CGRect space,
                                       MulleLayoutPlacement *preferences,
                                       unsigned int n_preferences);

//
// Something very similiar for menus really.
// 
// If you pass NULL for preferences, it will use
// a default scheme. If you don't want any scheme set n_preferences to 0
// and pass something else than NULL for preferences.
//
MULLE__LAYOUT_GLOBAL
CGRect   CGSizePlaceAdjacentToRectInSpace( CGSize menuSize,
                                           CGRect buttonFrame,
                                           CGRect space,
                                           MulleLayoutPlacement *preferences,
                                           unsigned int n_preferences);

#endif
