# Quirks

<!-- Keywords: pitfalls, invariants, restrictions, surprising, donot -->

## Margins merge (max wins), do not accumulate

```
// Two adjacent items with margins 10 and 20 → 20pt spacing, not 30pt
// src/MulleLayout.h:38-41
```

This differs from CSS/Yoga box model where margins add together.

## Always call lifecycle functions

`MulleLayoutInit` / `MulleGridInit` must be called before use, and `Done` after (even though both are currently no-ops — reserved for future use).

## Do not mutate `originalRect` or `originalMargins` post-init

These fields are read-only after initialization (`src/MulleLayout.h:47-55`).

## Do not access `_internal` fields

`MulleGrid._internal` is private. Never read or write it.

## `CGRectNull` signals failure

Many functions return `CGRectNull` instead of asserting. Always check with `CGRectIsNull()`:
- `MulleLayoutAddToRow` returns `CGRectNull` when the item does not fit in the current row.
- `MulleGridConstruct` sets `canvas` to `CGRectNull` if construction fails.
- `MulleGridIsValid` checks `canvas` is not null.

## NULL-safe wrappers exist alongside fast unsafe variants

| Safe (NULL-checked) | Unsafe (_ prefixed) |
|---|---|
| `MulleLayoutInit` | `_MulleLayoutInit` |
| `MulleLayoutAddToRow` | `_MulleLayoutAddToRow` |
| `MulleGridConstruct` | `_MulleGridConstruct` |

Use `_`-prefixed only when you know the pointer is non-NULL.

## `CGContentModeCenterPoint` is a special bit pattern

It ORs `CADirectionTop | CADirectionLeft | CADirectionBottom | CADirectionRight` (0x0F). Do not combine position bits yourself — use the named enum (`src/MulleLayoutContent.h:43-46`).

## `CGContentModeScaleAspectFill` can exceed the container rect

By design. The result may extend beyond the container bounds (`test/20-place/scaleaspectfillcenter.c`). Use scissoring or clipping if you need to constrain rendering.

## Coordinate system is not tracked

All layout calculations assume a consistent coordinate system. The library does not transform or convert between coordinate spaces.

## Not thread-safe

Layout structs (`MulleLayout`, `MulleGrid`) are designed for single-threaded use. Pure calculation functions (placement, partition, content mode) are reentrant.

## `MulleCornerInsets` uses `float`, not `CGFloat`

Fields are `float` to match nanovg's `nvgRoundedRectVarying` parameters (`src/MulleCornerInsets.h:12-16`).

## `CGRectAlignRight` has a misleading parameter name

The second parameter of `CGRectAlignRight` is named `y` but actually represents `x` position (`src/MulleLayoutPlacement.h:52-56`). This is a naming quirk — use it as an x-coordinate.

## Overflow is opt-in

`MulleLayoutAddToRow` returns `CGRectNull` on overflow. Use `MulleLayoutAddToRowWithOverflow` explicitly to allow items wider than the row.
