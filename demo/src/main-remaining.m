#import "import.h"

#import <string.h>


static void   drawFilledRect( NVGcontext *vg, CGRect rect, CGColor color)
{
   nvgBeginPath( vg);
   {
      nvgRect( vg, rect.origin.x,
                   rect.origin.y,
                   rect.size.width,
                   rect.size.height);
      nvgFillColor( vg, color);
   }
   nvgFill( vg);
}


static void   drawStrokedRect( NVGcontext *vg, CGRect rect, CGColor color)
{
   nvgBeginPath( vg);
   {
      nvgRect( vg, rect.origin.x,
                   rect.origin.y,
                   rect.size.width,
                   rect.size.height);
      nvgStrokeWidth( vg, 1.0);
      nvgStrokeColor( vg, color);
   }
   nvgStroke( vg);
}



static void   drawStuff( CALayer *layer, void *context, BOOL reverse, struct MulleEdgeInsets insets)
{
   struct MulleLayout  layout;
   CGRect              container;
   CGRect              rect;
   NVGcontext          *vg;

   vg = CGContextGetNVGContext( context);

   //
   // CGRectMake( 100, 100, 300, 200)
   //
   container = CGRectMake( 0, 0, 300, 150);
   drawFilledRect( vg, container, CGColorMakeRGBA( 0.66, 0.66, 0.66, 0.66));

   if( reverse)
      MulleLayoutInitReverse( &layout, container, MulleEdgeInsetsMake( 10, 15, 20, 25));
   else
      MulleLayoutInit( &layout, container, MulleEdgeInsetsMake( 10, 15, 20, 25));

   rect = MulleLayoutGetRemainingRowContentRect( &layout);
   drawStrokedRect( vg, rect, CGColorMakeRed());

   rect = MulleLayoutAddToRow( &layout, CGSizeMake( 50, 50), MulleEdgeInsetsMake( 7, 14, 21, 28));
   drawFilledRect( vg, rect, CGColorMakeRGBA( 0.0, 1.0, 0.0, 0.66));

   rect = MulleLayoutGetRemainingRowContentRect( &layout);
   drawStrokedRect( vg, rect, CGColorMakeRed());

   rect = MulleLayoutAddToRow( &layout, CGSizeMake( 50, 50), insets);
   drawFilledRect( vg, rect, CGColorMakeRGBA( 0.0, 0.0, 1.0, 0.66));

   rect = MulleLayoutGetRemainingRowContentRect( &layout);
   drawStrokedRect( vg, rect, CGColorMakeYellow());

   MulleLayoutDone( &layout);
}




static void   drawStuff1( CALayer *layer, void *context)
{
   drawStuff( layer, context, NO, MulleEdgeInsetsMake( 7, 14, 21, 28));
}
static void   drawStuff2( CALayer *layer, void *context)
{
   drawStuff( layer, context, NO, MulleEdgeInsetsMake( 20, 32, 0, 16));
}
static void   drawStuff3( CALayer *layer, void *context)
{
   drawStuff( layer, context, YES, MulleEdgeInsetsMake( 7, 14, 21, 28));
}
static void   drawStuff4( CALayer *layer, void *context)
{
   drawStuff( layer, context, YES, MulleEdgeInsetsMake( 20, 32, 0, 16));
}


void   setupScene( UIWindowPlane *contentPlane)
{
   UIView        *view;
   CGRect         frame;
   UIEdgeInsets   insets;
   CALayer        *layer;
   CGRect         rects[ 2][ 2];

   insets = UIEdgeInsetsMake( 8, 8, 8, 8);
   frame  = UIEdgeInsetsInsetRect( [contentPlane frame], insets);

   CGRectPartition2x2( frame,
                       CGSizeMake( 8, 8),
                       rects);

   frame  = UIEdgeInsetsInsetRect( rects[ 0][ 0], insets);

   view  = [UIView viewWithFrame:frame];
   layer = (CALayer *) [view layer];
   [layer setDrawCallback:drawStuff1];
   [contentPlane addSubview:view];

   frame  = UIEdgeInsetsInsetRect( rects[ 0][ 1], insets);

   view  = [UIView viewWithFrame:frame];
   layer = (CALayer *) [view layer];
   [layer setDrawCallback:drawStuff2];
   [contentPlane addSubview:view];

   frame  = UIEdgeInsetsInsetRect( rects[ 1][ 0], insets);

   view  = [UIView viewWithFrame:frame];
   layer = (CALayer *) [view layer];
   [layer setDrawCallback:drawStuff3];
   [contentPlane addSubview:view];

   frame  = UIEdgeInsetsInsetRect( rects[ 1][ 1], insets);

   view  = [UIView viewWithFrame:frame];
   layer = (CALayer *) [view layer];
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

      [app addWindow:window];

      [window setBackgroundColor:CGColorMakeWhite()];

      setupScene( [window contentPlane]);
   }
   return( [app run]);
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

