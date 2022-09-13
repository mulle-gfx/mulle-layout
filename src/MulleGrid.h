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
// uses less space, but needs more CPU time.

struct MulleGrid
{
   CGRect            canvas;
   MulleEdgeInsets   canvasInsets;
   unsigned int      rows;
   unsigned int      columns;
   CGSize            itemSize;
   CGSize            spacing;
};


void   MulleGridComputeItemSize( struct MulleGrid *p);
void   MulleGridComputeRowsColumns( struct MulleGrid *p);


static inline void   MulleGridInitWithRowsColumns( struct MulleGrid *p, 
                                                   CGRect canvas, 
                                                   MulleEdgeInsets canvasInsets,
                                                   unsigned int rows,
                                                   unsigned int columns,
                                                   CGSize spacing)
{
   if( p)
   {
      p->canvas       = canvas;
      p->canvasInsets = canvasInsets;
      p->rows         = rows;
      p->columns      = columns;
      p->spacing      = spacing;

      MulleGridComputeItemSize( p);
   }
}     


static inline void   MulleGridInitWithItemSize( struct MulleGrid *p, 
                                                CGRect canvas, 
                                                MulleEdgeInsets canvasInsets,
                                                CGSize itemSize,
                                                CGSize spacing)
{
   if( p)
   {
      p->canvas       = canvas;
      p->canvasInsets = canvasInsets;
      p->itemSize     = itemSize;
      p->spacing      = spacing;

      MulleGridComputeRowsColumns( p);
   }
}     


static inline void   MulleGridDone( struct MulleGrid *p)
{
} 



static inline CGRect   MulleGridGetItemRect( struct MulleGrid *p, 
                                             unsigned int row,
                                             unsigned int column)
{
   CGRect   rect;

   if( ! p || row >= p->rows || column >= p->columns)
      return( CGRectNull);

   rect           = MulleEdgeInsetsInsetRect( p->canvasInsets, p->canvas);
   rect.origin.x += (p->itemSize.width  + p->spacing.width) * column; 
   rect.origin.y += (p->itemSize.height + p->spacing.height) * row; 
   rect.size      = p->itemSize;
   return( rect);
}

#endif
