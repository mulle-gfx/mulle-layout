#include <mulle-layout/mulle-layout.h>

#include <stdio.h>


static void   test( CGRect layoutRect,
                    struct MulleEdgeInsets layoutInsets,
                    CGSize size,
                    struct MulleEdgeInsets insets)
{
   struct MulleLayout  layout;
   CGRect              part;
   CGSize              remainder;

   fprintf( stdout, "[%g %g / %g %g] ",
                        layoutInsets.left,
                        layoutInsets.right,
                        insets.left,
                        insets.right);

   MulleLayoutInit( &layout, layoutRect, layoutInsets);

   part = MulleLayoutAddToRow( &layout,
                               size,
                               insets);

   // expect 4, 0
   fprintf( stdout, "%g %g -> %g %g",
                     size.width,
                     size.height,
                     part.origin.x, part.origin.y);

   remainder = MulleLayoutGetRemainingRowContentSize( &layout);
   fprintf( stdout, " (%g %g)\n",
                     remainder.width,
                     remainder.height);
   MulleLayoutDone( &layout);
}




int   main()
{
   test( CGRectMake( 0, 0, 100, 20), MulleEdgeInsetsZero,
         CGSizeMake( 10, 10), MulleEdgeInsetsZero);
   test( CGRectMake( 0, 0, 100, 20), MulleEdgeInsetsZero,
         CGSizeMake( 10, 10), MulleEdgeInsetsMakeLR( 0, 4));
   test( CGRectMake( 0, 0, 100, 20), MulleEdgeInsetsZero,
         CGSizeMake( 10, 10), MulleEdgeInsetsMakeLR( 4, 0));
   test( CGRectMake( 0, 0, 100, 20), MulleEdgeInsetsZero,
         CGSizeMake( 10, 10), MulleEdgeInsetsMakeLR( 4, 4));

   printf("\n");

   test( CGRectMake( 0, 0, 100, 20), MulleEdgeInsetsMakeLR( 0, 4),
         CGSizeMake( 10, 10), MulleEdgeInsetsZero);
   test( CGRectMake( 0, 0, 100, 20), MulleEdgeInsetsMakeLR( 0, 4),
         CGSizeMake( 10, 10), MulleEdgeInsetsMakeLR( 0, 4));
   test( CGRectMake( 0, 0, 100, 20), MulleEdgeInsetsMakeLR( 0, 4),
         CGSizeMake( 10, 10), MulleEdgeInsetsMakeLR( 4, 0));
   test( CGRectMake( 0, 0, 100, 20), MulleEdgeInsetsMakeLR( 0, 4),
         CGSizeMake( 10, 10), MulleEdgeInsetsMakeLR( 4, 4));

   printf("\n");

   test( CGRectMake( 0, 0, 100, 20), MulleEdgeInsetsMakeLR( 4, 0),
         CGSizeMake( 10, 10), MulleEdgeInsetsZero);
   test( CGRectMake( 0, 0, 100, 20), MulleEdgeInsetsMakeLR( 4, 0),
         CGSizeMake( 10, 10), MulleEdgeInsetsMakeLR( 0, 4));
   test( CGRectMake( 0, 0, 100, 20), MulleEdgeInsetsMakeLR( 4, 0),
         CGSizeMake( 10, 10), MulleEdgeInsetsMakeLR( 4, 0));
   test( CGRectMake( 0, 0, 100, 20), MulleEdgeInsetsMakeLR( 4, 0),
         CGSizeMake( 10, 10), MulleEdgeInsetsMakeLR( 4, 4));

   printf("\n");

   test( CGRectMake( 0, 0, 100, 20), MulleEdgeInsetsMakeLR( 4, 4),
         CGSizeMake( 10, 10), MulleEdgeInsetsZero);
   test( CGRectMake( 0, 0, 100, 20), MulleEdgeInsetsMakeLR( 4, 4),
         CGSizeMake( 10, 10), MulleEdgeInsetsMakeLR( 0, 4));
   test( CGRectMake( 0, 0, 100, 20), MulleEdgeInsetsMakeLR( 4, 4),
         CGSizeMake( 10, 10), MulleEdgeInsetsMakeLR( 4, 0));
   test( CGRectMake( 0, 0, 100, 20), MulleEdgeInsetsMakeLR( 4, 4),
         CGSizeMake( 10, 10), MulleEdgeInsetsMakeLR( 4, 4));

   return( 0);
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

