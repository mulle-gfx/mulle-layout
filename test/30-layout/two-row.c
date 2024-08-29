#include <mulle-layout/mulle-layout.h>

#include <stdio.h>


static void   test( CGRect layoutRect,
                    struct MulleEdgeInsets layoutInsets,
                    CGSize size1,
                    struct  MulleEdgeInsets insets1,
                    CGSize size2,
                    struct  MulleEdgeInsets insets2,
                    CGSize size3,
                    struct  MulleEdgeInsets insets3)
{
   struct MulleLayout  layout;
   CGRect              part;
   CGSize              remainder;

   fprintf( stdout, "r=[%g %g %g %g] i=[%g %g %g %g] || ",
                        layoutRect.origin.x,
                        layoutRect.origin.y,
                        layoutRect.size.width,
                        layoutRect.size.height,
                        layoutInsets.top,
                        layoutInsets.left,
                        layoutInsets.bottom,
                        layoutInsets.right);

   MulleLayoutInit( &layout, layoutRect, layoutInsets);

   part = MulleLayoutAddToRow( &layout,
                               size1,
                               insets1);

   // expect 4, 0
   fprintf( stdout, "1: s=[%g %g] i=[%g %g %g %g] -> o=[%g %g]",
                     size1.width,
                     size1.height,
                     insets1.top,
                     insets1.left,
                     insets1.bottom,
                     insets1.right,
                     part.origin.x, part.origin.y);

   if( size2.width > 0.0)
   {
      part = MulleLayoutAddToRow( &layout,
                                  size2,
                                  insets2);
      fprintf( stdout, " 2: s=[%g %g] i=[%g %g %g %g] -> o=[%g %g]",
                        size2.width,
                        size2.height,
                        insets2.top,
                        insets2.left,
                        insets2.bottom,
                        insets2.right,
                        part.origin.x, part.origin.y);
   }

   if( size3.width > 0.0)
   {
      part = MulleLayoutAddToRow( &layout,
                                  size3,
                                  insets3);
      fprintf( stdout, " 3: s=[%g %g] i=[%g %g %g %g] -> o=[%g %g]",
                        size3.width,
                        size3.height,
                        insets3.top,
                        insets3.left,
                        insets3.bottom,
                        insets3.right,
                        part.origin.x, part.origin.y);
   }

   remainder = MulleLayoutGetRemainingRowContentSize( &layout);
   fprintf( stdout, "|| (%g %g)\n",
                     remainder.width,
                     remainder.height);
   MulleLayoutDone( &layout);
}


int   main()
{
#if 1  
   fprintf( stdout, "no margins, easy fit\n");
   test( CGRectMake( 0, 0, 20, 10), MulleEdgeInsetsZero,
         CGSizeMake( 10, 10), MulleEdgeInsetsZero,
         CGSizeMake( 10, 10), MulleEdgeInsetsZero,
         CGSizeZero, MulleEdgeInsetsZero);

   test( CGRectMake( 0, 0, 20, 10), MulleEdgeInsetsZero,
         CGSizeMake( 4, 10), MulleEdgeInsetsZero,
         CGSizeMake( 6, 10), MulleEdgeInsetsZero,
         CGSizeMake( 10, 10), MulleEdgeInsetsZero);

   fprintf( stdout, "\nno margins, misfit\n");
   test( CGRectMake( 0, 0, 20, 10), MulleEdgeInsetsZero,
         CGSizeMake( 10, 10), MulleEdgeInsetsZero,
         CGSizeMake( 11, 10), MulleEdgeInsetsZero,
         CGSizeZero, MulleEdgeInsetsZero);

   test( CGRectMake( 0, 0, 20, 10), MulleEdgeInsetsZero,
         CGSizeMake( 10, 10), MulleEdgeInsetsZero,
         CGSizeMake( 2, 10), MulleEdgeInsetsZero,
         CGSizeMake( 10, 10), MulleEdgeInsetsZero);


   fprintf( stdout, "\nlayout margins only, fit\n");
   test( CGRectMake( 0, 0, 20, 10), MulleEdgeInsetsMakeLR( 1, 3),
         CGSizeMake( 9, 10), MulleEdgeInsetsZero,
         CGSizeMake( 7, 10), MulleEdgeInsetsZero,
         CGSizeZero, MulleEdgeInsetsZero);

   test( CGRectMake( 0, 0, 20, 10), MulleEdgeInsetsMakeLR( 1, 3),
         CGSizeMake( 9, 10), MulleEdgeInsetsZero,
         CGSizeMake( 3, 10), MulleEdgeInsetsZero,
         CGSizeMake( 4, 10), MulleEdgeInsetsZero);
#endif
   fprintf( stdout, "\nboth margins, fit\n");
   test( CGRectMake( 0, 0, 20, 10), MulleEdgeInsetsMakeLR( 2, 2),
         CGSizeMake( 7, 10), MulleEdgeInsetsMakeLR( 2, 2),
         CGSizeMake( 7, 10), MulleEdgeInsetsMakeLR( 2, 2),
         CGSizeZero, MulleEdgeInsetsZero);

   test( CGRectMake( 0, 0, 20, 10), MulleEdgeInsetsMakeLR( 2, 2),
         CGSizeMake( 5, 10), MulleEdgeInsetsMakeLR( 2, 2),
         CGSizeMake( 4, 10), MulleEdgeInsetsMakeLR( 2, 2),
         CGSizeMake( 5, 10), MulleEdgeInsetsMakeLR( 2, 2));

   fprintf( stdout, "\nboth margins\n");
   // So now margins are merged from the container and the added rects.
   // Remember: margins are not added but sort of "unioned" 
   test( CGRectMake( 0, 0, 20, 10), MulleEdgeInsetsMakeLR( 1, 3),
         CGSizeMake( 9, 10), MulleEdgeInsetsMakeLR( 1, 0),
         CGSizeMake( 7, 10), MulleEdgeInsetsMakeLR( 0, 3),
         CGSizeZero, MulleEdgeInsetsZero);



   test( CGRectMake( 0, 0, 20, 10), MulleEdgeInsetsMakeLR( 1, 6),
         CGSizeMake( 4, 10), MulleEdgeInsetsMakeLR( 2, 3),
         CGSizeMake( 4, 10), MulleEdgeInsetsMakeLR( 4, 5),
         CGSizeZero, MulleEdgeInsetsZero);

   test( CGRectMake( 0, 0, 20, 10), MulleEdgeInsetsMakeLR( 1, 6),
         CGSizeMake( 4, 10), MulleEdgeInsetsMakeLR( 2, 3),
         CGSizeMake( 2, 10), MulleEdgeInsetsMakeLR( 4, 5),
         CGSizeZero, MulleEdgeInsetsZero);

   test( CGRectMake( 1, 0, 13, 10), MulleEdgeInsetsZero,
         CGSizeMake( 4, 10), MulleEdgeInsetsMakeLR( 2, 3),
         CGSizeMake( 4, 10), MulleEdgeInsetsMakeLR( 4, 5),
         CGSizeZero, MulleEdgeInsetsZero);

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

