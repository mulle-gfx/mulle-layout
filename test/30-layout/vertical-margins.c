#include <mulle-layout/mulle-layout.h>

#include <stdio.h>


//
// Test vertical margin merging across rows.
//
// The row gap is: max( frame.top_inset, previous_item.bottom_margin )
// The item's Y position within a row is: row_start + max( frame.top, item.top )
//
// We verify:
//   1. Frame top inset wins when larger than item top/bottom margins
//   2. Item bottom margin wins when larger than frame top inset
//   3. Item top margin wins when larger than frame top inset
//   4. Mixed heights in a row: the tallest item + its bottom margin
//      determines where the next row starts
//   5. Invariant: no two items overlap
//   6. Invariant: all items inside content rect
//

static int   n_items;
static CGRect items[20];


static void   add_item( struct MulleLayout *layout, CGSize size, MulleEdgeInsets margins)
{
   CGRect   r;

   r = MulleLayoutAddToRow( layout, size, margins);
   if( CGRectIsNull( r))
   {
      MulleLayoutNewRow( layout);
      r = MulleLayoutAddToRow( layout, size, margins);
   }
   assert( ! CGRectIsNull( r));
   items[ n_items++] = r;
}


static void   check_invariants( struct MulleLayout *layout)
{
   CGRect   content;
   int      i, j;

   content = MulleLayoutGetContentRect( layout);

   for( i = 0; i < n_items; i++)
   {
      // item inside content rect
      if( ! CGRectContainsRect( content, items[ i]))
      {
         fprintf( stdout, "FAIL: item %d outside content rect\n", i);
         fprintf( stdout, "  item:    %g %g %g %g\n",
                  items[i].origin.x, items[i].origin.y,
                  items[i].size.width, items[i].size.height);
         fprintf( stdout, "  content: %g %g %g %g\n",
                  content.origin.x, content.origin.y,
                  content.size.width, content.size.height);
      }

      // no overlap with any other item
      for( j = i + 1; j < n_items; j++)
      {
         if( CGRectIntersectsRect( items[ i], items[ j]))
         {
            fprintf( stdout, "FAIL: items %d and %d overlap\n", i, j);
            fprintf( stdout, "  item %d: %g %g %g %g\n", i,
                     items[i].origin.x, items[i].origin.y,
                     items[i].size.width, items[i].size.height);
            fprintf( stdout, "  item %d: %g %g %g %g\n", j,
                     items[j].origin.x, items[j].origin.y,
                     items[j].size.width, items[j].size.height);
         }
      }
   }

   // gap between vertically adjacent items >= max of their margins
   // (we can't fully verify this without knowing which items are in which
   //  row, but we can check that no vertical overlap exists — done above)
}


static void   print_items( char *label)
{
   int   i;

   fprintf( stdout, "%s\n", label);
   for( i = 0; i < n_items; i++)
      fprintf( stdout, "  #%d: %g %g %g %g\n", i,
               items[i].origin.x, items[i].origin.y,
               items[i].size.width, items[i].size.height);
}


//
// Test 1: frame top inset (5) > item top margin (2)
//         Row gap should be 5.
//
static void   test_frame_top_wins( void)
{
   struct MulleLayout  layout;

   n_items = 0;
   // frame 200x100, insets T=5 L=0 B=5 R=0
   MulleLayoutInit( &layout, CGRectMake( 0, 0, 200, 100),
                    MulleEdgeInsetsMake( 5, 0, 5, 0));

   // row 1: two items, bottom margin = 2
   add_item( &layout, CGSizeMake( 90, 20), MulleEdgeInsetsMake( 2, 0, 2, 0));
   add_item( &layout, CGSizeMake( 90, 20), MulleEdgeInsetsMake( 2, 0, 2, 0));
   // row 2: two items, top margin = 2
   add_item( &layout, CGSizeMake( 90, 20), MulleEdgeInsetsMake( 2, 0, 2, 0));
   add_item( &layout, CGSizeMake( 90, 20), MulleEdgeInsetsMake( 2, 0, 2, 0));

   print_items( "frame_top_wins (top=5, item_tb=2)");
   check_invariants( &layout);

   // row 1 at y=5, row 2 at y=5+20+5=30 (frame top wins over item bottom 2)
   assert( items[0].origin.y == 5.0f);
   assert( items[2].origin.y == 30.0f);

   MulleLayoutDone( &layout);
}


//
// Test 2: item bottom margin (10) > frame top inset (3)
//         Row gap should be 10.
//
static void   test_item_bottom_wins( void)
{
   struct MulleLayout  layout;

   n_items = 0;
   // frame 200x100, insets T=3 L=0 B=3 R=0
   MulleLayoutInit( &layout, CGRectMake( 0, 0, 200, 100),
                    MulleEdgeInsetsMake( 3, 0, 3, 0));

   // row 1: items with large bottom margin
   add_item( &layout, CGSizeMake( 90, 15), MulleEdgeInsetsMake( 0, 0, 10, 0));
   add_item( &layout, CGSizeMake( 90, 15), MulleEdgeInsetsMake( 0, 0, 10, 0));
   // row 2
   add_item( &layout, CGSizeMake( 90, 15), MulleEdgeInsetsMake( 0, 0, 10, 0));
   add_item( &layout, CGSizeMake( 90, 15), MulleEdgeInsetsMake( 0, 0, 10, 0));

   print_items( "item_bottom_wins (top=3, item_bottom=10)");
   check_invariants( &layout);

   // row 1 at y=3, row 2 at y=3+15+10=28 (item bottom 10 wins over frame top 3)
   assert( items[0].origin.y == 3.0f);
   assert( items[2].origin.y == 28.0f);

   MulleLayoutDone( &layout);
}


//
// Test 3: item top margin (8) > frame top inset (4)
//         First row starts at max(frame.top, item.top) = 8.
//
static void   test_item_top_wins( void)
{
   struct MulleLayout  layout;

   n_items = 0;
   // frame 200x100, insets T=4 L=0 B=4 R=0
   MulleLayoutInit( &layout, CGRectMake( 0, 0, 200, 100),
                    MulleEdgeInsetsMake( 4, 0, 4, 0));

   // items with large top margin
   add_item( &layout, CGSizeMake( 90, 20), MulleEdgeInsetsMake( 8, 0, 3, 0));
   add_item( &layout, CGSizeMake( 90, 20), MulleEdgeInsetsMake( 8, 0, 3, 0));
   // new row: item top=8, previous bottom=3 -> max(4, 3)=4? No.
   // NewRow sets margins.top = max(originalMargins.top, prev_margins.bottom)
   //                         = max(4, 3) = 4
   // Then AddToRow: margins.top = max(item.top=8, running_top=4) = 8
   add_item( &layout, CGSizeMake( 90, 20), MulleEdgeInsetsMake( 8, 0, 3, 0));
   add_item( &layout, CGSizeMake( 90, 20), MulleEdgeInsetsMake( 8, 0, 3, 0));

   print_items( "item_top_wins (frame_top=4, item_top=8, item_bottom=3)");
   check_invariants( &layout);

   // row 1 at y=8 (item top wins over frame top 4)
   assert( items[0].origin.y == 8.0f);
   // row 2: row1_y=0 + row_height (max extent from row origin)
   // row rect grows to union of items: origin.y=0, maxY = 8+20 = 28
   // NewRow: rect.origin.y += rect.size.height -> y = 28
   // margins.top = max(4, 3) = 4, then AddToRow: max(8, 4) = 8
   // so row 2 item y = 28 + 8 = 36
   assert( items[2].origin.y == 36.0f);

   MulleLayoutDone( &layout);
}


//
// Test 4: mixed heights in a row — tallest item's bottom margin determines gap
//
static void   test_mixed_heights( void)
{
   struct MulleLayout  layout;

   n_items = 0;
   // frame 200x200, insets T=2 L=0 B=2 R=0
   MulleLayoutInit( &layout, CGRectMake( 0, 0, 200, 200),
                    MulleEdgeInsetsMake( 2, 0, 2, 0));

   // row 1: short item with large bottom margin, tall item with small bottom
   add_item( &layout, CGSizeMake( 90, 10), MulleEdgeInsetsMake( 0, 0, 20, 0));
   add_item( &layout, CGSizeMake( 90, 30), MulleEdgeInsetsMake( 0, 0, 5, 0));
   // row 1 rect: origin.y=0, items at y=2
   //   item0: maxY=12, bottom=20 -> envelope maxY+bottom = 32
   //   item1: maxY=32, bottom=5  -> envelope maxY+bottom = 37
   // The bottom margin tracked is whichever produces the largest maxY+bottom:
   //   item0: 12 + 20 = 32 vs item1: 32 + 5 = 37 -> item1 wins, bottom=5
   // NewRow: margins.top = max(frame.top=2, prev_bottom=5) = 5
   add_item( &layout, CGSizeMake( 90, 10), MulleEdgeInsetsMake( 0, 0, 0, 0));
   add_item( &layout, CGSizeMake( 90, 10), MulleEdgeInsetsMake( 0, 0, 0, 0));

   print_items( "mixed_heights (short+bigbottom, tall+smallbottom)");
   check_invariants( &layout);

   // both row1 items at y=2
   assert( items[0].origin.y == 2.0f);
   assert( items[1].origin.y == 2.0f);
   // row rect: union of (0,2,90,10) and (90,2,90,30) = (0,2,180,30)
   // so rect.size.height=30, NewRow: rect.origin.y = 0+30=30 (wait, not 0)
   // Actually rect starts at origin (0,0) with size (0,0)
   // After item0: rect = union((0,0,0,0), (0,2,90,10)) = (0,0,90,12)
   //    No wait - CGRectUnion with non-Apple semantics preserves the zero rect
   //    origin. So union((0,0,0,0), (0,2,90,10)):
   //    minX=min(0,0)=0, minY=min(0,2)=0, maxX=max(0,90)=90, maxY=max(0,12)=12
   //    -> (0,0,90,12)
   // After item1: union((0,0,90,12), (90,2,90,30)) = (0,0,180,32)
   // NewRow: rect.origin.y += rect.size.height -> 0+32=32
   // margins.top = max(2, 5) = 5
   // row 2 item y = 32 + 5 = 37
   assert( items[2].origin.y == 37.0f);

   MulleLayoutDone( &layout);
}


//
// Test 5: asymmetric — different bottom margins on items in the same row
//
static void   test_asymmetric_bottom( void)
{
   struct MulleLayout  layout;

   n_items = 0;
   // frame 200x200, insets T=3 L=0 B=3 R=0
   MulleLayoutInit( &layout, CGRectMake( 0, 0, 200, 200),
                    MulleEdgeInsetsMake( 3, 0, 3, 0));

   // row 1: item with bottom=15, item with bottom=7 (same height)
   add_item( &layout, CGSizeMake( 90, 20), MulleEdgeInsetsMake( 0, 0, 15, 0));
   add_item( &layout, CGSizeMake( 90, 20), MulleEdgeInsetsMake( 0, 0, 7, 0));
   // item0: maxY = 3+20 = 23, bottom=15 -> 23+15=38
   // item1: maxY = 3+20 = 23, bottom=7  -> 23+7=30
   // item0 produces larger maxY+bottom, so p->margins.bottom = 15
   // rect = union of items = (0,0,180,23).. actually:
   // rect starts (0,0,0,0).
   // item0 placed at (0, 3): union((0,0,0,0),(0,3,90,20)) = (0,0,90,23)
   //   margins.bottom = 15 (since 3+20+15=38 > 0+0=0+existing)
   // item1 placed at (90, 3): union((0,0,90,23),(90,3,90,20)) = (0,0,180,23)
   //   23+7=30 vs 23+15=38 -> 15 stays
   // NewRow: rect.origin.y = 0+23=23, margins.top=max(3,15)=15
   add_item( &layout, CGSizeMake( 90, 20), MulleEdgeInsetsMake( 0, 0, 0, 0));

   print_items( "asymmetric_bottom (item0_bot=15, item1_bot=7)");
   check_invariants( &layout);

   assert( items[0].origin.y == 3.0f);
   assert( items[1].origin.y == 3.0f);
   // row 2: y = 23 + 15 = 38
   assert( items[2].origin.y == 38.0f);

   MulleLayoutDone( &layout);
}


//
// Test 6: three rows with escalating margins
//
static void   test_three_rows_escalating( void)
{
   struct MulleLayout  layout;

   n_items = 0;
   // frame 300x300, insets T=2 L=0 B=2 R=0
   MulleLayoutInit( &layout, CGRectMake( 0, 0, 300, 300),
                    MulleEdgeInsetsMake( 2, 0, 2, 0));

   // row 1: bottom=5
   add_item( &layout, CGSizeMake( 280, 20), MulleEdgeInsetsMake( 0, 0, 5, 0));
   // row 2: top=10, bottom=20
   add_item( &layout, CGSizeMake( 280, 20), MulleEdgeInsetsMake( 10, 0, 20, 0));
   // row 3: top=3, bottom=0
   add_item( &layout, CGSizeMake( 280, 20), MulleEdgeInsetsMake( 3, 0, 0, 0));

   print_items( "three_rows_escalating");
   check_invariants( &layout);

   // row 1: y = max(2, 0) = 2
   assert( items[0].origin.y == 2.0f);
   // row 1 rect: union((0,0,0,0),(0,2,280,20)) = (0,0,280,22)
   //   margins.bottom = 5
   // NewRow: rect.origin.y = 0+22=22, margins.top=max(2,5)=5
   // row 2: y = 22 + max(10, 5) = 22 + 10 = 32
   assert( items[1].origin.y == 32.0f);
   // row 2 rect: union((22,0,0,0) wait... after NewRow rect=(22,22,0,0)?
   // Actually NewRow: p->rect.origin.y += p->rect.size.height
   //   (0 + 22 = 22), rect.size = (0,0)
   // item1: margins.top = max(10, 5) = 10, y = 22 + 10 = 32
   //   rect = union((0,22,0,0),(0,32,280,20)) = (0,22,280,30)
   //   wait, rect.origin.x=0 after NewRow. Let me re-read.
   //   NewRow sets rect.origin.x = originalRect.origin.x = 0
   //   So rect = (0, 22, 0, 0). Then union with (0, 32, 280, 20):
   //   minX=0, minY=min(22,32)=22, maxX=max(0,280)=280, maxY=max(22,52)=52
   //   -> (0, 22, 280, 30). height=30.
   //   margins.bottom = 20
   // NewRow: rect.origin.y = 22+30=52, margins.top=max(2,20)=20
   // row 3: y = 52 + max(3, 20) = 52 + 20 = 72
   assert( items[2].origin.y == 72.0f);

   MulleLayoutDone( &layout);
}


int   main( void)
{
   test_frame_top_wins();
   test_item_bottom_wins();
   test_item_top_wins();
   test_mixed_heights();
   test_asymmetric_bottom();
   test_three_rows_escalating();

   fprintf( stdout, "all passed\n");
   return( 0);
}
