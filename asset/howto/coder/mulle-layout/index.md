# mulle-layout — rectangle geometry & manual layout

<!-- Keywords: layout, grid, partition, placement, contentmode, insets, corners -->

## When to use

Use `mulle-layout` when you need programmatic, explicit rectangle positioning — row/column layout, uniform grids, partitioning, content scaling/placement, and relative positioning. No constraint solver, no autolayout.

## Understand first

```
mulle-sde api cat mulle-layout
mulle-sde api apropos CGRectPartition
mulle-sde api apropos CGContentMode
mulle-sde api apropos MulleLayout
mulle-sde howto show --keyword layout
```

## Local references

| Path | What |
|------|------|
| `src/MulleLayout.h` | Row-based layout state machine |
| `src/MulleGrid.h` | Uniform grid layout |
| `src/MulleLayoutContent.h` | Content mode enum & placement |
| `src/MulleLayoutPlacement.h` | Relative placement API |
| `src/MulleLayoutPartition.h` | Partition & subdivide |
| `src/MulleEdgeInsets.h` | Inset/padding constructors |
| `src/MulleCornerInsets.h` | Corner radii (nanovg-compatible) |
| `demo/src/main-layout.m` | All API families demoed |
| `demo/src/main-grid.m` | Animated grid demo |
| `demo/src/main-remaining.m` | Row layout with `RemainingRowContentRect` |
| `test/20-place/place-size.c` | ContentMode placement matrix |
| `test/20-place/scaleaspectfillcenter.c` | ScaleAspectFill + CenterPoint |

## Dominant API families

| Family | Header | Purpose |
|--------|--------|---------|
| `MulleLayout` | `MulleLayout.h` | Row-based sequential layout |
| `MulleGrid` | `MulleGrid.h` | Dynamic uniform grid |
| Content mode | `MulleLayoutContent.h` | Place/scale a rect inside another |
| Placement | `MulleLayoutPlacement.h` | Position rect relative to another |
| Partition | `MulleLayoutPartition.h` | Divide rect into equal pieces |
| Edge insets | `MulleEdgeInsets.h` | Multi-constructor inset values |
| Corner insets | `MulleCornerInsets.h` | Per-corner radii |

## Primary workflow

1. Choose the API family that matches your layout problem (row, grid, partition, or relative).
2. Stack-allocate the layout struct on the stack — no heap allocation needed.
3. Always call the `Init` function, then use the produce/advance functions, then `Done`.
4. Check `CGRectIsNull()` return values — they indicate placement failure.
5. Margins between adjacent items **merge** (max wins), they do not **accumulate**.
6. Use `_MulleLayoutInit` / `_`-prefixed variants only in hot paths where NULL is impossible.

## Verify / report

```
mulle-sde craft
cd test
mulle-sde run all
```
