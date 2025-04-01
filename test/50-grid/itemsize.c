#include <mulle-layout/mulle-layout.h>

#include <stdio.h>


#define ALL

static CGSize    screen_sizes[] =
{
#ifdef ALL
   { .width = INFINITY, .height = INFINITY },
#endif
   { .width = 1024,     .height = INFINITY },
#ifdef ALL
   { .width = INFINITY, .height = 768 },
#endif
   { .width = 1024,     .height = 768 },
#ifdef ALL
   { .width = 128,      .height = 64 },
   { .width = 0,        .height = 0 }
#endif
};

static uintptr_t    number_of_items[] =
{
#ifdef ALL
   0,
#endif
   1,
#ifdef ALL
   4,
#endif
   16,
#ifdef ALL
   64,
   128,
#endif
   1024,
#ifdef ALL
   2048,
#endif
};


static CGSize    item_sizes[] =
{
#ifdef ALL
   { .width = 0,    .height = 0 },
   { .width = 0,    .height = 44 },
#endif
   { .width = 0,    .height = 128 },
#ifdef ALL
   { .width = 0,    .height = 256 },
   { .width = 44,   .height = 0 },
   { .width = 44,   .height = 44 },
#endif
   { .width = 128,  .height = 0 },
   { .width = 128,  .height = 128 },
#ifdef ALL
   { .width = 256,  .height = 0 },
   { .width = 256,  .height = 256 }
#endif
};


static CGSize    space_sizes[] =
{
   { .width = 0,  .height = 0 },
#ifdef ALL
   { .width = 10, .height = 10 }
#endif
};


static unsigned int    column_values[] =
{
   0,
   4
};


static unsigned int    row_values[] =
{
   0,
   4
};


#define array_count( array)  (sizeof( array) / sizeof( array[ 0]))

int   main()
{
   int                i;
   int                j;
   int                k;
   int                l;
   int                m;
   int                n;
   CGSize             size;
   unsigned int       rows;
   unsigned int       cols;
   struct MulleGrid   grid;
   CGRect             canvas;
   int                rval;
   uintptr_t          state;
   
   for( i = 0; i < array_count( screen_sizes); i++)
   {
      mulle_printf( "screen_size: %g,%g area:%g\n", screen_sizes[ i].width, screen_sizes[ i].height,
                                                 screen_sizes[ i].width * screen_sizes[ i].height);
      for( j = 0; j < array_count( item_sizes); j++)
      {
         mulle_printf( "   item_size: %g,%g\n", item_sizes[ j].width, item_sizes[ j].height);
         for( k = 0; k < array_count( space_sizes); k++)
         {
            for( l = 0; l < array_count( number_of_items); l++)
            {
               for( m = 0; m < array_count( column_values); m++)
               {
                  for( n = 0; n < array_count( row_values); n++)
                  {
                     canvas = CGRectMake( 0, 0, screen_sizes[ i].width, screen_sizes[ i].height);
                     MulleGridInit( &grid,
                                    canvas,
                                    MulleEdgeInsetsZero,
                                    item_sizes[ j],
                                    row_values[ m],
                                    column_values[ n],
                                    space_sizes[ k]);
                     rval = MulleGridConstruct( &grid, number_of_items[ l], &state);
                     if( MulleGridIsValid( &grid))
                     {
                        assert( rval == 1);
                        mulle_printf( "      spacing_size: %g,%g ", space_sizes[ k].width, space_sizes[ k].height);
                        mulle_printf( "row/column: %u,%u ", row_values[ m], column_values[ n]);
                        mulle_printf( "#%td ==> itemSize:%g,%g rows/columns:%u/%u area:%g",
                                                number_of_items[ l],
                                                grid.itemSize.width, grid.itemSize.height,
                                                grid.rows,
                                                grid.columns,
                                                number_of_items[ l] * grid.itemSize.width * grid.itemSize.height);
                        if( ! CGRectEqualToRect( grid.canvas, canvas))
                        {
                           mulle_printf( " canvas:{%g,%g,%g,%g}",
                                 grid.canvas.origin.x, grid.canvas.origin.y,
                                 grid.canvas.size.width, grid.canvas.size.height);
                        }
                        mulle_printf( "\n");
                     }
                     else
                        assert( rval == 0);
                  }       
               }
            }
         }
      }
   }

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

