# Patterns

<!-- Keywords: init, construct, newrow, addtorow, partition, contentmode, placement, insets -->

## Row layout (`MulleLayout`)

```
struct MulleLayout  layout;
CGRect              frame;

MulleLayoutInit( &layout, containerRect, MulleEdgeInsetsMakeLR( 10, 10));

// Add items left-to-right; CGRectNull means "start a new row"
frame = MulleLayoutAddToRow( &layout, CGSizeMake( 100, 40), MulleEdgeInsetsZero);
if( CGRectIsNull( frame))
{
   MulleLayoutNewRow( &layout);
   frame = MulleLayoutAddToRow( &layout, CGSizeMake( 100, 40), MulleEdgeInsetsZero);
}

MulleLayoutDone( &layout);
```

- Margins are **outside** the item rect (`src/MulleLayout.h:267-276`).
- `MulleLayoutAddToRow` returns `CGRectNull` when the item does not fit — that signals it is time for a new row (`src/MulleLayout.h:283-288`).
- `MulleLayoutAddToRowWithOverflow` adds even if it overflows bounds.
- `MulleLayoutAddRowWithHeight` preempts the current row and starts a full-width row at a fixed height.
- Use `MulleLayoutInitReverse` for right-to-left filling (`demo/src/main-remaining.m:52`).
- Use `MulleLayoutGetRemainingRowContentRect` to retrieve the unused space in the current row (`demo/src/main-remaining.m:56`).

## Grid layout (`MulleGrid`)

```
struct MulleGrid  grid;
uintptr_t         state;

MulleGridInit( &grid,
               CGRectMake( 0, 0, 800, 600),
               MulleEdgeInsetsMake( 10, 10, 10, 10),
               CGSizeMake( 0, 50),   // width auto-calculated
               0,                     // rows auto-calculated
               4,                     // 4 columns
               CGSizeMake( 5, 5));

if( MulleGridConstruct( &grid, 12, &state))
{
   for( uintptr_t i = 0; i < 12; i++)
   {
      CGRect item = MulleGridGetItemRectAtIndex( &grid, i);
   }
}
MulleGridDone( &grid);
```

- Pass `0` for unknown dimensions — `MulleGridConstruct` fills them in (`src/MulleGrid.h:13-25`).
- `MulleGridConstruct` sets `canvas` to `CGRectNull` on failure — always check the return value (`src/MulleGrid.h:120-134`).
- Use `MulleGridGetItemRect` (by row/col) or `MulleGridGetItemRectAtIndex` (linear index) to retrieve cell frames.
- `MulleGridGetColumnX` / `MulleGridGetRowY` give grid-line coordinates for drawing (`demo/src/main-grid.m:20-33`).

## Content mode placement (`CGContentMode`)

```
CGSize   imageSize = CGSizeMake( 200, 150);
CGRect   container = CGRectMake( 0, 0, 300, 300);
CGRect   placed;

placed = CGSizePlaceInRect( imageSize, container, CGContentModeScaleAspectFit);
```

- Lower 4 bits select **position** (top, left, centerPoint, etc.). Upper bits select **scaling**.
- `CGContentModeCenterPoint` ORs all four direction bits — it is a special value (`src/MulleLayoutContent.h:43-46`).
- `CGContentModeScaleToFill` (0x30) is the default and equals `CGContentModeDefault` (0x0).
- Validate with `CGContentModeIsValid()` before using user-provided modes.
- `CGRectPlaceInRect` preserves the source rect's x or y when the mode is `ScaleToFillHorizontal`/`ScaleToFillVertical` (`src/MulleLayoutContent.h:140-157`).

## Relative placement (`MulleLayoutPlacement`)

```
MulleLayoutPlacement   p;
CGPoint                origin;

p      = MulleLayoutPlacementMake( MulleLayoutPlacementLeftAligned,
                                   MulleLayoutPlacementDown);
origin = CGRectPlaceNextToRect( floatingRect, anchorRect, p);
```

- Horizontal and vertical components are packed into a single uintptr_t (`MulleLayoutPlacementMake`: h << 4 | v).
- `CGRectPlaceNextToRectInSpace` tries a list of preferences until one fits within a bounding space.
- Pass `NULL` for preferences to use a built-in default scheme (`src/MulleLayoutPlacement.h:270-275`).
- `CGSizePlaceAdjacentToRectInSpace` is specialized for menu-like placement next to a button.
- Simple convenience: `CGRectPlaceBeforeRect`, `CGRectPlaceAfterRect`, `CGRectPlaceAboveRect`, `CGRectPlaceBelowRect`.

## Partitioning

```
CGRect   columns[ 3];
CGRect   rows[ 4];
CGRect   grid[ 2][ 2];

// 3 equal columns with 10pt spacing
CGRectPartitionHorizontally( container, 10.0, 3, columns);

// 4 equal rows with 0 spacing
CGRectPartitionVertically( container, 0, 4, rows);

// 2x2 grid with 8pt spacing
CGRectPartition2x2( container, CGSizeMake( 8, 8), grid);

// n×m grid with spacing
CGRectPartitionIntoRowsAndColumns( container, spacing, rows, cols, rects);
```

- Aliases: `CGRectPartitionH` / `CGRectPartitionV` / `CGRectPartitionVH` (`src/MulleLayoutPartition.h:45-51,73-79`).
- `CGRectSubdivide` splits a rect around an overlapping "other" rect, returning up to 4 output rects (`src/MulleLayoutPartition.h:102-106`).
- `CGRectDivide` splits a rect into `slice` and `remainder` from a specified edge (`src/MulleLayoutPartition.h:127-158`).

## Edge insets (`MulleEdgeInsets`)

```
// TLBR — Apple convention (default)
MulleEdgeInsetsMake( 10, 10, 10, 10);

// Clockwise — CSS-style
MulleEdgeInsetsMakeClockwise( 10, 20, 10, 20);

// Natural reading order
MulleEdgeInsetsMakeNatural( 10, 10, 10, 10);

// Horizontal/vertical pairs
MulleEdgeInsetsMakeHV( 10, 10, 20, 20);

// Single axis
MulleEdgeInsetsMakeLR( 10, 10);
MulleEdgeInsetsMakeTB( 10, 10);
```

- Fields: `left, top, right, bottom` (the struct stores them in an order optimized for `InsetRect` — see `src/MulleEdgeInsets.h:16-22`).
- `MulleEdgeInsetsInsetRect` shrinks a rect; `MulleEdgeInsetsExtrudeRect` expands.
- `MulleEdgeInsetsUnion` / `MulleEdgeInsetsIntersect` compute element-wise max/min.
- `CGRectEdgeInsetsInRect` computes the insets of one rect inside another.

## Corner insets (`MulleCornerInsets`)

```
MulleCornerInsetsMake( 4, 4, 4, 4);        // TL, TR, BR, BL — nanovg order
MulleCornerInsetsMakeWithCornerRadius( 8);  // uniform
```

- Uses `float` (not CGFloat) — matches nanovg `nvgRoundedRectVarying` (`src/MulleCornerInsets.h:35-37`).
