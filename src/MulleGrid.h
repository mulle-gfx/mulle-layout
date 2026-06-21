//
// Copyright (c) 2022 Nat!, Mulle kybernetiK
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
///   - itemSize: The size of each grid item.
///   - rows: The number of rows in the grid.
///   - columns: The number of columns in the grid.
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
///
/// Example:
///   Set up a grid for 8 columns on a 1024x768 screen with 10px spacing.
///   Each row is 100px high.
///   struct MulleGrid grid;
///   MulleGridInit( &grid, CGRectMake(0, 0, 1024, 768),
///                         CGSizeMake( 0, 100),
///                         0,
///                         8,
///                         CGSizeMake(10, 10));
///   MulleGridConstruct(&grid, 0);
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
   if( ! p)
      return( CGRectNull);

   return( _MulleGridGetItemRect( p, i / p->columns, i % p->columns));
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


#endif
