//
// Copyright (c) 2020 Nat!, Mulle kybernetiK
//
#ifndef mulle_corner_insets_h__
#define mulle_corner_insets_h__

#include "include.h"


typedef struct MulleCornerInsets
{
   float   tl;
   float   tr;
   float   br;
   float   bl;
} MulleCornerInsets;

#define MulleCornerInsetsData( tl, tr, br, bl)  { (tl), (tr), (br), (bl) }

#define MulleCornerInsetsZero   MulleEdgeInsetsMake( 0.0f, 0.0f, 0.0f, 0.0f)



static inline struct MulleCornerInsets
   MulleCornerInsetsMakeLRTD( float topLeft, float topRight, float bottomLeft, float bottomRight)
{
   return( (struct MulleCornerInsets) { .tl = topLeft, .tr = topRight, .br = bottomRight, .bl = bottomLeft });
}


static inline struct MulleCornerInsets
   MulleCornerInsetsMake( float topLeft, float topRight, float bottomRight, float bottomLeft)
{
   return( (struct MulleCornerInsets) { .tl = topLeft, .tr = topRight, .br = bottomRight, .bl = bottomLeft });
}


#endif
