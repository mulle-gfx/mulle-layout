//
// Copyright (c) 2021 Nat!, Mulle kybernetiK
//
#include "MulleLayoutPlacement.h"

#include "include-private.h"



//
// placement allows rect to keep current origin in one of the dimensions
// assume "placement" is a constant, then this all boils down to very little
//
CGPoint   CGRectPlaceNextToRect( CGRect rect,
                                 CGRect other,
                                 MulleLayoutPlacement placement)
{
   CGPoint      origin;

   assert( placement <= MulleLayoutPlacementMake( MulleLayoutPlacementRightAligned,
                                                  MulleLayoutPlacementBottomAligned));

   switch( MulleLayoutPlacementGetHorizontalPlacement( placement))
   {
   default :
      origin.x = rect.origin.x;
      break;

   case MulleLayoutPlacementLeft :
      origin.x = CGRectGetMinX( other) - rect.size.width;
      break;

   case MulleLayoutPlacementRight :
      origin.x = CGRectGetMaxX( other);
      break;

   case MulleLayoutPlacementMiddle :
      origin.x = CGRectGetMidX( other) - rect.size.width / 2;
      break;

   case MulleLayoutPlacementLeftAligned :
      origin.x = other.origin.x;
      break;

   case MulleLayoutPlacementRightAligned :
      origin.x =  CGRectGetMaxX( other) - rect.size.width;
      break;
   }

   switch( MulleLayoutPlacementGetVerticalPlacement( placement))
   {
   default :
      origin.y = rect.origin.y;
      break;

   case MulleLayoutPlacementUp :
      origin.y = CGRectGetMinY( other) - rect.size.height;
      break;

   case MulleLayoutPlacementDown :
      origin.y = CGRectGetMaxY( other);
      break;

   case MulleLayoutPlacementCenter :
      origin.y = CGRectGetMidY( other) - rect.size.height / 2;
      break;

   case MulleLayoutPlacementTopAligned :
      origin.y = other.origin.y;
      break;

   case MulleLayoutPlacementBottomAligned :
      origin.y =  CGRectGetMaxY( other) - rect.size.height;
      break;
   }

   return( origin);
}


//
// position rect at either side of other, if space fits.
// Gives a list of MulleLayoutPlacement and they are tried until something
// fits.
//
CGPoint   CGRectPlaceNextToRectInSpace( CGRect rect,
                                        CGRect other,
                                        CGRect space,
                                        uintptr_t n_preferences,
                                        unsigned char *preferences,
                                        intptr_t fallback)

{
   unsigned char   *p;
   unsigned char   *sentinel;
   CGRect          placed;

   assert( n_preferences);
   assert( fallback < 0 || fallback < n_preferences);

   p        = preferences;
   sentinel = &p[ n_preferences];

   placed.size = rect.size;
   while( p < sentinel)
   {
      placed.origin = CGRectPlaceNextToRect( rect, other, *p);
      if( CGRectContainsRect( space, placed))
         return( placed.origin);
      ++p;
   }
   if( fallback >= 0)
      return( CGRectPlaceNextToRect( rect, other, preferences[ fallback]));

   return( CGPointMake( CGFLOAT_MIN, CGFLOAT_MIN));
}
