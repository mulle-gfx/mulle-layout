#import "import.h"

#import <string.h>


#define HZ  60.0f


static void   nvgStrokeGrid( NVGcontext *vg, struct MulleGrid *_grid)
{
   NSUInteger   i;
   CGFloat      x;
   CGFloat      y;

   nvgBeginPath(vg);

   // Draw vertical lines
   for( i = 0; i <= _grid->columns; i++)
   {
      x = MulleGridGetColumnX( _grid, i);

      nvgMoveTo( vg, x, CGRectGetMinY( _grid->canvas));
      nvgLineTo( vg, x, CGRectGetMaxY( _grid->canvas));
   }

   // Draw horizontal lines
   for( i = 0; i <= _grid->rows; i++)
   {
      y = MulleGridGetRowY( _grid, i);

      nvgMoveTo( vg, CGRectGetMinX( _grid->canvas), y);
      nvgLineTo( vg, CGRectGetMaxX( _grid->canvas), y);
   }

   nvgStroke( vg);
}


void   nvgCheckerboardFillGrid( NVGcontext *vg,
                                struct MulleGrid *_grid,
                                CGColor color1,
                                CGColor color2)
{
   NSUInteger   col;
   NSUInteger   row;
   CGRect       rect;
   CGColor      color;
   CGRect       textRect;

   for( row = 0; row < _grid->rows; row++)
   {
      for( col = 0; col < _grid->columns; col++)
      {
         rect = MulleGridGetItemRect( _grid, row, col);
         rect = CGRectIntegral( rect);

         nvgBeginPath(vg);
         nvgRect(vg, rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);

         if( (row + col) % 2 == 0)
            color = color1;
         else
            color = color2;

         nvgFillColor( vg, color);
         nvgFill( vg);

         mvgSave( vg);
         {
            mvgIntersectScissor( vg, rect);

            mvgTextColor( vg, CGColorSuggestTextColor( color));
            mvgTextBackgroundColor( vg, color);

            textRect = CGRectInset( rect, 4.0, 4.0);
            mvgFontSize( vg, 10.0);
            mvgFormatInRect( vg, textRect,
                                 CGContentModeLeft|CGContentModeTop,
                                 MVGTextNoClip,
                                 "%s",
                                 CGPointUTF8String( rect.origin));
            mvgFormatInRect( vg, textRect,
                                 CGContentModeRight|CGContentModeBottom,
                                 MVGTextNoClip,
                                 "%s",
                                 CGSizeUTF8String( rect.size));
         }
         mvgRestore( vg);
      }
   }
}



@interface GridLayer : CGGraphicsLayer< CAAnimationDelegate>
{
   struct MulleGrid   _grid;

   // do it all wrong and store animation info in the _grid layer, not
   // in CAAnimation subclass
   CAAnimation        *_animation;

   NSUInteger         _animationState;  // 0: insets 1: spacing 2: rows/cols 3: itemSize
}
@end


@implementation GridLayer

static void   animateGrid( CAAnimation *animation,
                           id object,
                           struct CAAnimationFrameInfo *info)
{
   GridLayer         *self = MULLE_OBJC_CLASS_CAST_NON_NIL( GridLayer, object);
   CGFloat           value;
   MulleEdgeInsets   insets;
   CGSize            spacing;
   CGRect            canvas;
   CGSize            itemSize;
   NSUInteger        rowsColumns;

   insets      = MulleEdgeInsetsMake( 5, 5, 5, 5);
   itemSize    = CGSizeZero;
   rowsColumns = 0;
   spacing     = CGSizeMake( 10, 10);

   canvas = [object canvas];
   switch( self->_animationState)
   {
   case 0 :
      value  = 5 * info->t;
      insets = MulleEdgeInsetsMake( value, value, value, value);
      break;

   case 1 :
      value   = 10 * info->t;
      spacing = CGSizeMake( value, value);
      break;

   case 2 :
      rowsColumns = (uintptr_t) round( 7 * info->t) + 1;
      break;

   case 3 :
      itemSize = CGSizeMake( canvas.size.width / 3 * 2 * info->t,
                             canvas.size.height / 3 * 2 * info->t);
      break;

   default : abort();
   }

   if( CGSizeEqualToSize( itemSize, CGSizeZero) && ! rowsColumns)
   {
      rowsColumns = 8;
   }

   MulleGridInit( &self->_grid, [self canvas],
                                insets,
                                itemSize,
                                rowsColumns,
                                rowsColumns,
                                spacing);
}


- (void) animationWillStart:(CAAnimation *) animation
{
}


- (void) animationDidStart:(CAAnimation *) animation
{
}


- (void) animationDidStop:(CAAnimation *) animation
                 finished:(BOOL) flag
{
   [animation reset];

   self->_animationState = (self->_animationState + 1) % 4;
}


- (void) dealloc
{
   [_animation release];
   [super dealloc];
}


- (BOOL) drawInContext:(CGContext *) context
{
   struct NVGcontext           *vg;
   CGRect                      canvas;
   CGRect                      textRect;
   CGFloat                     fontSize;
   uintptr_t                   state;
   struct CAAnimationOptions   options;

   [context useFontWithNameUTF8String:"mono"];
   vg = CGContextGetNVGContext( context);

   //
   // if we do this in -init, we have to write -mulleGainAccess to transfer
   // thread ownership
   //
   if( ! _animation)
   {
      options = CAAnimationOptionsMake( mulle_relativetimerange_make( 0, 2.0),
                                        0.0,
                                        2.0,
                                        CAAnimationReverses);

      _animation = [[CAAnimation animationWithAnimationOptions:&options] retain];
      [_animation setAnimationFunction:animateGrid];
      [_animation setDelegate:self];
   }

   [_animation animateObject:self
                absoluteTime:mulle_absolutetime_now()
               animationRate:1/HZ];

   if( MulleGridConstruct( &_grid, 0, &state))
   {
      // MulleGridRoundToIntegral( &_grid, state);
      nvgCheckerboardFillGrid( vg, &_grid, CGColorMakeWhiteSmoke(), CGColorMakeMidnightBlack());

      nvgStrokeWidth( vg, 2.0);
      nvgStrokeColor( vg, CGColorMakeRed());
      nvgStrokeGrid( vg, &_grid);
   }


   mvgTextColor( vg, CGColorMakeOrange());
   mvgTextBackgroundColor( vg, CGColorMakeWhite());

   fontSize = 24;
   mvgFontSize( vg, fontSize);

   canvas   = [self canvas];
   textRect = CGRectMake( canvas.origin.x + 4.0, canvas.origin.y + 4.0, canvas.size.width, fontSize);

   mvgFormatInRect( vg, textRect, 0, 0, "Canvas   : %s", CGRectUTF8String( _grid.canvas));
   textRect.origin.y += textRect.size.height;
   mvgFormatInRect( vg, textRect, 0, 0, "Insets   : %s", MulleEdgeInsetsUTF8String( _grid.insets));
   textRect.origin.y += textRect.size.height;
   mvgFormatInRect( vg, textRect, 0, 0, "ItemSize : %s", CGSizeUTF8String( _grid.itemSize));
   textRect.origin.y += textRect.size.height;
   mvgFormatInRect( vg, textRect, 0, 0, "Rows     : %u", _grid.rows);
   textRect.origin.y += textRect.size.height;
   mvgFormatInRect( vg, textRect, 0, 0, "Columns  : %u", _grid.columns);
   textRect.origin.y += textRect.size.height;
   mvgFormatInRect( vg, textRect, 0, 0, "Spacing  : %s", CGSizeUTF8String( _grid.spacing));
   textRect.origin.y += textRect.size.height;

   return( YES);
}

@end


int   main()
{
   UIWindow        *window;
   UIApplication   *app;
   UIView          *view;
   GridLayer       *layer;

   /*
    * window and app
    */
   app = [UIApplication sharedInstance];
   @autoreleasepool
   {
      window = [UIWindow windowWithFrame:CGRectMake( 0.0, 0.0, 800.0, 600.0)];
      [window setBackgroundColor:CGColorMakeGray()];
      [window setFrameRateHz:HZ];

      // in a different twist to most of the other demos, we don't use a
      // GridView to create the layer (we are not interested in events)
      // but just clobber the existing boring layer of the windows
      // contentView

      view  = [window contentView];
      assert( view);
      layer = [GridLayer layerWithFrame:[view contentRect]];
      [view removeAllLayers];
      [view addLayer:layer];
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

