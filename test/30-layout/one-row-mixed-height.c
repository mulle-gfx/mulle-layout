#include <mulle-layout/mulle-layout.h>

#include <stdio.h>


static void   test( CGRect layoutRect,
                    struct MulleEdgeInsets layoutInsets,
                    CGSize size1,
                    struct  MulleEdgeInsets insets1,
                    CGSize size2,
                    struct  MulleEdgeInsets insets2)
{
   struct MulleLayout  layout;
   CGRect              part;
   CGSize              remainder;

   fprintf( stdout, "[%g %g / %g %g / %g %g] ",
                        layoutInsets.left,
                        layoutInsets.right,
                        insets1.left,
                        insets1.right,
                        insets2.left,
                        insets2.right);

   MulleLayoutInit( &layout, layoutRect, layoutInsets);

   part = MulleLayoutAddToRow( &layout,
                               size1,
                               insets1);

   fprintf( stdout, "|| %g %g -> %g %g",
                     size1.width,
                     size1.height,
                     part.origin.x, part.origin.y);

   part = MulleLayoutAddToRow( &layout,
                               size2,
                               insets2);

   fprintf( stdout, " / %g %g -> %g %g ||",
                     size2.width,
                     size2.height,
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
         CGSizeMake( 10, 10), MulleEdgeInsetsZero,
         CGSizeMake( 10, 15), MulleEdgeInsetsZero);
   test( CGRectMake( 0, 0, 100, 20), MulleEdgeInsetsZero,
         CGSizeMake( 10, 10), MulleEdgeInsetsMakeLR( 0, 4),
         CGSizeMake( 10, 15), MulleEdgeInsetsMakeLR( 0, 4));
   test( CGRectMake( 0, 0, 100, 20), MulleEdgeInsetsZero,
         CGSizeMake( 10, 10), MulleEdgeInsetsMakeLR( 4, 0),
         CGSizeMake( 10, 15), MulleEdgeInsetsMakeLR( 4, 0));
   test( CGRectMake( 0, 0, 100, 20), MulleEdgeInsetsZero,
         CGSizeMake( 10, 10), MulleEdgeInsetsMakeLR( 4, 4),
         CGSizeMake( 10, 15), MulleEdgeInsetsMakeLR( 4, 4));

   printf("\n");

   test( CGRectMake( 0, 0, 100, 20), MulleEdgeInsetsMakeLR( 0, 4),
         CGSizeMake( 10, 10), MulleEdgeInsetsZero,
         CGSizeMake( 10, 15), MulleEdgeInsetsZero);
   test( CGRectMake( 0, 0, 100, 20), MulleEdgeInsetsMakeLR( 0, 4),
         CGSizeMake( 10, 10), MulleEdgeInsetsMakeLR( 0, 4),
         CGSizeMake( 10, 15), MulleEdgeInsetsMakeLR( 0, 4));
   test( CGRectMake( 0, 0, 100, 20), MulleEdgeInsetsMakeLR( 0, 4),
         CGSizeMake( 10, 10), MulleEdgeInsetsMakeLR( 4, 0),
         CGSizeMake( 10, 15), MulleEdgeInsetsMakeLR( 4, 0));
   test( CGRectMake( 0, 0, 100, 20), MulleEdgeInsetsMakeLR( 0, 4),
         CGSizeMake( 10, 10), MulleEdgeInsetsMakeLR( 4, 4),
         CGSizeMake( 10, 15), MulleEdgeInsetsMakeLR( 4, 4));

   printf("\n");

   test( CGRectMake( 0, 0, 100, 20), MulleEdgeInsetsMakeLR( 4, 0),
         CGSizeMake( 10, 10), MulleEdgeInsetsZero,
         CGSizeMake( 10, 15), MulleEdgeInsetsZero);
   test( CGRectMake( 0, 0, 100, 20), MulleEdgeInsetsMakeLR( 4, 0),
         CGSizeMake( 10, 10), MulleEdgeInsetsMakeLR( 0, 4),
         CGSizeMake( 10, 15), MulleEdgeInsetsMakeLR( 0, 4));
   test( CGRectMake( 0, 0, 100, 20), MulleEdgeInsetsMakeLR( 4, 0),
         CGSizeMake( 10, 10), MulleEdgeInsetsMakeLR( 4, 0),
         CGSizeMake( 10, 15), MulleEdgeInsetsMakeLR( 4, 0));
   test( CGRectMake( 0, 0, 100, 20), MulleEdgeInsetsMakeLR( 4, 0),
         CGSizeMake( 10, 10), MulleEdgeInsetsMakeLR( 4, 4),
         CGSizeMake( 10, 15), MulleEdgeInsetsMakeLR( 4, 4));

   printf("\n");

   test( CGRectMake( 0, 0, 100, 20), MulleEdgeInsetsMakeLR( 4, 4),
         CGSizeMake( 10, 10), MulleEdgeInsetsZero,
         CGSizeMake( 10, 15), MulleEdgeInsetsZero);
   test( CGRectMake( 0, 0, 100, 20), MulleEdgeInsetsMakeLR( 4, 4),
         CGSizeMake( 10, 10), MulleEdgeInsetsMakeLR( 0, 4),
         CGSizeMake( 10, 15), MulleEdgeInsetsMakeLR( 0, 4));
   test( CGRectMake( 0, 0, 100, 20), MulleEdgeInsetsMakeLR( 4, 4),
         CGSizeMake( 10, 10), MulleEdgeInsetsMakeLR( 4, 0),
         CGSizeMake( 10, 15), MulleEdgeInsetsMakeLR( 4, 0));
   test( CGRectMake( 0, 0, 100, 20), MulleEdgeInsetsMakeLR( 4, 4),
         CGSizeMake( 10, 10), MulleEdgeInsetsMakeLR( 4, 4),
         CGSizeMake( 10, 15), MulleEdgeInsetsMakeLR( 4, 4));

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

