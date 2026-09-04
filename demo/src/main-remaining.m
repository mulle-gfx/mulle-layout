#import "import.h"

#import <string.h>


static void   drawFilledRect( CGContext *context, CGRect rect, CGColor color)
{
   CGContextBeginPath( context);
   {
      CGContextRect( context, rect);
      CGContextFillColor( context, color);
   }
   CGContextFill( context);
}


static void   drawStrokedRect( CGContext *context, CGRect rect, CGColor color)
{
   CGContextBeginPath( context);
   {
      CGContextRect( context, rect);
      CGContextStrokeWidth( context, 1.0);
      CGContextStrokeColor( context, color);
   }
   CGContextStroke( context);
}



static void   drawStuff( CGGraphicsLayer *layer, void *context, BOOL reverse, struct MulleEdgeInsets insets)
{
   struct MulleLayout  layout;
   CGRect              container;
   CGRect              rect;

   //
   // CGRectMake( 100, 100, 300, 200)
   //
   container = CGRectMake( 0, 0, 300, 150);
   drawFilledRect( context, container, CGColorMakeWithRGBA( 0.66, 0.66, 0.66, 0.66));

   if( reverse)
      MulleLayoutInitReverse( &layout, container, MulleEdgeInsetsMake( 10, 15, 20, 25));
   else
      MulleLayoutInit( &layout, container, MulleEdgeInsetsMake( 10, 15, 20, 25));

   rect = MulleLayoutGetRemainingRowContentRect( &layout);
   drawStrokedRect( context, rect, CGColorMakeRed());

   rect = MulleLayoutAddToRow( &layout, CGSizeMake( 50, 50), MulleEdgeInsetsMake( 7, 14, 21, 28));
   drawFilledRect( context, rect, CGColorMakeWithRGBA( 0.0, 1.0, 0.0, 0.66));

   rect = MulleLayoutGetRemainingRowContentRect( &layout);
   drawStrokedRect( context, rect, CGColorMakeRed());

   rect = MulleLayoutAddToRow( &layout, CGSizeMake( 50, 50), insets);
   drawFilledRect( context, rect, CGColorMakeWithRGBA( 0.0, 0.0, 1.0, 0.66));

   rect = MulleLayoutGetRemainingRowContentRect( &layout);
   drawStrokedRect( context, rect, CGColorMakeYellow());

   MulleLayoutDone( &layout);
}




static BOOL   drawStuff1( CGGraphicsLayer *layer, CGContext *context)
{
   drawStuff( layer, context, NO, MulleEdgeInsetsMake( 7, 14, 21, 28));
   return( YES);
}
static BOOL   drawStuff2( CGGraphicsLayer *layer, CGContext *context)
{
   drawStuff( layer, context, NO, MulleEdgeInsetsMake( 20, 32, 0, 16));
   return( YES);
}
static BOOL   drawStuff3( CGGraphicsLayer *layer, CGContext *context)
{
   drawStuff( layer, context, YES, MulleEdgeInsetsMake( 7, 14, 21, 28));
   return( YES);
}
static BOOL   drawStuff4( CGGraphicsLayer *layer, CGContext *context)
{
   drawStuff( layer, context, YES, MulleEdgeInsetsMake( 20, 32, 0, 16));
   return( YES);
}


void   setupScene( UIWindowPlane *contentPlane)
{
   UIView            *view;
   CGRect            frame;
   UIEdgeInsets      insets;
   CGGraphicsLayer   *layer;
   CGRect            rects[ 2][ 2];

   insets = UIEdgeInsetsMake( 8, 8, 8, 8);
   frame  = UIEdgeInsetsInsetRect( [contentPlane frame], insets);

   CGRectPartition2x2( frame,
                       CGSizeMake( 8, 8),
                       rects);

   frame  = UIEdgeInsetsInsetRect( rects[ 0][ 0], insets);

   view  = [UIView viewWithFrame:frame];
   layer = MULLE_OBJC_CLASS_CAST( CGGraphicsLayer, [view layer]);
   [layer setDrawCallback:drawStuff1];
   [contentPlane addSubview:view];

   frame  = UIEdgeInsetsInsetRect( rects[ 0][ 1], insets);

   view  = [UIView viewWithFrame:frame];
   layer = MULLE_OBJC_CLASS_CAST( CGGraphicsLayer, [view layer]);
   [layer setDrawCallback:drawStuff2];
   [contentPlane addSubview:view];

   frame  = UIEdgeInsetsInsetRect( rects[ 1][ 0], insets);

   view  = [UIView viewWithFrame:frame];
   layer = MULLE_OBJC_CLASS_CAST( CGGraphicsLayer, [view layer]);
   [layer setDrawCallback:drawStuff3];
   [contentPlane addSubview:view];

   frame  = UIEdgeInsetsInsetRect( rects[ 1][ 1], insets);

   view  = [UIView viewWithFrame:frame];
   layer = MULLE_OBJC_CLASS_CAST( CGGraphicsLayer, [view layer]);
   [layer setDrawCallback:drawStuff4];
   [contentPlane addSubview:view];
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
      window = [UIWindow windowWithFrame:CGRectMake( 0.0, 0.0, 640.0, 400.0)];

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

