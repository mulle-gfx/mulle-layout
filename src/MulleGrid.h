//
//  MulleGrid.h
//  mulle-layout
//
//  Copyright (c) 2022 Nat! - Mulle kybernetiK.
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
#ifndef mulle_grid_h__
#define mulle_grid_h__

#include "include.h"

#include "MulleEdgeInsets.h"

//
// A grid is a representation of an area subdivided into rectangles of
// equal size. Basically like a spreadsheet.
// It is  dynamic alternative to CGRectPartitionIntoRowsAndColumns. MulleGrid 
// uses less space, but needs more CPU time. Where as in
// CGRectPartitionIntoRowsAndColumns, you need to know the rows and columns
// before hand, with MulleGrid you can get one of these values calculated.
//
// You setup a MulleGrid with the parameters you know, and then run
// MulleGridConstruct. This will try to fill in the missing values. If the
// grid can't be constructed, the canvas will be set to CGRectNull.
//
// A valid grid will always have at least one row and one column.
// The origin of the canvas is only used for calculating
// grid item frames.
//
struct MulleGrid
{
   CGRect            canvas;
   MulleEdgeInsets   insets;
   uintptr_t         rows;
   uintptr_t         columns;
   CGSize            itemSize;
   CGSize            spacing;
   uintptr_t         _internal;
};


/// Initializes a `MulleGrid` struct with the provided parameters.
///
/// - Parameters:
///   - p: A pointer to the `MulleGrid` struct to initialize.
///   - canvas: The canvas rectangle for the grid.
///   - insets: The edge insets to apply within the canvas.
///   - itemSize: The size of each grid item (0 in a dimension to compute it).
///   - rows: The number of rows in the grid (0 to compute).
///   - columns: The number of columns in the grid (0 to compute).
///   - spacing: The spacing between grid items.
///
/// This function sets the fields of the `MulleGrid` struct with the provided
/// parameters. If `p` is `NULL`, the function does nothing. You then can call
/// MulleGridConstruct to fill in missing values.
///
static inline void   MulleGridInit( struct MulleGrid *p,
                                    CGRect canvas,
                                    MulleEdgeInsets insets,
                                    CGSize itemSize,
                                    uintptr_t rows,
                                    uintptr_t columns,
                                    CGSize spacing)
{
   if( p)
   {
      p->canvas       = CGRectStandardize( canvas);
      p->insets       = insets;
      p->itemSize     = itemSize;
      p->rows         = rows;
      p->columns      = columns;
      p->spacing      = spacing;
      p->_internal    = 0;
   }
}     

MULLE__LAYOUT_GLOBAL
int   _MulleGridVerify( struct MulleGrid *p);

static inline int   MulleGridVerify( struct MulleGrid *p)
{
   if( ! p)
      return( 0);
   return( _MulleGridVerify( p));
}



static inline int   MulleGridIsValid( struct MulleGrid *p)
{
   return( p && ! CGRectIsNull( p->canvas));
}



static inline void   MulleGridDone( struct MulleGrid *p)
{
   MULLE_C_UNUSED( p);
}


MULLE__LAYOUT_GLOBAL
int   _MulleGridConstruct( struct MulleGrid *p,
                           uintptr_t numberOfItems,
                           uintptr_t *state);


/// Constructs a `MulleGrid` struct with the provided number of items.
/// Filling out missing values.
///
/// - Parameters:
///   - p: A pointer to the `MulleGrid` struct to construct.
///   - numberOfItems: The number of items to allocate space for in the grid.
///   - state: Optional pointer receiving a bitmask of which values were computed.
///
/// Example:
///   Set up a grid for 8 columns on a 1024x768 screen with 10px spacing.
///   Each row is 100px high.
///
///   struct MulleGrid grid;
///   uintptr_t        state;
///
///   MulleGridInit( &grid, CGRectMake( 0, 0, 1024, 768),
///                         MulleEdgeInsetsZero,
///                         CGSizeMake( 0, 100),
///                         0,
///                         8,
///                         CGSizeMake( 10, 10));
///   MulleGridConstruct( &grid, 0, &state);
///
static inline int   MulleGridConstruct( struct MulleGrid *p,
                                        uintptr_t numberOfItems,
                                        uintptr_t *state)
{
   if( ! p)
      return( 0);

   if( ! _MulleGridConstruct( p, numberOfItems, state) ||
       ! _MulleGridVerify( p))
   {
      p->canvas = CGRectNull;
      return( 0);
   }
   return( 1);
}


/*
 *
 */

//
// if you want "nicey" rect coordinates run CGRectIntegral on the
// returned value
//
MULLE__LAYOUT_GLOBAL
CGRect   _MulleGridGetItemRect( struct MulleGrid *p,
                                uintptr_t row,
                                uintptr_t column);


static inline CGRect   MulleGridGetItemRect( struct MulleGrid *p,
                                             uintptr_t row,
                                             uintptr_t column)
{
   if( ! p)
      return( CGRectNull);

   return( _MulleGridGetItemRect( p, row, column));
}



static inline CGRect   MulleGridGetItemRectAtIndex( struct MulleGrid *p,
                                                    uintptr_t i)
{
   if( ! p || ! p->columns)
      return( CGRectNull);

   return( _MulleGridGetItemRect( p, i / p->columns, i % p->columns));
}


/// Returns the total number of item slots in the grid (rows * columns).
/// Returns 0 for an unconstructed or invalid grid.
static inline uintptr_t   MulleGridGetCapacity( struct MulleGrid *p)
{
   if( ! p || ! p->columns || ! p->rows)
      return( 0);
   return( p->rows * p->columns);
}



MULLE__LAYOUT_GLOBAL
CGFloat   _MulleGridGetColumnX( struct MulleGrid *p, uintptr_t column);


static inline CGFloat   MulleGridGetColumnX( struct MulleGrid *p, uintptr_t column)
{
   if( ! p)
      return( NAN);

   return( _MulleGridGetColumnX( p, column));
}


MULLE__LAYOUT_GLOBAL
CGFloat   _MulleGridGetRowY( struct MulleGrid *p, uintptr_t row);


static inline CGFloat   MulleGridGetRowY( struct MulleGrid *p, uintptr_t row)
{
   if( ! p)
      return( NAN);

   return( _MulleGridGetRowY( p, row));
}


//
// Grid position for hit testing. row and column are -1 when the point
// doesn't hit any item.
//
typedef struct MulleGridPosition
{
   intptr_t   row;
   intptr_t   column;
} MulleGridPosition;


#define MulleGridPositionMake( r, c)  ((struct MulleGridPosition){ .row = (r), .column = (c) })
#define MulleGridPositionInvalid      MulleGridPositionMake( -1, -1)


static inline int   MulleGridPositionIsValid( struct MulleGridPosition pos)
{
   return( pos.row >= 0 && pos.column >= 0);
}


//
// Returns the grid position (row, column) of the item at `point`.
// Returns MulleGridPositionInvalid if the point is outside any item
// (in spacing, insets, or out of bounds).
// If numberOfItems > 0, positions beyond that count also return invalid.
//
MULLE__LAYOUT_GLOBAL
struct MulleGridPosition   _MulleGridGetPositionAtPoint( struct MulleGrid *p,
                                                         CGPoint point,
                                                         uintptr_t numberOfItems);


static inline struct MulleGridPosition
   MulleGridGetPositionAtPoint( struct MulleGrid *p,
                                CGPoint point,
                                uintptr_t numberOfItems)
{
   if( ! p)
      return( MulleGridPositionInvalid);

   return( _MulleGridGetPositionAtPoint( p, point, numberOfItems));
}


static inline intptr_t
   MulleGridGetIndexAtPoint( struct MulleGrid *p,
                             CGPoint point,
                             uintptr_t numberOfItems)
{
   struct MulleGridPosition   pos;

   pos = MulleGridGetPositionAtPoint( p, point, numberOfItems);
   if( ! MulleGridPositionIsValid( pos))
      return( -1);
   return( pos.row * (intptr_t) p->columns + pos.column);
}


#endif
