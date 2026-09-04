#include <mulle-layout/mulle-layout.h>

#include <stdio.h>


//
// Edge cases for vertical margin merging:
//   - Zero-height items
//   - Item that exactly fills the frame vertically
//   - Item top margin equal to frame top inset (boundary case)
//   - Single item per row with large bottom margin
//   - Many rows to verify margin doesn't accumulate
//

static int      n_items;
static CGRect   items[30];


static int   add_item( struct MulleLayout *layout, CGSize size, MulleEdgeInsets margins)
{
   CGRect   r;

   r = MulleLayoutAddToRow( layout, size, margins);
   if( CGRectIsNull( r))
   {
      MulleLayoutNewRow( layout);
      r = MulleLayoutAddToRow( layout, size, margins);
   }
   if( CGRectIsNull( r))
      return( 0);
   items[ n_items++] = r;
   return( 1);
}


static void   check_no_overlap( void)
{
   int   i, j;

   for( i = 0; i < n_items; i++)
      for( j = i + 1; j < n_items; j++)
         if( CGRectIntersectsRect( items[i], items[j]))
            mulle_printf( "OVERLAP: %d and %d\n", i, j);
}


static void   print_items( char *label)
{
   int   i;

   mulle_printf( "%s\n", label);
   for( i = 0; i < n_items; i++)
      mulle_printf( "  #%d: %g %g %g %g\n", i,
               items[i].origin.x, items[i].origin.y,
               items[i].size.width, items[i].size.height);
   check_no_overlap();
}


//
// Test: zero-height item doesn't affect row progression
// (zero-width items return CGRectNull and are skipped, but zero-height 
// items with non-zero width are placed)
//
static void   test_zero_height( void)
{
   struct MulleLayout  layout;

   n_items = 0;
   MulleLayoutInit( &layout, CGRectMake( 0, 0, 200, 100),
                    MulleEdgeInsetsMake( 5, 0, 5, 0));

   // zero-height item with bottom margin
   add_item( &layout, CGSizeMake( 50, 0), MulleEdgeInsetsMake( 0, 0, 8, 0));
   // normal item same row
   add_item( &layout, CGSizeMake( 50, 20), MulleEdgeInsetsMake( 0, 0, 3, 0));
   // new row
   add_item( &layout, CGSizeMake( 180, 20), MulleEdgeInsetsMake( 0, 0, 0, 0));

   print_items( "zero_height");
}


//
// Test: item exactly fills frame height (no room for a second row)
//
static void   test_exact_fill( void)
{
   struct MulleLayout  layout;
   CGRect              r;

   n_items = 0;
   // frame 100x50, insets T=5 B=5 -> usable height = 40
   MulleLayoutInit( &layout, CGRectMake( 0, 0, 100, 50),
                    MulleEdgeInsetsMake( 5, 0, 5, 0));

   // item height=40, top margin=0 -> y=5, maxY=45, frame bottom at 45. Fits exactly.
   add_item( &layout, CGSizeMake( 40, 40), MulleEdgeInsetsMake( 0, 0, 0, 0));

   // second item same row also fits width-wise
   add_item( &layout, CGSizeMake( 40, 40), MulleEdgeInsetsMake( 0, 0, 0, 0));

   // third item won't fit in width -> new row, but height is exhausted
   r = MulleLayoutAddToRow( &layout, CGSizeMake( 40, 40),
                            MulleEdgeInsetsMake( 0, 0, 0, 0));
   // should not fit
   MulleLayoutNewRow( &layout);
   r = MulleLayoutAddToRow( &layout, CGSizeMake( 40, 40),
                            MulleEdgeInsetsMake( 0, 0, 0, 0));

   mulle_printf( "exact_fill\n");
   mulle_printf( "  #0: %g %g %g %g\n", items[0].origin.x, items[0].origin.y,
                  items[0].size.width, items[0].size.height);
   mulle_printf( "  #1: %g %g %g %g\n", items[1].origin.x, items[1].origin.y,
                  items[1].size.width, items[1].size.height);
   mulle_printf( "  row2_fits: %d\n", ! CGRectIsNull( r));
   check_no_overlap();
}


//
// Test: equal margins (item top == frame top) — boundary of "who wins"
//
static void   test_equal_margins( void)
{
   struct MulleLayout  layout;

   n_items = 0;
   // frame T=7, item top=7 — should use 7 (max of equals)
   MulleLayoutInit( &layout, CGRectMake( 0, 0, 200, 200),
                    MulleEdgeInsetsMake( 7, 0, 7, 0));

   add_item( &layout, CGSizeMake( 180, 20), MulleEdgeInsetsMake( 7, 0, 7, 0));
   add_item( &layout, CGSizeMake( 180, 20), MulleEdgeInsetsMake( 7, 0, 7, 0));
   add_item( &layout, CGSizeMake( 180, 20), MulleEdgeInsetsMake( 7, 0, 7, 0));

   print_items( "equal_margins (frame=7, item=7)");

   // row 1 at y=7
   // row rect after item0: origin.y=0, height includes up to 27
   // NewRow: rect.y=0+27=27, margins.top=max(7,7)=7
   // row 2: y = 27 + max(7,7) = 27 + 7 = 34
   assert( items[0].origin.y == 7.0f);
   assert( items[1].origin.y == 34.0f);
   assert( items[2].origin.y == 61.0f);
}


//
// Test: single item per row with large bottom margin, verify gaps are
// consistent (no accumulation)
//
static void   test_no_accumulation( void)
{
   struct MulleLayout  layout;
   int                 i;

   n_items = 0;
   // frame T=2, items bottom=12
   MulleLayoutInit( &layout, CGRectMake( 0, 0, 100, 500),
                    MulleEdgeInsetsMake( 2, 0, 2, 0));

   for( i = 0; i < 5; i++)
      add_item( &layout, CGSizeMake( 90, 15), MulleEdgeInsetsMake( 0, 0, 12, 0));

   print_items( "no_accumulation (frame_top=2, item_bottom=12)");

   // row 1: y=2
   // row rect: (0,0,90,17) [union of (0,0,0,0) and (0,2,90,15)]
   // NewRow: rect.y=0+17=17, margins.top=max(2,12)=12
   // row 2: y=17+12=29
   // gap between rows should be constant: 12 (bottom wins over frame top 2)
   // row heights: rect always grows to 17 (from y=0 to y=2+15=17)
   // Actually let me just verify the constant spacing:
   for( i = 1; i < 5; i++)
   {
      CGFloat  gap = items[i].origin.y - (items[i-1].origin.y + items[i-1].size.height);
      assert( gap == 12.0f);
   }
}


//
// Test: item top margin varies per row
//
static void   test_varying_top( void)
{
   struct MulleLayout  layout;

   n_items = 0;
   MulleLayoutInit( &layout, CGRectMake( 0, 0, 100, 500),
                    MulleEdgeInsetsMake( 3, 0, 3, 0));

   // row 1: top=3 (matches frame)
   add_item( &layout, CGSizeMake( 90, 20), MulleEdgeInsetsMake( 3, 0, 5, 0));
   // row 2: top=10 (beats frame and prev bottom)
   add_item( &layout, CGSizeMake( 90, 20), MulleEdgeInsetsMake( 10, 0, 2, 0));
   // row 3: top=1 (frame top 3 beats it, but prev bottom 2 also < 3, so frame wins)
   add_item( &layout, CGSizeMake( 90, 20), MulleEdgeInsetsMake( 1, 0, 0, 0));
   // row 4: top=0 (frame top 3 wins, prev bottom 0 < 3)
   add_item( &layout, CGSizeMake( 90, 20), MulleEdgeInsetsMake( 0, 0, 0, 0));

   print_items( "varying_top");
}


int   main( void)
{
   test_zero_height();
   test_exact_fill();
   test_equal_margins();
   test_no_accumulation();
   test_varying_top();

   mulle_printf( "all passed\n");
   return( 0);
}
