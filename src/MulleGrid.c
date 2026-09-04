//
//  MulleGrid.c
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
#include "MulleGrid.h"

#include "include-private.h"


enum
{
   MulleGridStateRows         = 0x1,
   MulleGridStateColumns      = 0x2,
   MulleGridStateItemWidth    = 0x4,
   MulleGridStateItemHeight   = 0x8,
   MulleGridStateCanvasWidth  = 0x10,
   MulleGridStateCanvasHeight = 0x20
};


static void   _MulleGridComputeItemWidth( struct MulleGrid *p, CGRect insetCanvas)
{
   assert( p->columns && p->rows);

   p->itemSize.width = (insetCanvas.size.width - (p->columns - 1) * p->spacing.width) / p->columns;
}

static void   _MulleGridComputeItemHeight( struct MulleGrid *p, CGRect insetCanvas)
{
   assert( p->columns && p->rows);

   p->itemSize.height = (insetCanvas.size.height - (p->rows - 1) * p->spacing.height) / p->rows;
}


static void   _MulleGridComputeItemSize( struct MulleGrid *p, CGRect insetCanvas)
{
   assert( p->columns && p->rows);

   p->itemSize.width  = (insetCanvas.size.width - (p->columns - 1) * p->spacing.width) / p->columns;
   p->itemSize.height = (insetCanvas.size.height - (p->rows - 1) * p->spacing.height) / p->rows;
}


static void   _MulleGridComputeColumns( struct MulleGrid *p, CGRect insetCanvas)
{
   CGFloat   value;

   if( p->itemSize.width + p->spacing.width <= 0.0)
   {
      p->columns = 0;
      return;
   }

   if( isinf( insetCanvas.size.width))
      p->columns = 0;
   else
   {
      value = floor( (insetCanvas.size.width + p->spacing.width) /
                     (p->itemSize.width + p->spacing.width));
      p->columns = value > 0.0 ? (uintptr_t) value : 0;
   }
}


static void   _MulleGridComputeRows( struct MulleGrid *p, CGRect insetCanvas)
{
   CGFloat   value;

   if( p->itemSize.height + p->spacing.height <= 0.0)
   {
      p->rows = 0;
      return;
   }

   if( isinf( insetCanvas.size.height))
      p->rows = 0;
   else
   {
      value = floor( (insetCanvas.size.height + p->spacing.height) /
                     (p->itemSize.height + p->spacing.height));
      p->rows = value > 0.0 ? (uintptr_t) value : 0;
   }
}



static void   _MulleGridComputeCanvasWidth( struct MulleGrid *p)
{
   if( ! p->columns)
   {
      p->canvas.size.width = 0.0;
      return;
   }
   p->canvas.size.width = (p->columns * p->itemSize.width)
                          + ((p->columns - 1) * p->spacing.width)
                          + p->insets.left + p->insets.right;
}


static void   _MulleGridComputeCanvasHeight( struct MulleGrid *p)
{
   if( ! p->rows)
   {
      p->canvas.size.height = 0.0;
      return;
   }
   p->canvas.size.height = (p->rows * p->itemSize.height)
                          + ((p->rows - 1) * p->spacing.height)
                          + p->insets.top + p->insets.bottom;
}

#if 0
static void   _MulleGridComputeCanvas( struct MulleGrid *p)
{
   assert( p->itemSize.width > 0.0 && p->itemSize.height > 0.0);
   assert( p->columns && p->rows);

   p->canvas.size.width  = (p->columns - 1) * (p->itemSize.width + p->spacing.width) + p->itemSize.width;
   p->canvas.size.height = (p->rows - 1) * (p->itemSize.height + p->spacing.height) + p->itemSize.height;
}
#endif


static void   _MulleGridComputeItemSizeAndRowsColumns( struct MulleGrid *p,
                                                       CGRect insetCanvas,
                                                       uintptr_t numberOfItems)
{
   uintptr_t   bestColumns;
   CGSize      bestItemSize;
   uintptr_t   bestRows;
   CGFloat     bestScore;
   uintptr_t   cols;
   CGFloat     itemAspect;
   CGSize      itemSize;
   uintptr_t   rows;
   CGFloat     score;
   CGFloat     utilization;

   numberOfItems = numberOfItems ? numberOfItems : 1;

   // guard: degenerate canvas can't produce a valid grid
   if( insetCanvas.size.width <= 0.0 || insetCanvas.size.height <= 0.0
       || isinf( insetCanvas.size.width) || isinf( insetCanvas.size.height))
   {
      p->columns         = 0;
      p->rows            = 0;
      p->itemSize.width  = -1.0;
      p->itemSize.height = -1.0;
      return;
   }

   bestScore    = -1.0;
   bestColumns  = 1;
   bestRows     = numberOfItems;
   bestItemSize = CGSizeMake( insetCanvas.size.width,
                              (insetCanvas.size.height - (numberOfItems - 1) * p->spacing.height)
                              / numberOfItems);

   //
   // Exhaustive search over all valid column counts.
   // Score prefers square items (aspect close to 1.0) and high utilization.
   //
   for( cols = 1; cols <= numberOfItems; cols++)
   {
      rows = (numberOfItems + cols - 1) / cols;
      if( ! rows)
         rows = 1;

      itemSize.width  = (insetCanvas.size.width - (cols - 1) * p->spacing.width) / cols;
      itemSize.height = (insetCanvas.size.height - (rows - 1) * p->spacing.height) / rows;

      // skip degenerate candidates where spacing exceeds canvas
      if( itemSize.width <= 0.0 || itemSize.height <= 0.0)
         continue;

      // utilization: fraction of canvas covered by items
      utilization = (itemSize.width * itemSize.height * numberOfItems)
                    / (insetCanvas.size.width * insetCanvas.size.height);

      // item aspect ratio closeness to 1.0 (square), range (0..1]
      itemAspect = itemSize.width / itemSize.height;
      if( itemAspect > 1.0)
         itemAspect = 1.0 / itemAspect;

      // combined score: both factors in [0..1], weighted equally
      score = utilization * 0.5 + itemAspect * 0.5;

      if( score > bestScore)
      {
         bestScore    = score;
         bestColumns  = cols;
         bestRows     = rows;
         bestItemSize = itemSize;
      }
   }

   p->columns  = bestColumns;
   p->rows     = bestRows;
   p->itemSize = bestItemSize;
}


static void   _MulleGridComputeItemSizeAndRows( struct MulleGrid *p,
                                                CGRect insetCanvas,
                                                uintptr_t numberOfItems)
{
   numberOfItems = numberOfItems ? numberOfItems : 1;
   p->rows       = (numberOfItems + p->columns - 1) / p->columns;
   if( ! p->rows)
      p->rows = 1;

   _MulleGridComputeItemSize( p, insetCanvas);
}


static void   _MulleGridComputeItemSizeAndColumns( struct MulleGrid *p,
                                                   CGRect insetCanvas,
                                                   uintptr_t numberOfItems)
{
   numberOfItems = numberOfItems ? numberOfItems : 1;
   p->columns    = (numberOfItems + p->rows - 1) / p->rows;
   if( ! p->columns)
      p->columns = 1;

   _MulleGridComputeItemSize( p, insetCanvas);
}



/**
 * @brief Initialize a MulleGrid structure with the given parameters
 *
 * This function sets up a grid layout based on the provided canvas, number of items,
 * item size, and spacing.
 *
 * @param p Pointer to the MulleGrid structure to be initialized
 * @param canvas The rectangular area in which to layout the grid
 * @param numberOfItems The number of items to be placed in the grid
 * @param itemSize The size of each item in the grid
 * @param spacing The spacing between items in the grid
 *
 * @note If itemSize.width is zero, the function calculates it based on the available height
 * @note If itemSize.height is zero, the function calculates it based on the available width
 * @note If canvas.size.height is zero, the function computes it based on rows and items
 * @note If canvas.size.width is zero, the function computes it based on columns and items
 *
 * You can't compute an itemSize dimension and a canvas dimension at the same time.
 * @return void
 */
int   _MulleGridConstruct( struct MulleGrid *p,
                           uintptr_t numberOfItems,
                           uintptr_t *state)
{
   CGFloat     n;
   uintptr_t   dummy;
   CGRect      insetCanvas;

   n      = numberOfItems ? (CGFloat) numberOfItems : 1.0f;

   state  = state ? state : &dummy;
   *state = 0;

   // this calculates easier...
   if( p->canvas.size.width == 0.0f)
      p->canvas.size.width = INFINITY;

   if( p->canvas.size.height == 0.0f)
      p->canvas.size.height = INFINITY;

   // we keep the canvas
   insetCanvas = MulleEdgeInsetsInsetRect( p->insets, p->canvas);

   if( p->itemSize.width == 0.0 && p->itemSize.height == 0.0)
   {
      *state |= MulleGridStateItemWidth;
      *state |= MulleGridStateItemHeight;

      // ok assume grid is calculated, calculate itemSize
      if( ! p->rows)
      {
         *state |= MulleGridStateRows;
         if( ! p->columns)
         {
            *state |= MulleGridStateColumns;
            _MulleGridComputeItemSizeAndRowsColumns( p, insetCanvas, numberOfItems);
         }
         else
            _MulleGridComputeItemSizeAndRows( p, insetCanvas, numberOfItems);
      }
      else
         if( ! p->columns)
         {
            *state |= MulleGridStateColumns;
            _MulleGridComputeItemSizeAndColumns( p, insetCanvas, numberOfItems);
         }
   }

   if( p->itemSize.width == 0.0)
   {
      *state |= MulleGridStateItemWidth;

      if( p->rows == 0)
      {
         *state |= MulleGridStateRows;
         _MulleGridComputeRows( p, insetCanvas);
         if( p->rows == 0)
            goto invalid;
      }

      if( p->columns == 0)
         p->columns = (uintptr_t) ceil( n / p->rows);

      _MulleGridComputeItemWidth( p, insetCanvas);
   }

   if( p->itemSize.height == 0.0)
   {
      *state |= MulleGridStateItemHeight;

      if( p->columns == 0)
      {
         *state    |= MulleGridStateColumns;
         _MulleGridComputeColumns( p, insetCanvas);
         if( p->columns == 0)
            goto invalid;
      }

      if( p->rows == 0)
         p->rows = (uintptr_t) ceil( n / p->columns);

      _MulleGridComputeItemHeight( p, insetCanvas);
   }

   // from here on
   if( p->itemSize.width <= 0.0f || p->itemSize.height <= 0.0f)
      goto invalid;

   if( ! p->columns)
   {
      *state |= MulleGridStateColumns;
      if( p->itemSize.width + p->spacing.width <= 0.0)
         goto invalid;

      _MulleGridComputeColumns( p, insetCanvas);

      if( ! p->columns)
         p->columns = 1;
   }

   if( ! p->rows)
   {
      *state |= MulleGridStateRows;
      if( p->itemSize.height + p->spacing.height <= 0.0)
         goto invalid;

      _MulleGridComputeRows( p, insetCanvas);

      if( ! p->rows)
         p->rows = 1;
   }

   if( numberOfItems != 0)
   {
      // only recompute rows if the caller did not specify them,
      // or if columns were also caller-specified (grid can grow vertically)
      if( (*state & MulleGridStateRows) || ! (*state & MulleGridStateColumns))
         p->rows = (uintptr_t) ceilf( n / p->columns);
   }

   // from here on, this can't really fail, its more for documentation and
   // later edits
   assert( p->columns != 0 && p->rows != 0);

   if( isinf( p->canvas.size.height) || isinf( p->canvas.size.width))
   {
#if 0
      mulle_fprintf( stdout, "< canvas   %.1f %.1f\n", p->canvas.size.width, p->canvas.size.height);
      mulle_fprintf( stdout, "< itemSize %.1f %.1f\n", p->itemSize.width, p->itemSize.height);
      mulle_fprintf( stdout, "< spacing  %.1f %.1f\n", p->spacing.width, p->spacing.height);
      mulle_fprintf( stdout, "< insets   %.1f %.1f %.1f %.1f\n", p->insets.top, p->insets.left, p->insets.bottom, p->insets.right);
      mulle_fprintf( stdout, "< row/col  %.tu %tu (%.1f)\n", p->rows, p->columns, n);
#endif
      if( isinf( p->canvas.size.width))
      {
         *state |= MulleGridStateCanvasWidth;
         _MulleGridComputeCanvasWidth( p);
      }

      if( isinf( p->canvas.size.height))
      {
         *state |= MulleGridStateCanvasHeight;
         _MulleGridComputeCanvasHeight( p);
      }
   }


   return( 1);

invalid:
   p->canvas = CGRectNull;
   return( 0);
}


int   _MulleGridVerify( struct MulleGrid *p)
{
   // sanity check, if we computed nothing everything is invalid
   if( ! p->columns || ! p->rows)
      return( 0);

   if( p->itemSize.width <= 0.0f || p->itemSize.height <= 0.0f)
      return( 0);
   if( isinf( p->itemSize.width) || isinf( p->itemSize.height))
      return( 0);
   if( isnan( p->itemSize.width) || isnan( p->itemSize.height))
      return( 0);

   if( p->canvas.size.width <= 0.0f || p->canvas.size.height <= 0.0f)
      return( 0);
   if( isinf( p->canvas.size.width) || isinf( p->canvas.size.height))
      return( 0);
   if( isnan( p->canvas.size.width) || isnan( p->canvas.size.height))
      return( 0);

   return( 1);
}


CGFloat   _MulleGridGetColumnX( struct MulleGrid *p, uintptr_t column)
{
   CGFloat   x;

   if( column == 0)
      return( p->canvas.origin.x);
   if( column == p->columns)
      return( CGRectGetMaxX( p->canvas));
   if( column > p->columns)
      return( NAN);

   x = p->canvas.origin.x + p->insets.left
       + (column * (p->itemSize.width + p->spacing.width))
       - p->spacing.width / 2.0;

   return( x);
}


CGFloat   _MulleGridGetRowY( struct MulleGrid *p, uintptr_t row)
{
   CGFloat   y;

   if( row == 0)
      return( p->canvas.origin.y);
   if( row == p->rows)
      return( CGRectGetMaxY( p->canvas));
   if( row > p->rows)
      return( NAN);

   y = p->canvas.origin.y + p->insets.top
       + (row * (p->itemSize.height + p->spacing.height))
       - p->spacing.height / 2.0;

   return( y);
}


CGRect   _MulleGridGetItemRect( struct MulleGrid *p,
                                uintptr_t row,
                                uintptr_t column)
{
   CGRect  rect;

   if( row >= p->rows || column >= p->columns)
      return( CGRectNull);

   rect.origin.x = p->canvas.origin.x + p->insets.left;
   if( column)
      rect.origin.x += column * (p->itemSize.width + p->spacing.width);

   rect.origin.y = p->canvas.origin.y + p->insets.top;
   if( row)
      rect.origin.y += row * (p->itemSize.height + p->spacing.height);

   rect.size = p->itemSize;

   return( rect);
}


// void   _MulleGridRoundToIntegralFuzzy( struct MulleGrid *p,
//                                        uintptr_t state,
//                                        CGFloat fuzz)
// {
//    assert( _MulleGridVerify( p));
//    CGRect            insetCanvas;
//
//    p->insets.top    = roundf( p->insets.top);
//    p->insets.left   = roundf( p->insets.left);
//    p->insets.bottom = roundf( p->insets.bottom);
//    p->insets.right  = roundf( p->insets.right);
//
//    p->spacing.width  = roundf( p->spacing.width);
//    p->spacing.height = roundf( p->spacing.height);
//
//    if( state & (MulleGridStateCanvasWidth|MulleGridStateCanvasHeight))
//    {
//       if( state & MulleGridStateCanvasWidth)
//          _MulleGridComputeCanvasWidth( p);
//       if( state & MulleGridStateCanvasHeight)
//          _MulleGridComputeCanvasHeight( p);
//    }
//
//    p->canvas.origin.x    = floorf( p->canvas.origin.x  + fuzz);
//    p->canvas.origin.y    = floorf( p->canvas.origin.y + fuzz);
//    p->canvas.size.width  = ceilf( p->canvas.size.width - fuzz);
//    p->canvas.size.height = ceilf( p->canvas.size.height - fuzz);
//
//    insetCanvas = MulleEdgeInsetsInsetRect( p->insets, p->canvas);
//
//    // if we changed the canvas, recompute rows/cols if needed
//    // trying to keep itemSize stable
//    if( state & MulleGridStateRows)
//       _MulleGridComputeRows( p, insetCanvas);
//    if( state & MulleGridStateItemHeight)
//       _MulleGridComputeItemHeight( p, insetCanvas);
//
//    if( state & MulleGridStateColumns)
//       _MulleGridComputeRows( p, insetCanvas);
//    if( state & MulleGridStateItemWidth)
//       _MulleGridComputeItemWidth( p, insetCanvas);
//
//    // for the grid we gotta floor always now
//    p->itemSize.width  = floorf( p->itemSize.width + fuzz);
//    p->itemSize.height = floorf( p->itemSize.height + fuzz);
//
//    if( state & MulleGridStateRows)
//       _MulleGridComputeRows( p, insetCanvas);
//    if( state & MulleGridStateColumns)
//       _MulleGridComputeRows( p, insetCanvas);
//
//    _MulleGridVerify( p);
// }


struct MulleGridPosition   _MulleGridGetPositionAtPoint( struct MulleGrid *p,
                                                          CGPoint point,
                                                          uintptr_t numberOfItems)
{
   CGFloat     x;
   CGFloat     y;
   CGFloat     cellWidth;
   CGFloat     cellHeight;
   intptr_t    col;
   intptr_t    row;
   intptr_t    index;

   // offset relative to content area (canvas + insets)
   x = point.x - (p->canvas.origin.x + p->insets.left);
   y = point.y - (p->canvas.origin.y + p->insets.top);

   if( x < 0.0f || y < 0.0f)
      return( MulleGridPositionInvalid);

   cellWidth  = p->itemSize.width + p->spacing.width;
   cellHeight = p->itemSize.height + p->spacing.height;

   if( cellWidth <= 0.0f || cellHeight <= 0.0f)
      return( MulleGridPositionInvalid);

   col = (intptr_t) (x / cellWidth);
   row = (intptr_t) (y / cellHeight);

   // check within item bounds (not in the spacing)
   if( x - col * cellWidth > p->itemSize.width)
      return( MulleGridPositionInvalid);
   if( y - row * cellHeight > p->itemSize.height)
      return( MulleGridPositionInvalid);

   if( col < 0 || (uintptr_t) col >= p->columns)
      return( MulleGridPositionInvalid);
   if( row < 0 || (uintptr_t) row >= p->rows)
      return( MulleGridPositionInvalid);

   if( numberOfItems)
   {
      index = row * (intptr_t) p->columns + col;
      if( (uintptr_t) index >= numberOfItems)
         return( MulleGridPositionInvalid);
   }

   return( MulleGridPositionMake( row, col));
}
