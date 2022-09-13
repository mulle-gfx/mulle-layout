//
// Copyright (c) 2022 Nat!, Mulle kybernetiK
//
#include "MulleGrid.h"

#include "include-private.h"


void   MulleGridComputeItemSize( struct MulleGrid *p)
{
   CGRect   rect;

   if( ! p || ! p->columns || ! p->rows)
      return;

   rect               = MulleEdgeInsetsInsetRect( p->canvasInsets, p->canvas);

   p->itemSize.width  = ((rect.size.width + p->spacing.width) / p->columns) - p->spacing.width;
   p->itemSize.height = ((rect.size.height + p->spacing.height) / p->rows) - p->spacing.height;
}


void   MulleGridComputeRowsColumns( struct MulleGrid *p)
{
   CGRect   rect;

   if( ! p || p->itemSize.width == 0.0 || p->itemSize.height == 0.0)
      return;

   rect       = MulleEdgeInsetsInsetRect( p->canvasInsets, p->canvas);

   p->columns = (unsigned int) floor( (rect.size.width + p->spacing.width) / 
                                       (p->itemSize.width + p->spacing.width));
   p->rows    = (unsigned int) floor( (rect.size.height + p->spacing.height) / 
                                      (p->itemSize.height + p->spacing.height));
}
