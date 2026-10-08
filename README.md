# mulle-layout

#### 📏 Rectangle geometry and manual layouting

# mulle-layout Semantic Model

## Overview

`mulle-layout` is an allocation-free, immediate-mode rectangle layout library.
Layout state lives in a single stack struct (`struct MulleLayout` or
`struct MulleGrid`). There is no node tree, no constraint solver, and no
heap allocation.


## MulleLayout — Row-based layout

### The frame and its insets

```
         originalRect
+------------------------+
| left     top     right |
|    +------------+      |
|    | contentRect|      |
|    +------------+      |
|        bottom          |
+------------------------+
```

`MulleLayoutInit` takes a bounding `rect` and `MulleEdgeInsets` (top, left,
bottom, right). The insets define the padding between the frame and the
content area.

**The frame's top inset doubles as inter-row spacing.** When a new row is
started, the gap above it is `max(frame.top, previous_row_bottom_margin)`.
There is no separate row-spacing parameter; if you want flush rows with frame
padding, set item bottom margins to 0 and frame top inset to your desired
padding.


### Adding items to a row

`MulleLayoutAddToRow(p, size, margins)` places a rectangle of `size` into
the current row, separated from the previous item (or from the frame edge)
by its margins.

Returns the placed `CGRect`, or `CGRectNull` if the item does not fit.


### Margin merging

Horizontal margins between adjacent items **merge**: the gap is
`max(left_item.right, right_item.left)`, not the sum. This mirrors CSS
margin collapsing.

Vertical margins merge the same way: the row gap is
`max(row_above.bottom, frame.top)`.

**Example** — two items with left=10, right=10 margins in a row:
```
gap = max(10, 10) = 10   (not 20)
```


### Fit check asymmetry

When checking whether an item fits in the row:

- The **left margin** of the item participates in the fit check (it
  determines where the item starts).
- The **right margin** of the item is **not** checked against the frame
  boundary. It only contributes to the gap before the *next* item.
- The **bottom margin** of the item is **not** checked against the frame's
  bottom inset either.

This is intentional: the right margin affects spacing to the next neighbor,
not whether this item fits. However, it means an item can be placed such
that its right margin would extend past the frame — the next item simply
won't fit.


### Reverse layout (right-to-left)

`MulleLayoutInitReverse` swaps left/right margins internally and mirrors
every returned rect horizontally within the frame. The caller's code is
identical to LTR — only the output coordinates change.


### Content rect

`MulleLayoutGetContentRect` returns the usable area inside the frame insets.
In reverse mode it is mirrored to match where items actually land.


### Row lifecycle

```c
MulleLayoutInit(&layout, frame, insets);

for each item:
    r = MulleLayoutAddToRow(&layout, size, margins);
    if (CGRectIsNull(r))
    {
        MulleLayoutNewRow(&layout);
        r = MulleLayoutAddToRow(&layout, size, margins);
    }
```


## MulleGrid — Uniform grid layout

`MulleGrid` lays out N equally-sized items in a rows×columns grid with
uniform spacing.

### Construction modes

| `itemSize`         | `rows` | `columns` | Behavior                          |
|--------------------|--------|-----------|-----------------------------------|
| given (both > 0)   | 0      | 0         | compute rows & cols from canvas   |
| given (width > 0)  | 0      | 0         | compute rows from height & canvas |
| given (height > 0) | 0      | 0         | compute cols from width & canvas  |
| zero               | given  | given     | compute itemSize from canvas      |
| zero               | 0      | given     | compute rows & itemSize           |
| zero               | given  | 0         | compute cols & itemSize           |
| zero               | 0      | 0         | auto: exhaustive search           |

In auto mode (both zero), the algorithm evaluates all feasible column counts
and picks the one that maximizes `0.5 * utilization + 0.5 * squareness`,
where utilization is the fraction of canvas area used and squareness is how
close each item's aspect ratio is to 1:1.


### Accessing grid cells

```c
MulleGridGetItemRect(&grid, index)     // rect for item at linear index
MulleGridGetColumnX(&grid, col)        // x coordinate of column edge
MulleGridGetRowY(&grid, row)           // y coordinate of row edge
```

Out-of-range column/row returns `NAN`.


## CGRectSubdivide — Rect subtraction

`CGRectSubdivide(rect, other, output[4])` computes `rect - other`: the
remaining pieces of `rect` after removing the overlap with `other`.

Returns:
- **0** when `other` completely covers `rect` (nothing remains)
- **1** when there is no overlap (`output[0]` = original `rect`)
- **1–4** non-overlapping pieces otherwise


## CGRectEdge — Direction-based edge enum

**Not value-compatible with Apple's `CGRectEdge`.**

| Name           | Value | Apple equivalent value |
|----------------|-------|----------------------|
| CGRectMinXEdge | 4     | 0                    |
| CGRectMinYEdge | 1     | 1                    |
| CGRectMaxXEdge | 8     | 2                    |
| CGRectMaxYEdge | 2     | 3                    |

Code ported from UIKit/AppKit that passes raw integer literals (e.g. `2` for
`CGRectMaxXEdge`) will silently get the wrong edge. Always use the named
constants.


## CGContentMode — Placement and scaling

`CGContentMode` is a bitmask, not an opaque enum:

```
Bits [3:0]  positioning (CADirection flags)
  0x1=Up, 0x2=Down, 0x4=Left, 0x8=Right, 0xF=Center

Bits [7:4]  scaling
  0x10=FillH, 0x20=FillV, 0x30=ScaleToFill
  0x40=AspectFill, 0x80=AspectFit, 0xC0=ScaleAspect
```

Modes can be combined: `CGContentModeAspectFit | CGContentModeTopLeft` scales
to fit then anchors top-left instead of centering.


## CGRectPartitionIntoRowsAndColumns

Partitions a frame into a `rowSize × columnSize` grid of rects.

**Naming note:** `rowSize` means "number of rows" and `columnSize` means
"number of columns" (not "size of a row/column"). The output array must hold
`rowSize * columnSize` elements.


## Design decisions

- **No allocations.** All state fits in stack structs.
- **NULL-tolerant public API.** Every `MulleXxxFoo(p, ...)` wrapper checks
  `p` before calling `_MulleXxxFoo`. The `_`-prefixed variants assume valid
  pointers.
- **−0.0 / memcmp equality.** `MulleEdgeInsets` avoids negative zero so
  struct comparison with `memcmp` works correctly.
- **Non-Apple `CGRectUnion` semantics.** The layout relies on
  `CGRectUnion(zeroSizeRect, r)` preserving the zero-size rect's origin.
  Apple's `CGRectUnion` discards empty rects. Do not substitute Apple's
  implementation.



| Release Version                                       | Release Notes  | AI Documentation
|-------------------------------------------------------|----------------|---------------
| ![Mulle kybernetiK tag](https://img.shields.io/github/tag/mulle-gfx/mulle-layout.svg) [![Build Status](https://github.com/mulle-gfx/mulle-layout/workflows/CI/badge.svg)](//github.com/mulle-gfx/mulle-layout/actions) | [RELEASENOTES](RELEASENOTES.md) | [DeepWiki for mulle-layout](https://deepwiki.com/mulle-gfx/mulle-layout)




## Documentation & Guides

* [API Summary](asset/dox/api/toc)
* [Coder Guide](asset/howto/coder/mulle-layout)




### You are here

![Overview](overview.dot.svg)





## Add





### Add mulle-layout as a cmake subproject

In your code include its header:

``` c
#include <mulle-layout/mulle-layout.h>
```

Add mulle-layout and dependencies as git submodules and link against
them:

``` bash
git submodule add -f --name "m" \
                            "" \
                            "stash/m"
git submodule add -f --name "m" \
                            "" \
                            "stash/m"
git submodule add -f --branch * --name "mintomic" \
                            "clib:mulle-concurrent/mintomic@*" \
                            "stash/mintomic"
git submodule add -f --branch * --name "dlfcn-win32" \
                            "clib:mulle-core/dlfcn-win32@*" \
                            "stash/dlfcn-win32"
git submodule add -f --branch * --name "src/mulle-aba" \
                            "clib:mulle-concurrent/mulle-aba@*" \
                            "stash/src/mulle-aba"
git submodule add -f --branch * --name "src/mulle-allocator" \
                            "clib:mulle-c/mulle-allocator@*" \
                            "stash/src/mulle-allocator"
git submodule add -f --branch * --name "src/mulle-buffer" \
                            "clib:mulle-c/mulle-buffer@*" \
                            "stash/src/mulle-buffer"
git submodule add -f --branch * --name "src/mulle-c11" \
                            "clib:mulle-c/mulle-c11@*" \
                            "stash/src/mulle-c11"
git submodule add -f --branch * --name "src/mulle-concurrent" \
                            "clib:mulle-concurrent/mulle-concurrent@*" \
                            "stash/src/mulle-concurrent"
git submodule add -f --branch * --name "src/mulle-container-debug" \
                            "clib:mulle-c/mulle-container-debug@*" \
                            "stash/src/mulle-container-debug"
git submodule add -f --branch * --name "src/mulle-container" \
                            "clib:mulle-c/mulle-container@*" \
                            "stash/src/mulle-container"
git submodule add -f --branch * --name "src/mulle-data" \
                            "clib:mulle-c/mulle-data@*" \
                            "stash/src/mulle-data"
git submodule add -f --branch * --name "src/mulle-dlfcn" \
                            "clib:mulle-core/mulle-dlfcn@*" \
                            "stash/src/mulle-dlfcn"
git submodule add -f --branch * --name "src/mulle-dtostr" \
                            "clib:mulle-core/mulle-dtostr@*" \
                            "stash/src/mulle-dtostr"
git submodule add -f --branch * --name "src/mulle-fifo" \
                            "clib:mulle-concurrent/mulle-fifo@*" \
                            "stash/src/mulle-fifo"
git submodule add -f --branch * --name "src/mulle-fprintf" \
                            "clib:mulle-core/mulle-fprintf@*" \
                            "stash/src/mulle-fprintf"
git submodule add -f --branch * --name "src/mulle-http" \
                            "clib:mulle-c/mulle-http@*" \
                            "stash/src/mulle-http"
git submodule add -f --branch * --name "src/mulle-linkedlist" \
                            "clib:mulle-concurrent/mulle-linkedlist@*" \
                            "stash/src/mulle-linkedlist"
git submodule add -f --branch * --name "src/mulle-mmap" \
                            "clib:mulle-core/mulle-mmap@*" \
                            "stash/src/mulle-mmap"
git submodule add -f --branch * --name "src/mulle-multififo" \
                            "clib:mulle-concurrent/mulle-multififo@*" \
                            "stash/src/mulle-multififo"
git submodule add -f --branch * --name "src/mulle-rbtree" \
                            "clib:mulle-c/mulle-rbtree@*" \
                            "stash/src/mulle-rbtree"
git submodule add -f --branch * --name "src/mulle-rbtree-debug" \
                            "clib:mulle-core/mulle-rbtree-debug@*" \
                            "stash/src/mulle-rbtree-debug"
git submodule add -f --branch * --name "src/mulle-regex" \
                            "clib:mulle-c/mulle-regex@*" \
                            "stash/src/mulle-regex"
git submodule add -f --branch * --name "src/mulle-slug" \
                            "clib:mulle-c/mulle-slug@*" \
                            "stash/src/mulle-slug"
git submodule add -f --branch * --name "src/mulle-sprintf" \
                            "clib:mulle-core/mulle-sprintf@*" \
                            "stash/src/mulle-sprintf"
git submodule add -f --branch * --name "src/mulle-storage" \
                            "clib:mulle-c/mulle-storage@*" \
                            "stash/src/mulle-storage"
git submodule add -f --branch * --name "src/mulle-thread" \
                            "clib:mulle-concurrent/mulle-thread@*" \
                            "stash/src/mulle-thread"
git submodule add -f --branch * --name "src/mulle-time" \
                            "clib:mulle-core/mulle-time@*" \
                            "stash/src/mulle-time"
git submodule add -f --branch * --name "src/mulle-unicode" \
                            "clib:mulle-c/mulle-unicode@*" \
                            "stash/src/mulle-unicode"
git submodule add -f --branch * --name "src/mulle-url" \
                            "clib:mulle-c/mulle-url@*" \
                            "stash/src/mulle-url"
git submodule add -f --branch * --name "src/mulle-utf" \
                            "clib:mulle-c/mulle-utf@*" \
                            "stash/src/mulle-utf"
git submodule add -f --branch * --name "src/mulle-vararg" \
                            "clib:mulle-c/mulle-vararg@*" \
                            "stash/src/mulle-vararg"
git submodule add -f --name "stdthreads" \
                            "" \
                            "stash/stdthreads"
git submodule add -f --name "pthread" \
                            "" \
                            "stash/pthread"
git submodule add -f --name "src/mulle-spawn" \
                            "clib:mulle-core/mulle-spawn@*" \
                            "stash/src/mulle-spawn"
git submodule add -f --name "src/mulle-socket" \
                            "clib:mulle-core/mulle-socket@*" \
                            "stash/src/mulle-socket"
git submodule add -f --name "ws2_32" \
                            "" \
                            "stash/ws2_32"
git submodule add -f --name "mulle-core" \
                            "https://github.com/mulle-core/mulle-core.git" \
                            "stash/mulle-core"
git submodule add -f --name "math" \
                            "" \
                            "stash/math"
git submodule add -f --name "mulle-geometry" \
                            "https://github.com/MulleUI/mulle-geometry.git" \
                            "stash/mulle-geometry"
git submodule add -f --name "mulle-layout" \
                        "https://github.com/mulle-gfx/mulle-layout" \
                        "stash/mulle-layout"
git submodule update --init --recursive
```

``` cmake
include_directories( SYSTEM "${CMAKE_BINARY_DIR}/include")

add_subdirectory( stash/m)
add_subdirectory( stash/m)
add_subdirectory( stash/mintomic)
add_subdirectory( stash/dlfcn-win32)
add_subdirectory( stash/src/mulle-aba)
add_subdirectory( stash/src/mulle-allocator)
add_subdirectory( stash/src/mulle-buffer)
add_subdirectory( stash/src/mulle-c11)
add_subdirectory( stash/src/mulle-concurrent)
add_subdirectory( stash/src/mulle-container-debug)
add_subdirectory( stash/src/mulle-container)
add_subdirectory( stash/src/mulle-data)
add_subdirectory( stash/src/mulle-dlfcn)
add_subdirectory( stash/src/mulle-dtostr)
add_subdirectory( stash/src/mulle-fifo)
add_subdirectory( stash/src/mulle-fprintf)
add_subdirectory( stash/src/mulle-http)
add_subdirectory( stash/src/mulle-linkedlist)
add_subdirectory( stash/src/mulle-mmap)
add_subdirectory( stash/src/mulle-multififo)
add_subdirectory( stash/src/mulle-rbtree)
add_subdirectory( stash/src/mulle-rbtree-debug)
add_subdirectory( stash/src/mulle-regex)
add_subdirectory( stash/src/mulle-slug)
add_subdirectory( stash/src/mulle-sprintf)
add_subdirectory( stash/src/mulle-storage)
add_subdirectory( stash/src/mulle-thread)
add_subdirectory( stash/src/mulle-time)
add_subdirectory( stash/src/mulle-unicode)
add_subdirectory( stash/src/mulle-url)
add_subdirectory( stash/src/mulle-utf)
add_subdirectory( stash/src/mulle-vararg)
add_subdirectory( stash/stdthreads)
add_subdirectory( stash/pthread)
add_subdirectory( stash/src/mulle-spawn)
add_subdirectory( stash/src/mulle-socket)
add_subdirectory( stash/ws2_32)
add_subdirectory( stash/mulle-core)
add_subdirectory( stash/math)
add_subdirectory( stash/mulle-geometry)
add_subdirectory( stash/mulle-layout)

target_link_libraries( ${PROJECT_NAME} PUBLIC m)
target_link_libraries( ${PROJECT_NAME} PUBLIC m)
target_link_libraries( ${PROJECT_NAME} PUBLIC mintomic)
target_link_libraries( ${PROJECT_NAME} PUBLIC dlfcn-win32)
target_link_libraries( ${PROJECT_NAME} PUBLIC src/mulle-aba)
target_link_libraries( ${PROJECT_NAME} PUBLIC src/mulle-allocator)
target_link_libraries( ${PROJECT_NAME} PUBLIC src/mulle-buffer)
target_link_libraries( ${PROJECT_NAME} PUBLIC src/mulle-c11)
target_link_libraries( ${PROJECT_NAME} PUBLIC src/mulle-concurrent)
target_link_libraries( ${PROJECT_NAME} PUBLIC src/mulle-container-debug)
target_link_libraries( ${PROJECT_NAME} PUBLIC src/mulle-container)
target_link_libraries( ${PROJECT_NAME} PUBLIC src/mulle-data)
target_link_libraries( ${PROJECT_NAME} PUBLIC src/mulle-dlfcn)
target_link_libraries( ${PROJECT_NAME} PUBLIC src/mulle-dtostr)
target_link_libraries( ${PROJECT_NAME} PUBLIC src/mulle-fifo)
target_link_libraries( ${PROJECT_NAME} PUBLIC src/mulle-fprintf)
target_link_libraries( ${PROJECT_NAME} PUBLIC src/mulle-http)
target_link_libraries( ${PROJECT_NAME} PUBLIC src/mulle-linkedlist)
target_link_libraries( ${PROJECT_NAME} PUBLIC src/mulle-mmap)
target_link_libraries( ${PROJECT_NAME} PUBLIC src/mulle-multififo)
target_link_libraries( ${PROJECT_NAME} PUBLIC src/mulle-rbtree)
target_link_libraries( ${PROJECT_NAME} PUBLIC src/mulle-rbtree-debug)
target_link_libraries( ${PROJECT_NAME} PUBLIC src/mulle-regex)
target_link_libraries( ${PROJECT_NAME} PUBLIC src/mulle-slug)
target_link_libraries( ${PROJECT_NAME} PUBLIC src/mulle-sprintf)
target_link_libraries( ${PROJECT_NAME} PUBLIC src/mulle-storage)
target_link_libraries( ${PROJECT_NAME} PUBLIC src/mulle-thread)
target_link_libraries( ${PROJECT_NAME} PUBLIC src/mulle-time)
target_link_libraries( ${PROJECT_NAME} PUBLIC src/mulle-unicode)
target_link_libraries( ${PROJECT_NAME} PUBLIC src/mulle-url)
target_link_libraries( ${PROJECT_NAME} PUBLIC src/mulle-utf)
target_link_libraries( ${PROJECT_NAME} PUBLIC src/mulle-vararg)
target_link_libraries( ${PROJECT_NAME} PUBLIC stdthreads)
target_link_libraries( ${PROJECT_NAME} PUBLIC pthread)
target_link_libraries( ${PROJECT_NAME} PUBLIC src/mulle-spawn)
target_link_libraries( ${PROJECT_NAME} PUBLIC src/mulle-socket)
target_link_libraries( ${PROJECT_NAME} PUBLIC ws2_32)
target_link_libraries( ${PROJECT_NAME} PUBLIC mulle-core)
target_link_libraries( ${PROJECT_NAME} PUBLIC math)
target_link_libraries( ${PROJECT_NAME} PUBLIC mulle-geometry)
target_link_libraries( ${PROJECT_NAME} PUBLIC mulle-layout)
```

Or let [mulle-sde](//github.com/mulle-sde) do it:

``` sh
mulle-sde add github:mulle-gfx/mulle-layout
```

## Install

Use [mulle-sde](//github.com/mulle-sde) to build and install mulle-layout and all dependencies:

``` sh
mulle-sde install --prefix /usr/local \
   https://github.com/mulle-gfx/mulle-layout/archive/latest.tar.gz
```

### Legacy Installation

Install the requirements:

| Requirements                                 | Description
|----------------------------------------------|-----------------------
| [mulle-geometry](https://github.com/MulleUI/mulle-geometry)             | 🔭 Basic geometric types

Download the latest [tar](https://github.com/mulle-gfx/mulle-layout/archive/refs/tags/latest.tar.gz) or [zip](https://github.com/mulle-gfx/mulle-layout/archive/refs/tags/latest.zip) archive and unpack it.

Install **mulle-layout** into `/usr/local` with [cmake](https://cmake.org):

``` sh
PREFIX_DIR="/usr/local"
cmake -B build                               \
      -DMULLE_SDK_PATH="${PREFIX_DIR}"       \
      -DCMAKE_INSTALL_PREFIX="${PREFIX_DIR}" \
      -DCMAKE_PREFIX_PATH="${PREFIX_DIR}"    \
      -DCMAKE_BUILD_TYPE=Release &&
cmake --build build --config Release &&
cmake --install build --config Release
```


## Author

[Nat!](https://mulle-kybernetik.com/weblog) for Mulle kybernetiK  



