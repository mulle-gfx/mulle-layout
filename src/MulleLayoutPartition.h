//
//  MulleLayoutPartition.h
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
#ifndef mulle_layout_partition_h__
#define mulle_layout_partition_h__

#include "include.h"

#include <stdint.h>


//
// Partition divides a rectangle into smaller rectangles. 
//

// mulle additions:
// Example: partition `frame` into `n`(3) `rects` leaving `space` between them.
//
//  frame.origin
//       .----------------------------------------------------.
//       | .----------.       .----------.       .----------. |
//       | | rects[0] | space | rects[1] | space | rects[2] | |
//       | '----------'       '----------'       '----------' |
//       '----------------------------------------------------'
//                                                       frame.size

MULLE__LAYOUT_GLOBAL
void   CGRectPartitionHorizontally( CGRect frame,
                                    CGFloat space,
                                    uintptr_t n,
                                    CGRect *rects);


// different name, because I am used to CGRectPartitionIntoRowsAndColumns
static inline void   CGRectPartitionIntoColumns( CGRect frame,
                                                 CGFloat space,
                                                 uintptr_t n,
                                                 CGRect *rects)
{
   CGRectPartitionHorizontally( frame, space, n, rects);
}


// different name, because I prefer H over Horizontally at the moment
static inline void   CGRectPartitionH( CGRect frame,
                                       CGFloat space,
                                       uintptr_t n,
                                       CGRect *rects)
{
   CGRectPartitionHorizontally( frame, space, n, rects);
}

//
// as above but vertically
//
MULLE__LAYOUT_GLOBAL
void   CGRectPartitionVertically( CGRect frame,
                                  CGFloat space,
                                  uintptr_t n,
                                  CGRect *rects);



static inline void   CGRectPartitionIntoRows( CGRect frame,
                                              CGFloat space,
                                              uintptr_t n,
                                              CGRect *rects)
{
   CGRectPartitionVertically( frame, space, n, rects);
}


static inline void   CGRectPartitionV( CGRect frame,
                                       CGFloat space,
                                       uintptr_t n,
                                       CGRect *rects)
{
   CGRectPartitionVertically( frame, space, n, rects);
}


//
// Assume other is smack dab in the middle of rect
//
//    000000000
//    111   222
//    333333333
//
// Assume other is overlapping middle and part of the bottom
//
//    000000000
//    111   222
//    111   222
//
// Assume other is overlapping middle and part of the bottom
//
//    000000000
//    111
//    111
//
// returns the number of rects created. If there is no overlap, you get
// the original rect back in output[ 0] and i == 1.
// Returns 0 when other completely covers rect (nothing remains).
//
// This is the rect subtraction primitive of this library: subtract one rect
// from another, yielding up to 4 non-overlapping remainder fragments.
// For multiple occluders, apply iteratively — subdivide each surviving
// fragment against the next occluder. A dedicated multi-occluder API is
// not provided; the iterative approach is simple and covers real use cases
// (dirty-region culling, popup occlusion) without combinatorial output buffers.
//
MULLE__LAYOUT_GLOBAL
unsigned int   CGRectSubdivide( CGRect rect, CGRect other, CGRect output[ 4]);


// like CGRectSubdivid, but returns 0 if other completely overlaps rect, and
// -1 if rect and other do not overlap
//
MULLE__LAYOUT_GLOBAL
int   CGRectSubdivideIfOverlap( CGRect rect, CGRect other, CGRect output[ 4]);

// CGRectEdge values differ from Apple's (MinX=0, MinY=1, MaxX=2, MaxY=3).
// Here they map to CADirection: MinX=4(Left), MinY=1(Top), MaxX=8(Right),
// MaxY=2(Bottom). Not value-compatible — do not pass Apple enum literals.
//
enum
{
   CGRectMinXEdge = CADirectionLeft,
   CGRectMinYEdge = CADirectionTop,
   CGRectMaxXEdge = CADirectionRight,
   CGRectMaxYEdge = CADirectionBottom
};

typedef uintptr_t   CGRectEdge;



/* Divides a source rectangle into two component rectangles.
 *
 * (0,0)                      (width,0)
 * x-----------------------------x
 * |                             |
 * |                             |
 * |                             |
 * |                             |
 * |                             |
 * x-----------------------------x
 * (0,height)               (width,height)
 *
 *
 * The division is based on CGRectEdge, which edge to divide.
 * e.g CGRectMinYEdge
 *
 *   (0,0)                      (width,0)
 *    x-----------------------------x
 *    | slice            | (amount) |
 *    |                  v          |
 * (0,amount)------------------(width,amount)
 *    | reminder                    |
 *    |                             |
 *    x-----------------------------x
 *   0,height-amount)               (width,height-amount)
 */
MULLE__LAYOUT_GLOBAL
void   CGRectDivide( CGRect rect,
                     CGRect *slice, 
                     CGRect *remainder, 
                     CGFloat amount, 
                     CGRectEdge edge);


static inline CGRect   CGRect2DArrayGetRowColumn( CGRect *rects,
                                                  int row,
                                                  int column,
                                                  unsigned int columnSize)
{
   return( rects[ columnSize * row + column]);
}


static inline CGRect   *CGRect2DArrayGetRowColumnAddress( CGRect *rects,
                                                          int row,
                                                          int column,
                                                          unsigned int columnSize)
{
   return( &rects[ columnSize * row + column]);
}



// partition two dimensionally like 2x2 4x3 and so on
// rects must have enough space for rowSize * columnSize CGRects.
// rowSize is the number of rows, columnSize is the number of columns.
// (Not "size of a row".)
MULLE__LAYOUT_GLOBAL
void   CGRectPartitionIntoRowsAndColumns( CGRect frame,
                                          CGSize space,
                                          unsigned int rowSize,  
                                          unsigned int columnSize,
                                          CGRect *rects);

static inline void   CGRectPartitionVH( CGRect frame,
                                        CGSize space,
                                        unsigned int rowSize,  
                                        unsigned int columnSize,
                                        CGRect *rects)
{
   return( CGRectPartitionIntoRowsAndColumns( frame, space, rowSize, columnSize, rects));
}


static inline void   CGRectPartitionVerticallyAndHorizontally( CGRect frame,
                                                               CGSize space,
                                                               unsigned int rowSize,  
                                                               unsigned int columnSize,
                                                               CGRect *rects)
{
   return( CGRectPartitionIntoRowsAndColumns( frame, space, rowSize, columnSize, rects));
}


// some  conveniences for often used partitions
static inline void   CGRectPartition2x2( CGRect frame,
                                         CGSize space,
                                         CGRect rects[ 2][ 2]) // h w
{
   CGRectPartitionIntoRowsAndColumns( frame, space, 2, 2, (CGRect *) rects);
}


static inline void   CGRectPartition3x3( CGRect frame,
                                         CGSize space,
                                         CGRect rects[ 3][ 3]) // h w
{
   CGRectPartitionIntoRowsAndColumns( frame, space, 3, 3, (CGRect *) rects);
}

#endif
