# mulle-layout Library Documentation for AI
<!-- Keywords: layout, geometry, rectangles, placement, grid, insets, contentmode -->

## 1. Introduction & Purpose

**mulle-layout** provides functions for manual layouting of rectangles in 2D space. It offers a comprehensive set of tools for positioning, sizing, and arranging rectangular UI elements in a precise, programmatic way without automatic layout engines. The library is designed for graphics and UI applications where fine-grained control over element placement is required.

Key features include:
- **Row/column layout system**: Place rectangles left-to-right in rows (or top-to-bottom in columns) with margin handling, both in normal and reverse (right-to-left) direction
- **Grid layout**: Arrange elements in uniform grids with automatic calculations, plus hit-testing of points to grid cells
- **Content modes**: Position and scale rectangles within other rectangles (similar to UIView contentMode)
- **Partitioning**: Divide rectangles into smaller sections horizontally, vertically, or in 2D grids
- **Placement**: Position rectangles adjacent to other rectangles with various alignment options
- **Edge and corner insets**: Flexible margin/padding system with multiple constructor patterns

This library is a component of the **mulle-gfx** framework and depends on **mulle-geometry** for basic geometric types (CGRect, CGPoint, CGSize, etc.).

## 2. Key Concepts & Design Philosophy

### Core Design Principles

1. **Explicit Over Implicit**: All layout decisions are explicit and deterministic. No hidden auto-layout algorithms.

2. **C89 Compatibility**: Pure C library with inline functions for performance-critical operations.

3. **Margin Merging**: Unlike CSS box model, margins between adjacent elements merge (the larger margin wins), not accumulate. E.g., `---| |----` results in 20pt spacing, not 30pt.
   ```
      10   20
   ```

4. **State-Based Layout**: Layout operations maintain state in stack-allocated structs (e.g., `struct MulleLayout`) rather than requiring heap allocation.

5. **NULL-Safe API**: Most functions have `_prefixed` versions (unsafe, no NULL checks) and non-prefixed versions (safe, with NULL checks).

6. **Geometry Foundation**: Built on top of CoreGraphics-style geometry types (CGRect, CGPoint, CGSize, CGFloat) from mulle-geometry.

7. **Flexible Insets**: Multiple constructor patterns for MulleEdgeInsets (TLBR, clockwise, natural, HV, etc.) to match different mental models.

### Architectural Patterns

- **Immediate Mode**: Layout calculations are performed immediately and results are returned as CGRect values.
- **No Constraints**: Unlike Auto Layout, there's no constraint solver—layouts are calculated directly.
- **Overflow Handling**: Functions explicitly handle or report overflow conditions (e.g., when content doesn't fit).
- **Reverse Layout**: Support for right-to-left layout flows through reverse initialization (`MulleLayoutInitReverse`).
- **Vertical Layout**: Support for top-to-bottom, column-filling layout via `MulleLayoutInitVertical`, which transposes the frame and reuses the horizontal row logic.
- **Grid Hit-Testing**: `MulleGrid` supports locating the grid cell at an arbitrary point, with row/column resolution down to individual items.

## 3. Core API & Data Structures

### 3.1. `MulleEdgeInsets.h`

#### `struct MulleEdgeInsets`
- **Purpose:** Represents insets (margins, padding) on all four sides of a rectangle.
- **Key Fields:**
  - `CGFloat left`: Left inset
  - `CGFloat top`: Top inset
  - `CGFloat right`: Right inset
  - `CGFloat bottom`: Bottom inset

#### Lifecycle Functions
- `MulleEdgeInsetsMake(top, left, bottom, right)`: Standard constructor (TLBR order, Apple convention)
- `MulleEdgeInsetsMakeTLBR(top, left, bottom, right)`: Explicit TLBR alias
- `MulleEdgeInsetsMakeCounterClockwise(top, left, bottom, right)`: Another TLBR alias
- `MulleEdgeInsetsMakeClockwise(top, right, bottom, left)`: CSS-style ordering
- `MulleEdgeInsetsMakeNatural(left, top, right, bottom)`: Natural reading order
- `MulleEdgeInsetsMakeHV(left, right, top, bottom)`: Groups horizontal/vertical pairs
- `MulleEdgeInsetsMakeAtariLE(top, bottom, left, right)`: Atari joystick order (little-endian)
- `MulleEdgeInsetsMakeAtariBE(right, left, bottom, top)`: Atari joystick order (big-endian)
- `MulleEdgeInsetsMakeLR(left, right)`: Left-right only (top/bottom are 0)
- `MulleEdgeInsetsMakeTB(top, bottom)`: Top-bottom only (left/right are 0)
- `MulleEdgeInsetsMakeU(topLeft, leftTop, bottomRight, rightBottom, index)`: Alternating constructor (index 0 or 1 swaps pairs)
- `MulleEdgeInsetsData(top, left, bottom, right)`: Static initializer macro

#### Core Operations
- `MulleEdgeInsetsInsetRect(insets, rect)`: Shrinks a rect by applying insets inward
- `MulleEdgeInsetsExtrudeRect(insets, rect)`: Expands a rect by applying insets outward
- `MulleEdgeInsetsInsetPoint(insets, origin)`: Offsets a point by left/top insets
- `MulleEdgeInsetsExtrudePoint(insets, origin)`: Offsets a point by negative left/top insets
- `MulleEdgeInsetsInsetSize(insets, size)`: Reduces size by sum of horizontal/vertical insets
- `MulleEdgeInsetsExtrudeSize(insets, size)`: Increases size by sum of horizontal/vertical insets
- `MulleEdgeInsetsUnion(a, b)`: Returns maximum insets (element-wise max)
- `MulleEdgeInsetsIntersect(a, b)`: Returns minimum insets (element-wise min)
- `CGRectInsetUsingEdgeInsets(rect, insets)`: Alias for InsetRect with swapped argument order
- `CGRectExtrudeUsingEdgeInsets(rect, insets)`: Alias for ExtrudeRect with swapped argument order

#### Inspection Functions
- `MulleEdgeInsetsEqualToEdgeInsets(insets1, insets2)`: Equality comparison
- `CGRectEdgeInsetsInRect(rect, outer)`: Calculate insets of rect within outer rect

#### Constants
- `MulleEdgeInsetsZero`: All four insets set to 0.0

### 3.2. `MulleCornerInsets.h`

#### `struct MulleCornerInsets`
- **Purpose:** Represents corner radii for rounded rectangles (compatible with nanovg).
- **Key Fields:**
  - `CGFloat topLeft`: Top-left corner radius
  - `CGFloat topRight`: Top-right corner radius
  - `CGFloat bottomRight`: Bottom-right corner radius
  - `CGFloat bottomLeft`: Bottom-left corner radius

#### Lifecycle Functions
- `MulleCornerInsetsMake(topLeft, topRight, bottomRight, bottomLeft)`: Standard constructor (nanovg order)
- `MulleCornerInsetsMakeTLRBLR(topLeft, topRight, bottomLeft, bottomRight)`: Alternate constructor (TL-TR-BL-BR order)
- `MulleCornerInsetsMakeWithCornerRadius(r)`: Uniform corner radius for all corners
- `MulleCornerInsetsData(tl, tr, br, bl)`: Static initializer macro

#### Constants
- `MulleCornerInsetsZero`: All corners with 0.0 radius

### 3.3. `MulleLayoutContent.h`

#### `enum CGContentMode`
- **Purpose:** Defines how a rectangle is positioned and scaled within another rectangle.
- **Values:**
  - **Positioning (non-scaling):**
    - `CGContentModeTop`, `CGContentModeTopRight`, `CGContentModeRight`, `CGContentModeBottomRight`, `CGContentModeBottom`, `CGContentModeBottomLeft`, `CGContentModeLeft`, `CGContentModeTopLeft`
    - `CGContentModeCenterPoint`: Center in both dimensions
  - **Scaling:**
    - `CGContentModeDefault`: 0x0, default mode (interpreted as scale-to-fill)
    - `CGContentModeScaleToFill`: Stretch to fill, both dimensions. Can be combined with `CGContentModeTop`, `CGContentModeLeft`, etc. for anchoring
    - `CGContentModeScaleToFillHorizontal`: Stretch horizontally only (mixable with vertical position bits)
    - `CGContentModeScaleToFillVertical`: Stretch vertically only (mixable with horizontal position bits)
    - `CGContentModeScaleAspectFit`: Scale to fit inside, maintain aspect ratio
    - `CGContentModeScaleAspectFill`: Scale to fill, maintain aspect ratio (may crop)
    - `CGContentModeScaleAspect`: Alias for `CGContentModeScaleAspectFit`
    - `CGContentModeCenter`: Backwards-compatibility alias for `CGContentModeCenterPoint`

#### Core Operations
- `CGSizePlaceInRect(size, rect, contentMode)`: Position and scale a size within a rect using contentMode
- `CGRectPlaceInRect(r1, r2, contentMode)`: Position rect r1 within rect r2
- `CGRectPositionInRect(r1, r2, contentMode)`: Position r1 within r2 (lower 4 bits only)
- `CGSizeAspectFitToSize(size, containerSize)`: Scale size to fit in container, preserving aspect
- `CGSizeAspectFillToSize(size, containerSize)`: Scale size to fill container, preserving aspect
- `CGSizePlaceInRectWithAspectFit(size, rect)`: Convenience for aspect fit placement
- `CGSizePlaceInRectWithAspectFill(size, rect)`: Convenience for aspect fill placement

#### Utility Functions
- `CGSizeGetAspectFitFactor(dstSize, srcSize)`: Get scale factor for aspect fit
- `CGSizeGetAspectFillFactor(dstSize, srcSize)`: Get scale factor for aspect fill
- `CGSizePlaceInRectWithAspectCorrection(size, rect, fill)`: Core aspect placement (0=fit, 1=fill)
- `CGSizeGetCenterInRect(size, rect)`: Get origin point to center size in rect
- `CGRectCenterInRect(rect, bounds)`: Center a rect within bounds
- `CGSizeCenterInRect(size, bounds)`: Center a size within bounds, returns CGRect
- `CGRectCenterAtPoint(rect, point)`: Center rect at specific point
- `CGRectGetLargestSquareSize(rect)`: Get largest square that fits in rect
- `CGRectMirrorInRectH(rect, container)`: Mirror rect horizontally within container
- `CGPointMirrorInRectH(origin, container)`: Mirror point horizontally within container
- `CGRectMirrorInRectV(rect, container)`: Mirror rect vertically within container
- `CGPointMirrorInRectV(origin, container)`: Mirror point vertically within container
- `CGAffineTransformMakeWithContentMode(contentMode, sourceRect, sourceRotation, destinationRect, destinationRotation)`: Create an affine transform that maps a source rect/rotation to a destination rect/rotation using the given content mode

#### Validation
- `CGContentModeIsValid(mode)`: Check if contentMode is valid
- `CGContentModeIsScaling(mode)`: Check if mode includes scaling
- `CGContentModeAssert(mode)`: Assert that contentMode is valid

### 3.4. `MulleLayoutPlacement.h`

#### `typedef MulleLayoutPlacement`
- **Purpose:** Encodes both horizontal and vertical placement of a rectangle relative to another.

#### Enums
- **`MulleLayoutVerticalPlacement`:**
  - `MulleLayoutPlacementKeepY`: No vertical change
  - `MulleLayoutPlacementUp`: Align bottom of rect with top of other
  - `MulleLayoutPlacementDown`: Align top of rect with bottom of other
  - `MulleLayoutPlacementMiddle`: Align vertical centers
  - `MulleLayoutPlacementTopAligned`: Align tops
  - `MulleLayoutPlacementBottomAligned`: Align bottoms

- **`MulleLayoutHorizontalPlacement`:**
  - `MulleLayoutPlacementKeepX`: No horizontal change
  - `MulleLayoutPlacementLeft`: Align right edge of rect with left edge of other
  - `MulleLayoutPlacementRight`: Align left edge of rect with right edge of other
  - `MulleLayoutPlacementCenter`: Align horizontal centers
  - `MulleLayoutPlacementLeftAligned`: Align left edges
  - `MulleLayoutPlacementRightAligned`: Align right edges

#### Core Operations
- `MulleLayoutPlacementMake(h, v)`: Create placement from horizontal and vertical components
- `MulleLayoutPlacementData(h, v)`: Static initializer macro for placement constants
- `MulleLayoutPlacementGetHorizontalPlacement(value)`: Extract horizontal component
- `MulleLayoutPlacementGetVerticalPlacement(value)`: Extract vertical component
- `CGRectPlaceNextToRect(rect, other, placement)`: Position rect adjacent to other using placement; returns the new origin as a `CGPoint`
- `CGRectPlaceNextToRectInSpace(rect, other, space, preferences, n_preferences)`: Find best placement within space from preference list; returns the new origin as a `CGPoint`. Passing `NULL` preferences uses a default scheme; pass non-NULL with `n_preferences` = 0 to disable the default scheme
- `CGRectFindBestPlacementNextToRectInSpace(rect, other, space, preferences, n_preferences)`: Return index of best placement (same preference NULL/0 semantics)
- `CGSizePlaceAdjacentToRectInSpace(menuSize, buttonFrame, space, preferences, n_preferences)`: Place menu-like rect adjacent to button, returns resulting `CGRect`

#### Simple Placement Utilities
- `CGRectPlaceBeforeRect(rect, spacing, width)`: Place rect before (left of) another
- `CGRectPlaceAfterRect(rect, spacing, width)`: Place rect after (right of) another
- `CGRectPlaceAboveRect(rect, spacing, height)`: Place rect above another
- `CGRectPlaceBelowRect(rect, spacing, height)`: Place rect below another

#### Alignment Utilities
- `CGRectAlignLeft(rect, y)`, `CGRectAlignCenter(rect, x)`, `CGRectAlignRight(rect, y)`
- `CGRectAlignTop(rect, y)`, `CGRectAlignMiddle(rect, y)`, `CGRectAlignBottom(rect, y)`

### 3.5. `MulleLayoutPartition.h`

#### Partitioning Functions
- **Purpose:** Divide a rectangle into multiple equal-sized rectangles with spacing.

#### Core Operations
- `CGRectPartitionHorizontally(frame, space, n, rects)`: Divide horizontally into n columns
- `CGRectPartitionVertically(frame, space, n, rects)`: Divide vertically into n rows
- `CGRectPartitionIntoRowsAndColumns(frame, space, rowSize, columnSize, rects)`: 2D partition
- `CGRectPartition2x2(frame, space, rects[2][2])`: Convenience for 2x2 grid
- `CGRectPartition3x3(frame, space, rects[3][3])`: Convenience for 3x3 grid

#### Aliases
- `CGRectPartitionH()`, `CGRectPartitionV()`, `CGRectPartitionVH()`: Short aliases
- `CGRectPartitionIntoColumns()`, `CGRectPartitionIntoRows()`: Descriptive aliases
- `CGRectPartitionVerticallyAndHorizontally()`: Descriptive alias for PartitionIntoRowsAndColumns

#### Subdivision Operations
- `CGRectSubdivide(rect, other, output[4])`: Subdivide rect by removing overlapping area of other, returns up to 4 rects
- `CGRectSubdivideIfOverlap(rect, other, output[4])`: Returns -1 if no overlap, 0 if completely overlapped

#### CGRectDivide
- `CGRectDivide(rect, slice, remainder, amount, edge)`: Split rect into two parts from specified edge
  - `edge`: `CGRectMinXEdge`, `CGRectMinYEdge`, `CGRectMaxXEdge`, `CGRectMaxYEdge`

#### 2D Array Access
- `CGRect2DArrayGetRowColumn(rects, row, column, columnSize)`: Access element in 2D rect array
- `CGRect2DArrayGetRowColumnAddress(rects, row, column, columnSize)`: Get pointer to element

### 3.6. `MulleLayout.h`

#### `struct MulleLayout`
- **Purpose:** State management for row- or column-based layout (similar to flexbox row/column direction).
- **Key Fields:**
  - `CGRect originalRect`: Initial layout bounds
  - `MulleEdgeInsets originalMargins`: Initial margins
  - `CGRect rect`: Current row bounding rect (grows as items are added)
  - `MulleEdgeInsets margins`: Current row margins
  - `int reverse`: Flag for right-to-left layout
  - `int vertical`: Flag for top-to-bottom (column) layout

#### Lifecycle Functions
- `MulleLayoutInit(p, rect, margins)`: Initialize for left-to-right layout
- `MulleLayoutInitReverse(p, rect, margins)`: Initialize for right-to-left layout
- `MulleLayoutInitVertical(p, rect, margins)`: Initialize for vertical layout. Items fill top-to-bottom, wrapping to a new column when the height is exhausted. Internally the frame is transposed so the horizontal row logic operates on the vertical axis.
- `MulleLayoutDone(p)`: Cleanup (currently no-op, reserved for future use)

#### Core Operations
- `MulleLayoutAddToRow(p, size, margins)`: Add rect of given size to current row with margins
  - Returns `CGRectNull` if it doesn't fit (time for new row)
  - Returns positioned CGRect if successful
- `MulleLayoutAddToRowWithOverflow(p, size, margins)`: Add rect to current row; if it doesn't fit, automatically start a new row and retry. Returns the layouted rect, or `CGRectNull` if it still doesn't fit in the fresh row
- `MulleLayoutAddRowWithHeight(p, h, margins)`: Add full-width row of specific height
- `MulleLayoutNewRow(p)`: Finish current row and start a new one below it

#### Inspection Functions
- `MulleLayoutGetContentRect(p)`: Get total available content area (original rect minus margins)
- `MulleLayoutGetRowContentRect(p)`: Get bounding rect of items placed in current row
- `MulleLayoutGetRowContentSize(p)`: Get size of current row content
- `MulleLayoutGetRowContentOrigin(p)`: Get origin of current row content
- `MulleLayoutGetRemainingRowContentRect(p)`: Get available space remaining in current row
- `MulleLayoutGetRemainingRowContentSize(p)`: Get size of remaining space
- `MulleLayoutGetRemainingRowContentOrigin(p)`: Get origin of remaining space
- `MulleLayoutGetUsedRect(p)`: Get bounding box of all placed items across all rows. Width spans the full content width (frame minus left/right insets); height covers from content top to the bottom of the current row's items. Returns `CGRectZero` if nothing has been placed. Use for scroll-view content size or sizeToFit calculations.
- `MulleLayoutGetUsedSize(p)`: Get size of the used rect (convenience wrapper over `MulleLayoutGetUsedRect`)
- `MulleLayoutIsEmptyRow(p)`: Check if current row has no items

### 3.7. `MulleGrid.h`

#### `struct MulleGrid`
- **Purpose:** Dynamic grid layout system for arranging items in uniform rows and columns.
- **Key Fields:**
  - `CGRect canvas`: Total area for the grid
  - `MulleEdgeInsets insets`: Margins inside canvas
  - `uintptr_t rows`: Number of rows (can be calculated)
  - `uintptr_t columns`: Number of columns (can be calculated)
  - `CGSize itemSize`: Size of each grid item (can be calculated)
  - `CGSize spacing`: Spacing between items
  - `uintptr_t _internal`: Internal state for construction

#### Lifecycle Functions
- `MulleGridInit(p, canvas, insets, itemSize, rows, columns, spacing)`: Initialize with known values (use 0 for unknowns)
- `MulleGridConstruct(p, numberOfItems, state)`: Calculate missing values (returns 1 if successful)
  - Can calculate missing dimension of itemSize
  - Can calculate rows or columns if one is known
  - Sets `canvas` to `CGRectNull` if construction fails
- `MulleGridDone(p)`: Cleanup (currently no-op)

#### Validation
- `MulleGridIsValid(p)`: Check if grid is valid (canvas not null)
- `MulleGridVerify(p)`: Verify grid constraints

#### Core Operations
- `MulleGridGetItemRect(p, row, column)`: Get rect for item at specific row/column (returns `CGRectNull` if row/column out of range)
- `MulleGridGetItemRectAtIndex(p, i)`: Get rect for item at linear index (returns `CGRectNull` if `p` is NULL or columns is 0)
- `MulleGridGetColumnX(p, column)`: Get x-coordinate of column. Column `0` returns canvas x, column `columns` returns canvas max-x, out-of-range returns `NAN`
- `MulleGridGetRowY(p, row)`: Get y-coordinate of row. Mirrors `MulleGridGetColumnX`
- `MulleGridGetCapacity(p)`: Get total number of item slots (`rows * columns`). Returns 0 for an unconstructed or invalid grid

#### Hit-Testing
##### `struct MulleGridPosition`
- **Purpose:** Grid position (row, column) of an item, used for hit-testing a point against the grid.
- **Key Fields:**
  - `intptr_t row`: Row index, or -1 when the point doesn't hit any item
  - `intptr_t column`: Column index, or -1 when the point doesn't hit any item

- `MulleGridPositionMake(r, c)`: Designated-initializer macro to build a position
- `MulleGridPositionInvalid`: Constant `MulleGridPositionMake(-1, -1)`
- `MulleGridPositionIsValid(pos)`: Check that both row and column are >= 0
- `MulleGridGetPositionAtPoint(p, point, numberOfItems)`: Return grid position of the item at `point`. Returns `MulleGridPositionInvalid` if the point is in spacing, insets, out of bounds, or (if `numberOfItems` > 0) beyond the item count
- `MulleGridGetIndexAtPoint(p, point, numberOfItems)`: Linear index version of `MulleGridGetPositionAtPoint`. Returns -1 if the point doesn't hit any item

## 4. Performance Characteristics

### General
- **O(1)**: Most operations are constant time calculations
- **No allocations**: All operations use stack-allocated structs, no heap allocations
- **Inline functions**: Many small functions are inlined for optimal performance
- **Cache-friendly**: Small struct sizes (4-8 CGFloats typically)

### Specific Operations
- **MulleLayout row operations**: O(1) - simple arithmetic and margin comparisons
- **Grid construction**: O(1) - algebraic calculation to fill missing values
- **Grid item lookup**: O(1) - index arithmetic
- **Partitioning**: O(n) - where n is number of partitions (must fill output array)
- **Subdivision**: O(1) - generates up to 4 output rects
- **Content mode placement**: O(1) - geometric calculations

### Thread Safety
- **Not thread-safe**: Layout structs are designed for single-threaded use
- **Reentrant**: Pure calculation functions (those without state) are reentrant
- **No shared state**: Each layout struct maintains independent state

### Trade-offs
- **Memory vs. flexibility**: Grid stores all parameters, enabling later queries without recalculation
- **Precision vs. speed**: Uses CGFloat (typically double), prioritizing precision over raw speed
- **Manual vs. automatic**: Requires explicit layout steps, giving control but requiring more code

## 5. AI Usage Recommendations & Patterns

### Best Practices

1. **Always use lifecycle functions**: Initialize with `Init` functions, cleanup with `Done` functions (even if currently no-op).

2. **Check for NULL rects**: Many functions return `CGRectNull` to indicate failure. Use `CGRectIsNull()` to check.

3. **Stack allocation**: All layout structs are designed for stack allocation:
   ```c
   struct MulleLayout  layout;  // Good
   ```

4. **Prefer safe wrappers**: Use non-prefixed functions (with NULL checks) unless in performance-critical code:
   ```c
   MulleLayoutInit( &layout, ...);        // Safe, checks for NULL
   _MulleLayoutInit( &layout, ...);       // Unsafe, no checks
   ```

5. **Margin merging awareness**: Remember that adjacent margins merge (max wins), not accumulate.

6. **Use appropriate inset constructors**: Choose the constructor that matches your mental model:
   - `MulleEdgeInsetsMakeNatural()` for left-to-right thinking
   - `MulleEdgeInsetsMakeClockwise()` for CSS-style ordering
   - `MulleEdgeInsetsMakeLR()` or `MulleEdgeInsetsMakeTB()` for single-axis margins

7. **Grid construction pattern**:
   ```c
   struct MulleGrid  grid;
   MulleGridInit( &grid, canvas, insets, itemSize, rows, 0, spacing);  // columns=0 to calculate
   if( ! MulleGridConstruct( &grid, numberOfItems, NULL))
   {
      // Grid construction failed
   }
   ```

### Common Pitfalls

1. **Don't access `_internal` or private fields**: Fields starting with `_` are internal.

2. **Don't modify originalRect/originalMargins**: These are read-only after initialization.

3. **Don't forget to call NewRow()**: After adding items, call `MulleLayoutNewRow()` before starting next row.

4. **Don't mix coordinate systems**: Ensure all rects use the same coordinate system (mulle-layout doesn't track origins).

5. **Read the docs on overflow**: `MulleLayoutAddToRow` returns `CGRectNull` when an item doesn't fit — that's the signal to call `MulleLayoutNewRow()`. `MulleLayoutAddToRowWithOverflow` wraps this by starting a row for you, but it doesn't make items overlap the frame.

6. **Validate content modes**: Use `CGContentModeIsValid()` before using user-provided modes.

### Idiomatic Usage

**MulleLayout pattern:**
```c
struct MulleLayout  layout;
CGRect              itemRect;

MulleLayoutInit( &layout, containerRect, MulleEdgeInsetsMakeLR( 10, 10));

itemRect = MulleLayoutAddToRow( &layout, CGSizeMake( 100, 40), MulleEdgeInsetsZero);
if( CGRectIsNull( itemRect))
{
   MulleLayoutNewRow( &layout);
   itemRect = MulleLayoutAddToRow( &layout, CGSizeMake( 100, 40), MulleEdgeInsetsZero);
}

MulleLayoutDone( &layout);
```

**MulleGrid pattern:**
```c
struct MulleGrid  grid;
uintptr_t         state;

MulleGridInit( &grid, 
               CGRectMake( 0, 0, 800, 600),
               MulleEdgeInsetsMake( 10, 10, 10, 10),
               CGSizeMake( 0, 50),  // width calculated, height fixed
               0,                    // rows calculated
               4,                    // 4 columns
               CGSizeMake( 5, 5));

if( MulleGridConstruct( &grid, 12, &state))
{
   for( uintptr_t i = 0; i < 12; i++)
   {
      CGRect  itemRect = MulleGridGetItemRectAtIndex( &grid, i);
      // Use itemRect
   }
}

MulleGridDone( &grid);
```

**Content mode pattern:**
```c
CGRect   imageRect;
CGSize   imageSize = CGSizeMake( 200, 150);
CGRect   container = CGRectMake( 0, 0, 300, 300);

imageRect = CGSizePlaceInRect( imageSize, container, CGContentModeScaleAspectFit);
// imageRect is now centered and scaled to fit
```

**Partitioning pattern:**
```c
CGRect   columns[ 3];

CGRectPartitionHorizontally( containerRect, 10.0, 3, columns);
// columns[0], columns[1], columns[2] now contain equal-width rects with 10pt spacing
```

## 6. Integration Examples

### Example 1: Creating a Three-Column Layout with Headers

```c
#include <mulle-layout/mulle-layout.h>

void   layout_three_columns( CGRect bounds)
{
   struct MulleLayout  layout;
   CGRect              headerRect;
   CGRect              columns[ 3];
   CGRect              column;
   int                 i;

   // Initialize layout with 20pt margins
   MulleLayoutInit( &layout, bounds, MulleEdgeInsetsMake( 20, 20, 20, 20));

   // Add header row (full width, 60pt high)
   headerRect = MulleLayoutAddRowWithHeight( &layout, 60.0, MulleEdgeInsetsZero);

   // Start new row for columns
   MulleLayoutNewRow( &layout);

   // Get remaining space and partition into 3 columns
   column = MulleLayoutGetRemainingRowContentRect( &layout);
   CGRectPartitionHorizontally( column, 10.0, 3, columns);

   // Use the columns
   for( i = 0; i < 3; i++)
   {
      // Draw or populate columns[i]
   }

   MulleLayoutDone( &layout);
}
```

### Example 2: Creating a Photo Grid with Dynamic Item Count

```c
#include <mulle-layout/mulle-layout.h>

void   layout_photo_grid( CGRect canvas, uintptr_t photoCount)
{
   struct MulleGrid  grid;
   uintptr_t         state;
   uintptr_t         i;
   CGRect            photoRect;

   // Initialize grid: 4 columns, calculate rows and item width
   // Items are 100pt tall
   MulleGridInit( &grid,
                  canvas,
                  MulleEdgeInsetsMake( 10, 10, 10, 10),
                  CGSizeMake( 0, 100),  // width auto, height 100
                  0,                     // rows auto
                  4,                     // 4 columns
                  CGSizeMake( 8, 8));    // 8pt spacing

   if( MulleGridConstruct( &grid, photoCount, &state))
   {
      for( i = 0; i < photoCount; i++)
      {
         photoRect = MulleGridGetItemRectAtIndex( &grid, i);
         // Draw photo at photoRect
      }
   }

   MulleGridDone( &grid);
}
```

### Example 3: Placing an Image with Content Mode

```c
#include <mulle-layout/mulle-layout.h>

CGRect   place_image_in_view( CGSize imageSize, CGRect viewBounds, int maintainAspect)
{
   CGRect         imageRect;
   CGContentMode  mode;

   if( maintainAspect)
      mode = CGContentModeScaleAspectFit;
   else
      mode = CGContentModeScaleToFill;

   imageRect = CGSizePlaceInRect( imageSize, viewBounds, mode);

   return( imageRect);
}
```

### Example 4: Building a Toolbar with Variable-Width Items

```c
#include <mulle-layout/mulle-layout.h>

void   layout_toolbar( CGRect toolbarRect)
{
   struct MulleLayout  layout;
   CGRect              buttonRect;
   MulleEdgeInsets     buttonMargins;

   buttonMargins = MulleEdgeInsetsMakeLR( 4, 4);

   MulleLayoutInit( &layout, toolbarRect, MulleEdgeInsetsMakeLR( 8, 8));

   // Add buttons of various widths
   buttonRect = MulleLayoutAddToRow( &layout, CGSizeMake( 80, 32), buttonMargins);
   // Draw "Save" button at buttonRect

   buttonRect = MulleLayoutAddToRow( &layout, CGSizeMake( 60, 32), buttonMargins);
   // Draw "Open" button at buttonRect

   buttonRect = MulleLayoutAddToRow( &layout, CGSizeMake( 100, 32), buttonMargins);
   // Draw "Settings" button at buttonRect

   MulleLayoutDone( &layout);
}
```

### Example 5: Positioning a Popup Menu Adjacent to a Button

```c
#include <mulle-layout/mulle-layout.h>

CGRect   position_popup_menu( CGSize menuSize, CGRect buttonFrame, CGRect windowBounds)
{
   CGRect              menuRect;
   MulleLayoutPlacement  preferences[ 4];

   // Prefer: below-left, below-right, above-left, above-right
   preferences[ 0] = MulleLayoutPlacementMake( MulleLayoutPlacementLeftAligned,
                                               MulleLayoutPlacementDown);
   preferences[ 1] = MulleLayoutPlacementMake( MulleLayoutPlacementRightAligned,
                                               MulleLayoutPlacementDown);
   preferences[ 2] = MulleLayoutPlacementMake( MulleLayoutPlacementLeftAligned,
                                               MulleLayoutPlacementUp);
   preferences[ 3] = MulleLayoutPlacementMake( MulleLayoutPlacementRightAligned,
                                               MulleLayoutPlacementUp);

   menuRect = CGSizePlaceAdjacentToRectInSpace( menuSize,
                                                buttonFrame,
                                                windowBounds,
                                                preferences,
                                                4);

   return( menuRect);
}
```

### Example 6: Subdividing Window Space Around a Center Panel

```c
#include <mulle-layout/mulle-layout.h>

void   layout_with_center_panel( CGRect windowRect, CGRect centerPanel)
{
   CGRect        surrounding[ 4];
   unsigned int  n;
   unsigned int  i;

   // Subdivide window around center panel
   n = CGRectSubdivide( windowRect, centerPanel, surrounding);

   // surrounding[] now contains up to 4 rects (top, left, right, bottom areas)
   for( i = 0; i < n; i++)
   {
      // Use surrounding[i] for sidebars, toolbars, etc.
   }
}
```

### Example 7: Creating a Card Layout with Rounded Corners and Insets

```c
#include <mulle-layout/mulle-layout.h>

void   draw_card( CGRect bounds)
{
   MulleEdgeInsets    padding;
   MulleCornerInsets  corners;
   CGRect             contentRect;
   CGRect             titleRect;
   CGRect             bodyRect;

   // 16pt padding on all sides
   padding     = MulleEdgeInsetsMakeClockwise( 16, 16, 16, 16);
   corners     = MulleCornerInsetsMakeWithCornerRadius( 8.0);

   contentRect = MulleEdgeInsetsInsetRect( padding, bounds);

   // Divide content into title (top 40pt) and body (rest)
   CGRectDivide( contentRect, &titleRect, &bodyRect, 40.0, CGRectMinYEdge);

   // Draw card background with rounded corners using corners
   // Draw title in titleRect
   // Draw body in bodyRect
}
```

### Example 8: Grid Hit-Testing with MulleGridPosition

```c
#include <mulle-layout/mulle-layout.h>

// Returns the index of the grid item at `point`, or -1 when the point
// is in spacing, insets, or out of bounds.
intptr_t   hit_test_grid( struct MulleGrid *grid, CGPoint point, uintptr_t itemCount)
{
   intptr_t   index;

   index = MulleGridGetIndexAtPoint( grid, point, itemCount);
   if( index < 0)
      return( -1);

   return( index);
}

// Alternatively use the row/column variant directly.
void   hit_test_grid_row_column( struct MulleGrid *grid, CGPoint point)
{
   struct MulleGridPosition   pos;

   pos = MulleGridGetPositionAtPoint( grid, point, 0);
   if( MulleGridPositionIsValid( pos))
   {
      // item at pos.row / pos.column was clicked
   }
}
```

### Example 9: Vertical Column Layout

```c
#include <mulle-layout/mulle-layout.h>

void   layout_vertical( CGRect bounds)
{
   struct MulleLayout  layout;
   CGRect              itemRect;
   uintptr_t           i;

   // Fill top-to-bottom, wrapping into a new column when height is exhausted
   MulleLayoutInitVertical( &layout, bounds, MulleEdgeInsetsMake( 10, 10, 10, 10));

   for( i = 0; i < 6; i++)
   {
      itemRect = MulleLayoutAddToRow( &layout, CGSizeMake( 100, 40), MulleEdgeInsetsZero);
      if( CGRectIsNull( itemRect))
      {
         MulleLayoutNewRow( &layout);
         itemRect = MulleLayoutAddToRow( &layout, CGSizeMake( 100, 40), MulleEdgeInsetsZero);
      }
   }

   MulleLayoutDone( &layout);
}
```

## 7. Dependencies

**Direct Dependencies:**
- **mulle-geometry**: Provides fundamental geometric types (CGRect, CGPoint, CGSize, CGFloat, CGAffineTransform, CADirection, CARotation)
- **m** (math.h): Standard C math library (used for NAN, floating-point operations)

**Transitive Dependencies** (via mulle-geometry):
- mulle-geometry itself may depend on other mulle libraries like mulle-c11

**Position in Hierarchy:**
mulle-layout is a mid-level graphics library, sitting above mulle-geometry and below UI frameworks that use it for layout calculations.
