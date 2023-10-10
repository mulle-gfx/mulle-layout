//
// Copyright (c) 2021 Nat!, Mulle kybernetiK
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

MULLE__GEOMETRY_EXTERN_GLOBAL
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
MULLE__GEOMETRY_EXTERN_GLOBAL
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


// The original
//
// and assume other is smack dab in the middle of rect
//
//    000000000
//    111   222
//    333333333
//
// assume other is overlapping middle and part of the bottom
//
//    000000000
//    111   222
//    111   222
//
// assume other is overlapping middle and part of the bottom
//
//    000000000
//    111
//    111
//
// returns the number of rects created. If there is no overlap, you get
// the original rect back.
//
MULLE__GEOMETRY_EXTERN_GLOBAL
unsigned int   CGRectSubdivide( CGRect rect, CGRect other, CGRect output[ 4]);


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
MULLE__GEOMETRY_EXTERN_GLOBAL
void   CGRectDivide( CGRect rect,
                     CGRect* slice, 
                     CGRect* remainder, 
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
// rects must have enough space for verticalCount * horizontalCount
// CGRects
MULLE__GEOMETRY_EXTERN_GLOBAL
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
