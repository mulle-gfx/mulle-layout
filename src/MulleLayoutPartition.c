//
//  MulleLayoutPartition.c
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
#include "MulleLayoutPartition.h"

#include "include-private.h"


//
// void   CGRectSplitVertically( CGRect frame, CGRect *frame1, CGRect *frame2)
// {
//    frame1->origin      = frame.origin;
//    frame1->size.width  = frame.size.width;
//    frame1->size.height = frame.size.height / 2.0;
//
//    frame2->origin.x    = frame.origin.x;
//    frame2->origin.y    = frame.origin.y + frame1->size.height;
//    frame2->size        = frame1->size;
// }


void   CGRectPartitionVertically( CGRect frame,
                                  CGFloat space,
                                  uintptr_t n,
                                  CGRect *rects)
{
   CGRect  *p;
   CGRect  *q;
   CGRect  *sentinel;

   if( ! n || ! rects)
      return;

   q              = rects;
   q->origin      = frame.origin;
   q->size.width  = frame.size.width;
   q->size.height = (frame.size.height - (n - 1) * space) / n;
   if( q->size.height < 0.0)
      q->size.height = 0.0;

   p        = q + 1;
   sentinel = &q[ n];
   while( p < sentinel)
   {
      p->origin.x = frame.origin.x;
      p->origin.y = CGRectGetMaxY( *q) + space;
      p->size     = rects->size;

      q = p;
      ++p;
   }
}

//
// void   CGRectSplitHorizontally( CGRect frame, CGRect *frame1, CGRect *frame2)
// {
//    frame1->origin      = frame.origin;
//    frame1->size.width  = frame.size.width / 2.0;
//    frame1->size.height = frame.size.height;
//
//    frame2->origin.x    = frame.origin.x + frame1->size.width;
//    frame2->origin.y    = frame.origin.y;
//    frame2->size        = frame1->size;
// }
//

void   CGRectPartitionHorizontally( CGRect frame,
                                    CGFloat space,
                                    uintptr_t n,
                                    CGRect *rects)
{
   CGRect  *p;
   CGRect  *q;
   CGRect  *sentinel;

   if( ! n || ! rects)
      return;

   q              = rects;
   q->origin      = frame.origin;
   q->size.width  = (frame.size.width - (n - 1) * space) / n;
   if( q->size.width < 0.0)
      q->size.width = 0.0;
   q->size.height = frame.size.height;

   p        = q + 1;
   sentinel = &q[ n];
   while( p < sentinel)
   {
      p->origin.x =  CGRectGetMaxX( *q) + space;
      p->origin.y = frame.origin.y;
      p->size     = rects->size;

      q = p;
      ++p;
   }
}



int   CGRectSubdivideIfOverlap( CGRect rect, CGRect other, CGRect output[ 4])
{
   int            i;
   CGFloat        left_margin;
   CGFloat        right_margin;
   CGFloat        top_margin;
   CGFloat        bottom_margin;
   CGFloat        extent;
   CGFloat        width;

   // isn't this overkill ?
   rect  = CGRectStandardize( rect);
   other = CGRectStandardize( other);

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

   i = 0;

   extent = CGRectGetHeight( rect);
   width  = CGRectGetWidth( rect);

   if( ! (width > 0.0f && extent > 0.0f))
      return( -1);

   if( ! CGRectIntersectsRect( rect, other))
      return( -1);

   top_margin    = CGRectGetMinY( other) - CGRectGetMinY( rect);
   bottom_margin = CGRectGetMaxY( rect)  - CGRectGetMaxY( other);

   left_margin   = CGRectGetMinX( other) - CGRectGetMinX( rect);
   right_margin  = CGRectGetMaxX( rect)  - CGRectGetMaxX( other);

   //
   //    000000000  rect
   //    111111111
   //    111111111  other
   //
   if( top_margin > 0.0)
   {
      output[ i++] = CGRectMake( CGRectGetMinX( rect),
                                 CGRectGetMinY( rect),
                                 width,
                                 top_margin);
      extent -= top_margin;
   }
   else
      top_margin = 0.0;

   if( bottom_margin > 0.0)
   {
      output[ i++] = CGRectMake( CGRectGetMinX( rect),
                                 CGRectGetMaxY( rect) - bottom_margin,
                                 width,
                                 bottom_margin);
      extent -= bottom_margin;
   }

   if( extent > 0.0)
   {
      if( left_margin > 0.0)
      {
         output[ i++] = CGRectMake( CGRectGetMinX( rect),
                                    CGRectGetMinY( rect) + top_margin,
                                    left_margin,
                                    extent);
      }

      if( right_margin > 0.0)
      {
         output[ i++] = CGRectMake( CGRectGetMaxX( rect) - right_margin,
                                    CGRectGetMinY( rect) + top_margin,
                                    right_margin,
                                    extent);
      }
   }

   return( i);
}



unsigned int   CGRectSubdivide( CGRect rect, CGRect other, CGRect output[ 4])
{
   int   i;

   i = CGRectSubdivideIfOverlap( rect, other, output);
   if( i == -1)
   {
      output[ 0] = rect;
      return( 1);
   }
   return( i);
}


void   CGRectPartitionIntoRowsAndColumns( CGRect frame,
                                          CGSize space,
                                          unsigned int rowSize,  //
                                          unsigned int columnSize,
                                          CGRect *rects)
{
   CGRect         *rows;
   CGRect         *columns;
   unsigned int   i;

   if( ! rects || ! rowSize || ! columnSize)
      return;

   // put at end of rects
   rows    = &rects[ rowSize * columnSize - rowSize];
   columns = rects;

   CGRectPartitionVertically( frame, space.height, rowSize, rows);

   for( i = 0; i < rowSize; i++)
   {
      CGRectPartitionHorizontally( *rows, space.width, columnSize, columns);

      rows++;
      columns += columnSize;
   }
}


/**
 * This code is from Cocotron
 */
void   CGRectDivide( CGRect rect, 
                     CGRect *slice, 
                     CGRect *remainder, 
                     CGFloat amount, 
                     CGRectEdge edge) 
{
   assert( slice);
   assert( remainder);

    if( ! slice || ! remainder)
      return;

    if (CGRectIsNull(rect)) 
    {
        *slice     = CGRectNull;
        *remainder = CGRectNull;
        return;
    }

    amount = amount < 0.0f ? 0.0f : amount;
    rect   = CGRectStandardize(rect);

    /*
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

    // Set both to rect, then update the sizes.
    *slice = rect;
    *remainder = rect;

    switch (edge) {
        case CGRectMinYEdge:
            amount = (amount > rect.size.height) ? rect.size.height : amount;
            remainder->origin.y += amount;
            if (amount >= rect.size.height) {
                remainder->size.height = 0;
            } else {
                slice->size.height = amount;
                remainder->size.height -= amount;
            }
            break;
        case CGRectMaxYEdge:
            if (amount >= rect.size.height) {
                remainder->size.height = 0;
            } else {
                slice->origin.y += (rect.size.height - amount);
                slice->size.height = amount;
                remainder->size.height -= amount;
            }
            break;
        case CGRectMinXEdge:
            amount = (amount > rect.size.width) ? rect.size.width : amount;
            remainder->origin.x += amount;
            if (amount >= rect.size.width) {
                remainder->size.width = 0;
            } else {
                slice->size.width = amount;
                remainder->size.width -= amount;
            }
            break;
        case CGRectMaxXEdge:
            if (amount >= rect.size.width) {
                remainder->size.width = 0;
            } else {
                remainder->size.width -= amount;
                slice->origin.x += rect.size.width - amount;
                slice->size.width = amount;
            }
            break;
        default:
            // handle edge == 0 as MinXEdge for Apple CoreGraphics compatibility
            amount = (amount > rect.size.width) ? rect.size.width : amount;
            remainder->origin.x += amount;
            if (amount >= rect.size.width) {
                remainder->size.width = 0;
            } else {
                slice->size.width = amount;
                remainder->size.width -= amount;
            }
            break;
    }
}
