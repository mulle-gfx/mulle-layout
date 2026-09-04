#import "import.h"

#import <string.h>



static CGContentMode   contentModes[ 12] =
{
   CGContentModeScaleToFill,
   CGContentModeScaleAspectFit,
   CGContentModeScaleAspectFill,
   CGContentModeCenterPoint,

   CGContentModeTop,
   CGContentModeBottom,
   CGContentModeLeft,
   CGContentModeRight,

   CGContentModeTopLeft,
   CGContentModeTopRight,
   CGContentModeBottomLeft,
   CGContentModeBottomRight
};

#define N_CGContentModes  (sizeof( contentModes) / sizeof( contentModes[ 0]))



//
// draw a square in a rect, with the various contentmodes
//
void   drawContentMode( CGContext *context,
                        CGRect rect,
                        unsigned int contentModeIndex)
{
   CGSize   size;
   CGRect   smallRect;
   CGFloat  hue;

   /*
    * draw border
    */
   CGContextBeginPath( context);
   {
      CGContextRect( context, rect);
      CGContextStrokeWidth( context, 1.0);
      CGContextStrokeColor( context, CGColorMakeWithHexColor( 0x8080007F));
   }
   CGContextStroke( context);

   /*
    * smaller rect is half size
    */
   size.width  = rect.size.width  / 2.0;
   size.height = rect.size.height / 2.0 > size.width ? size.width : rect.size.height / 2.0;

   smallRect = CGSizePlaceInRect( size, rect, contentModes[ contentModeIndex]);
   CGContextBeginPath( context);
   {
      CGContextRoundedRect( context, smallRect, smallRect.size.width / 10.0);
      CGContextStrokeWidth( context, 2.0);
      hue = 0.2 + 0.6 * ((double) contentModeIndex / (N_CGContentModes - 1));
      CGContextStrokeColor( context, CGColorMakeWithHSLA( hue, 1.0, 0.7, 0.7));
   }
   CGContextStroke( context);
}

//
// draw two squares in a rect, with the various rectangle placements
//
void   drawRectPlacement( CGContext *context,
                          CGRect rect,
                          NSUInteger i)
{
   CGSize          size;
   CGRect          smallRect;
   CGRect          otherRect;
   CGFloat         hue;
   CGContentMode   contentMode;

   assert( i >= 0 && i <= 36);

   CGContextBeginPath( context);
   {
      CGContextRect( context, rect);
      CGContextStrokeWidth( context, 1.0);
      CGContextStrokeColor( context, CGColorMakeWithHexColor( 0x8080007F));
   }
   CGContextStroke( context);

   size.width  = rect.size.width  / 3.0;
   size.height = rect.size.height / 3.0 > size.width ? size.width : rect.size.height / 3.0;

   smallRect = CGSizePlaceInRect( size, rect, CGContentModeCenterPoint);

   CGContextBeginPath( context);
   {
      CGContextRoundedRect( context, smallRect, smallRect.size.width / 10.0);
      CGContextStrokeWidth( context, 2.0);
      hue = 0.0 + 0.5 * ((double) i / (36 - 1));
      CGContextStrokeColor( context, CGColorMakeWithHSLA( hue, 1.0, 0.7, 0.7));
   }
   CGContextStroke( context);

   size.width  = rect.size.width  / 4.0;
   size.height = rect.size.height / 4.0 > size.width ? size.width : rect.size.height / 4.0;

   otherRect.size   = size;
   otherRect.origin = rect.origin;

   contentMode      = MulleLayoutPlacementMake( i % 6, i / 6);
   otherRect.origin = CGRectPlaceNextToRect( otherRect, smallRect, contentMode);

   CGContextBeginPath( context);
   {
      CGContextRoundedRect( context, otherRect, otherRect.size.width / 10.0);
      hue = 0.5 + 0.5 * ((double) i / (36 - 1));
      CGContextFillColor( context, CGColorMakeWithHSLA( hue, 1.0, 0.7, 0.7));
   }
   CGContextFill( context);
}


static MulleLayoutPlacement   preferences[] =
{
   MulleLayoutPlacementData( MulleLayoutPlacementLeftAligned,  MulleLayoutPlacementDown),
   MulleLayoutPlacementData( MulleLayoutPlacementRightAligned, MulleLayoutPlacementUp),
   MulleLayoutPlacementData( MulleLayoutPlacementRightAligned, MulleLayoutPlacementDown),
   MulleLayoutPlacementData( MulleLayoutPlacementLeftAligned,  MulleLayoutPlacementUp),
   MulleLayoutPlacementData( MulleLayoutPlacementCenter,       MulleLayoutPlacementMiddle)
};

//
// draw two squares in a rect, with the various rectangle placements
//
void   drawRectSpacePlacement( CGContext *context,
                               CGRect rect,
                               NSUInteger contentModeIndex)
{
   CGSize   size;
   CGRect   smallRect;
   CGRect   otherRect;
   CGFloat  hue;

   CGContextBeginPath( context);
   {
      CGContextRect( context, rect);
      CGContextStrokeWidth( context, 1.0);
      CGContextStrokeColor( context, CGColorMakeWithHexColor( 0x8080007F));
   }
   CGContextStroke( context);

   size.width  = rect.size.width  / 3.0;
   size.height = rect.size.height / 3.0 > size.width ? size.width : rect.size.height / 3.0;

   smallRect = CGSizePlaceInRect( size, rect, contentModes[ contentModeIndex]);
   CGContextBeginPath( context);
   {
      CGContextRoundedRect( context, smallRect, smallRect.size.width / 10.0);
      CGContextStrokeWidth( context, 2.0);
      hue = 0.0 + 0.5 * ((double) contentModeIndex / (N_CGContentModes - 1));
      CGContextStrokeColor( context, CGColorMakeWithHSLA( hue, 1.0, 0.7, 0.7));
   }
   CGContextStroke( context);

   otherRect.origin      = rect.origin;
   otherRect.size.width  = rect.size.width * 2.0 / 3.0;
   otherRect.size.height = rect.size.height * 2.0 / 3.0 > otherRect.size.width
                             ? otherRect.size.width
                             : rect.size.height * 2.0 / 3.0;

   otherRect.origin = CGRectPlaceNextToRectInSpace( otherRect,
                                                    smallRect,
                                                    rect,
                                                    preferences,
                                                    sizeof( preferences) / sizeof( preferences[ 0]));
   assert( ! CGPointIsNull( otherRect.origin));

   CGContextBeginPath( context);
   {
      CGContextRoundedRect( context, otherRect, otherRect.size.width / 10.0);
      hue = 0.0 + 0.5 * ((double) contentModeIndex / (N_CGContentModes - 1));
      CGContextFillColor( context, CGColorMakeWithHSLA( hue, 1.0, 0.7, 0.7));
   }
   CGContextFill( context);
}


static void   drawLayoutInSpace( CGContext *context, CGRect rect)
{
   struct MulleLayout  layout;
   CGRect              part;
   BOOL                flag;
   NSUInteger          i;

   MulleLayoutInit( &layout, rect, UIEdgeInsetsMake( 3, 3, 3, 3));

   CGContextBeginPath( context);
   {
      CGContextRect( context, rect);
      CGContextStrokeWidth( context, 1.0);
      CGContextStrokeColor( context, CGColorMakeWithHexColor( 0x8080007F));
   }
   CGContextStroke( context);

   i    = 0;
   flag = NO;
   for(;;)
   {
      part = MulleLayoutAddToRow( &layout,
                                  CGSizeMake( (rect.size.width - 6.0 - 5 * 10.0) / 6.0,
                                              ((rect.size.height - 6.0) * (i + 1) / 6.0) / 3.0),
                                  MulleEdgeInsetsMake( 0, 0, 0, 10));
      if( part.size.width == 0.0f)
      {
         if( flag == YES)
            break;
         flag = YES;
         i = 0;
         MulleLayoutNewRow( &layout);
         continue;
      }

      flag = NO;
      CGContextBeginPath( context);
      {
         CGContextRect( context, part);
         CGContextStrokeWidth( context, 1.0);
         CGContextStrokeColor( context, CGColorMakeWithHSLA( fmod( 0.13 * i, 1.0), 1.0, 0.2, 0.7));
      }
      CGContextStroke( context);
      ++i;
   }

   MulleLayoutDone( &layout);
}


#define N_ROWS 4


static BOOL  drawStuff( CGGraphicsLayer *layer, CGContext *context)
{
   CGRect              rows[ N_ROWS];
   CGRect              subrows[ 2];
   CGRect              rects[ N_CGContentModes > 18 ? N_CGContentModes : 18];
   NSUInteger          i;
   NSUInteger          j;
   UIEdgeInsets        insets;
   CGRect              canvas;

   canvas = [layer canvas];
   CGRectPartitionVertically( canvas, 0, N_ROWS, rows);

   //
   //  test CGContentMode
   //
   CGRectPartitionHorizontally( rows[ 0], 0, N_CGContentModes, rects);

   for( i = 0; i < N_CGContentModes; i++)
   {
      insets = UIEdgeInsetsMake( 4, 4, 4, 4);
      drawContentMode( context, UIEdgeInsetsInsetRect( rects[ i], insets), i);
   }

   //
   // test rectangle placement
   //
   CGRectPartitionVertically( rows[ 1], 0, 2, subrows);
   for( j = 0; j < 2; j++)
   {
      CGRectPartitionHorizontally( subrows[ j], 0, 18, rects);
      for( i = 0; i < 18; i++)
      {
         drawRectPlacement( context, UIEdgeInsetsInsetRect( rects[ i], insets), j * 18 + i);
      }
   }

   //
   // test rectangle placement in space
   //
   CGRectPartitionHorizontally( rows[ 2], 0, N_CGContentModes - CGContentModeTop, rects);
   for( i = CGContentModeTop; i < N_CGContentModes; i++)
   {
      drawRectSpacePlacement( context,
                              UIEdgeInsetsInsetRect( rects[ i - CGContentModeTop], insets),
                              i);
   }

   //
   // test layout struct
   //
   drawLayoutInSpace( context, UIEdgeInsetsInsetRect( rows[ 3], insets));
   return( YES);
}


void   setupScene( UIView *contentView)
{
   UIView         *view;
   CGRect         frame;
   UIEdgeInsets   insets;

   assert( contentView);

   frame  = [contentView contentRect];
   insets = UIEdgeInsetsMake( 8, 8, 8, 8);
   frame  = UIEdgeInsetsInsetRect( frame, insets);

   view   = [UIView viewWithFrame:frame];
   [view setNameUTF8String:"me"];
   [view setDrawCallback:drawStuff];
   [contentView addSubview:view];
}


int   main()
{
   UIWindow        *window;
   UIApplication   *app;
   /*
    * window and app
    */
   app = [UIApplication sharedInstance];
   @autoreleasepool
   {
      window = [UIWindow windowWithFrame:CGRectMake( 0.0, 0.0, 800.0, 600.0)];
      [window setBackgroundColor:CGColorMakeWhite()];

      setupScene( [window contentView]);

      [app addWindow:window];
   }
   return( [app runApp]);
}




//
//                           ,╓≤æKª╨╙╙▓█D"╙╙╙*╨¥W▄╓,
//                      ╓▄ª╙└╧▓        █           ⁿ▓▓ª╦w
//                  ╓#╜^*█⌐   `╕       █           ╓▌    `╙%w
//               ╓ΦΓ      █    ╙⌐     ,█,,,Φ      ╓▌         ╙¥▄
//            ,▄▀ █▄       █µ,╓╝,╓≤╗≡≡ª╨╨╨¥≡∞╗w╓,ª█,  ,       ]▄▀V,
//          ,Φ└   ║"▀▄     ,▄▀╨"                 "╙¥▓▓       ▄▀  º▄¥,
//        ,#▌Φ▓▄╓  Φ `█Q▄M╙                           ╙¥▄ ,▄▀ ╙▄⌐`  `Φ
//       ▄▀   `V,"▀▀¼╓╩└                                 ╙¥▓╗ ,       ╙▄
//      Φ       `¥═▄▀                            ╒          ▀▄          ▓
//    .▓         ╓▓]V▄H       ▓                 ,Å            ▀,         ▀
//    █      ,-ª▓╛  `██▀½      "▄  ,╓╓╓╓;╓▄▄▄▄╗▄█,             ╙▄         ▀
//   █  `██▄   ▓. - Φ█═.~     ,≤█▄▀-      `   █└"▀▀▀▀▄╓╓╓╓       ▌    ██▄, █
//  ║⌐  " ▀   ▓╨⌐   }▀`      ,█  `           █      .█▀▄▄███▄     ▌  ` ▀    ▌
//  ▌        ▐M A   '┴      ▌                ▀        ╙█▌████▌    ╙▄        ║⌐
// ║⌐        █⌐'║▄ Γr2«    ║▀                 ▄▄▄      ╙▌▄███      █         ▌
// █]     ,▄▐H  ██████═ª  ╓` ▄█▀▀▀▀█▄        ████     ╓███` `╙╙║█  ▐H        █
// █ "% ,▄▀ ║░╠K█═╣███^   ╛╓█▀      ▐█         ,╓╓═ª╙└   █      ╙█▄▐▌▓######▌╫
// █║▄▄▄█▄▄▄║H:  "╙▀███▄╓▄,█▌╓▄,,▄∩  ██p$ΦΦ▀▀▀▀▀ΦΦΦR≡≥▄╓ ║▄       ╙j▌` ▄▀╙╗ `╫
// █"      ╙▐H-═M ▄███████¥█H "^╙    '█, ,«^``     `     ║▌        ▐M▄▀    "Φ█
// ║        !█▄▄████≤═^`╓█ █▌╓▀╦▄▀█   ╙█⌐               ╓█▌        ▓ `       █
//  ▌      ,▄██╫██ ▄█═╜" █▌▐█╣Hj⌐ █    █▌╙¥w,           ██▄       ╓▌        ▐M
//  ╢"▄▄#▀▀╙╟█Ü███▀`▀█  ║██J█▄▌jH █    ██    ╙▀W▄;    ,██  `╙w,   █▄     ,╓ █
//   █╙     ▐███▌▀█w ▀█ ╠██████j▌║█▌   ▀██▄       `╙▓██▀w,     ╙╗█'`"▀█▀╙"`║`
//   ╙▌     Φ████████▄██ ███████████    ═██▀W▄,   ╓█▀└         ╓▀ "    ╙▄ ╓▀
//    ╙▄     ╙▀███╙█▄▐██▄,╙▀█████▄▀▄▀▄   ,█M╗ ▀▄██▀╙""*ⁿ═─≈-,,▄▌,        ▄▀
//     `▌╙µ,▄Φ▀x███▄██████████▀V███▀╙└ ,▄#▌████▀`"*«,      .█▀▄▀▄,      ▄╛
//       ▀Ö█  ;▓████████▄▄█▀▄╙W "V▄██▀▀│▄███▀└ ▀╦     "*═╥▄▀ ║,  ║▀▄  ,▓
//        ╙W▀ '█████╨▀▀█████████   "▀▀████└      ╙▄   ,▄▀'   `▀▀╣H ▐▀▄▀
//          ╙▄  ▀▀Γ╓▌  ▄▀ⁿ`╙▀███      ██▌         ╓█R▀▄A█        ▀▀▄▀
//            ╙W,Φ╙  ║█        ▄╙╙ª%≡╦██≤≤╗╗≡≡╜▀▀   ╣▌   █╕     ,▄▀
//              `▀▄M█┘        ║▀▌   ╫"    *Φ▄  █       Φ` ▀▌, ▄M╙
//                 "▀╦,       M ╙▌  ▀       `█W▐▌       wæQ▄▀╙
//                     ╙▀W╥,╓╣   ╙█║        ╫M  █    ,╓▄╝╙`
//                          ╙╙▀%▄▄▄▌,≤▄_▄≡,▐▓▓▄▄#ª▀"`

