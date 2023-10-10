//
// Copyright (c) 2020 Nat!, Mulle kybernetiK
//
#ifndef mulle_corner_insets_h__
#define mulle_corner_insets_h__

#include "include.h"


typedef struct MulleCornerInsets
{
   float   topLeft;
   float   topRight;
   float   bottomRight;
   float   bottomLeft;
} MulleCornerInsets;

#define MulleCornerInsetsData( tl, tr, br, bl)  { (tl), (tr), (br), (bl) }

#define MulleCornerInsetsZero  \
   ((struct MulleCornerInsets) {  0.0f, 0.0f, 0.0f, 0.0f })


static inline struct MulleCornerInsets
   MulleCornerInsetsMakeTLRBLR( float topLeft, float topRight, float bottomLeft, float bottomRight)
{
   return( (struct MulleCornerInsets) { .topLeft     = topLeft, 
                                        .topRight    = topRight, 
                                        .bottomRight = bottomRight, 
                                        .bottomLeft  = bottomLeft 
                                      });
}


// order as in nanovg nvgRoundedRectVarying
static inline struct MulleCornerInsets
   MulleCornerInsetsMake( float topLeft, float topRight, float bottomRight, float bottomLeft)
{
   return( (struct MulleCornerInsets) { .topLeft     = topLeft, 
                                        .topRight    = topRight, 
                                        .bottomRight = bottomRight, 
                                        .bottomLeft  = bottomLeft 
                                      });
}


static inline struct MulleCornerInsets
   MulleCornerInsetsMakeWithCornerRadius( float r)
{
   return( (struct MulleCornerInsets) { .topLeft     = r, 
                                        .topRight    = r, 
                                        .bottomRight = r, 
                                        .bottomLeft  = r
                                      });
}


#endif
