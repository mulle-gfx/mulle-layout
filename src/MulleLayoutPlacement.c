//
//  MulleLayoutPlacement.c
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
      origin.y = CGRectGetMaxY( other) - rect.size.height;
      break;
   }

   return( origin);
}



static MulleLayoutPlacement    menu_placement_preferences[] =
{
   MulleLayoutPlacementData( MulleLayoutPlacementLeftAligned,  MulleLayoutPlacementDown),
   MulleLayoutPlacementData( MulleLayoutPlacementLeftAligned,  MulleLayoutPlacementUp),
   MulleLayoutPlacementData( MulleLayoutPlacementRightAligned, MulleLayoutPlacementDown),
   MulleLayoutPlacementData( MulleLayoutPlacementRightAligned, MulleLayoutPlacementUp),
   MulleLayoutPlacementData( MulleLayoutPlacementCenter,       MulleLayoutPlacementDown),
   MulleLayoutPlacementData( MulleLayoutPlacementCenter,       MulleLayoutPlacementUp),
   MulleLayoutPlacementData( MulleLayoutPlacementCenter,       MulleLayoutPlacementMiddle)
};
#define N_MENU_PLACEMENT_PREFERENCES \
   (sizeof( menu_placement_preferences) / sizeof( menu_placement_preferences[ 0]))




CGPoint  CGRectPlaceNextToRectInSpace( CGRect rect,
                                       CGRect other,
                                       CGRect space,
                                       MulleLayoutPlacement *preferences,
                                       unsigned int n_preferences)

{
   MulleLayoutPlacement   *p;
   MulleLayoutPlacement   *sentinel;
   CGRect                 placed;

 
   if( ! preferences)
   {
      preferences   = menu_placement_preferences;
      n_preferences = N_MENU_PLACEMENT_PREFERENCES;
   }


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

   return( CGPointNull);
}



//
// position rect at either side of other, if space fits.
// Gives a list of MulleLayoutPlacement and they are tried until something
// fits.
//
int  CGRectFindBestPlacementNextToRectInSpace( CGRect rect,
                                               CGRect other,
                                               CGRect space,
                                               MulleLayoutPlacement *preferences,
                                               unsigned int n_preferences)

{
   MulleLayoutPlacement   *p;
   MulleLayoutPlacement   *sentinel;
   CGRect                 placed;
 
   if( ! preferences)
   {
      preferences   = menu_placement_preferences;
      n_preferences = N_MENU_PLACEMENT_PREFERENCES;
   }

   p        = preferences;
   sentinel = &p[ n_preferences];

   placed.size = rect.size;
   while( p < sentinel)
   {
      placed.origin = CGRectPlaceNextToRect( rect, other, *p);
      if( CGRectContainsRect( space, placed))
         return( p - preferences);
      ++p;
   }

   return( -1);
}



CGRect   CGSizePlaceAdjacentToRectInSpace( CGSize menuSize,
                                           CGRect buttonFrame,
                                           CGRect space,
                                           MulleLayoutPlacement *preferences,
                                           unsigned int n_preferences)
{
   CGRect                 menuFrame;
   MulleLayoutPlacement   placement;
   int                    index;

   menuFrame.origin = CGPointZero;
   menuFrame.size   = menuSize;

   //  0   4   8  12   16
   //  +---------------+
   //  | t :   o   : p |
   //  |...+-------+...| bounds
   //  |   | frame |   |
   //  |...+-------+...|
   //  |   :       :   |
   //  +---------------+
   if( ! preferences)
   {
      preferences   = menu_placement_preferences;
      n_preferences = N_MENU_PLACEMENT_PREFERENCES;
   }

   index = CGRectFindBestPlacementNextToRectInSpace( menuFrame,
                                                     buttonFrame,
                                                     space,
                                                     preferences,
                                                     n_preferences);
   if( index == -1)
   {
      if( ! n_preferences)
         return( CGRectNull);
      placement = preferences[ n_preferences - 1];
   }
   else
      placement = preferences[ index];

   menuFrame.origin = CGRectPlaceNextToRect( menuFrame, buttonFrame, placement);

   return( menuFrame);
}
