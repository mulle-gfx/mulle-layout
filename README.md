# mulle-layout

#### 📏 Rectangle geometry and manual layouting

This library contains functions for manual layouting of rectangles.

| Datatype             | Description                                                      |
|----------------------|------------------------------------------------------------------|
| `CGRectEdge`         | legacy compatibility enum for CADirection                        |
| `CGContentMode`      | an enumeration of various placements like `CGContentModeTopLeft` |
| `struct MulleLayout` | layout rectangles in row column fashion                          |
| `struct MulleGrid`   | layout rectangles in a uniform grid                              |



![Demo Screenshot](demo/layout.png)

Here you can see a demo of the library in action. The available window space
is split into vertical and horizontal slices using "partitioning".

In the first row you see "contentMode" examples of a rectangle inside
another rectangle.

In the second and third row, you see "placement" of a rectangle with fixed
size relative to another rectangle of fixed size.

The fourth row, shows "placement" of a rectangle of "flexible" size
relative to another rectangle of fixed size.

The last row, shows the "layout" code, placing rectangles in column first,
then row fashion.





| Release Version                                       | Release Notes  | AI Documentation
|-------------------------------------------------------|----------------|---------------
| ![Mulle kybernetiK tag](https://img.shields.io/github/tag/mulle-gfx/mulle-layout.svg) [![Build Status](https://github.com/mulle-gfx/mulle-layout/workflows/CI/badge.svg)](//github.com/mulle-gfx/mulle-layout/actions) | [RELEASENOTES](RELEASENOTES.md) | [DeepWiki for mulle-layout](https://deepwiki.com/mulle-gfx/mulle-layout)




## Documentation & Guides

* [API Summary](asset/dox/api/toc)
* [Coder Guide](asset/howto/coder/mulle-layout)

## Usage

Manual, incremental layout of rectangles in rows and columns. Neighboring
margins merge (the larger one wins), and a rectangle that does not fit
starts a new row:

``` c
#include <mulle-layout/mulle-layout.h>

int  main( void)
{
   struct MulleLayout  layout;

   MulleLayoutInit( &layout, CGRectMake( 0, 0, 180, 60), MulleEdgeInsetsZero);

   for( int i = 0; i < 5; i++)
   {
      CGRect  r = MulleLayoutAddToRow( &layout,
                                       CGSizeMake( 50, 20),
                                       MulleEdgeInsetsMake( 0, 10, 0, 10));
      if( CGRectIsNull( r))                 /* didn't fit -> new row */
      {
         MulleLayoutNewRow( &layout);
         r = MulleLayoutAddToRow( &layout,
                                  CGSizeMake( 50, 20),
                                  MulleEdgeInsetsMake( 0, 10, 0, 10));
      }
      printf( "slot %d at (%g, %g)\n", i, r.origin.x, r.origin.y);
   }
   return( 0);
}
```


### You are here

![Overview](overview.dot.svg)





## Add

**This project is a component of the [mulle-gfx](//github.com/mulle-gfx/mulle-gfx) library.
As such you usually will *not* add or install it individually, unless you
specifically do not want to link against `mulle-gfx`.**


### Add as an individual component

Use [mulle-sde](//github.com/mulle-sde) to add mulle-layout to your project:

``` sh
mulle-sde add github:mulle-gfx/mulle-layout
```

To only add the sources of mulle-layout with dependency
sources use [clib](https://github.com/clibs/clib):


``` sh
clib install --out src/mulle-gfx mulle-gfx/mulle-layout
```

Add `-isystem src/mulle-gfx` to your `CFLAGS` and compile all the sources that were downloaded with your project.


## Install

### Install with mulle-sde

Use [mulle-sde](//github.com/mulle-sde) to build and install mulle-layout and all dependencies:

``` sh
mulle-sde install --prefix /usr/local \
   https://github.com/mulle-gfx/mulle-layout/archive/latest.tar.gz
```

### Manual Installation

Install the requirements:

| Requirements                                 | Description
|----------------------------------------------|-----------------------
| [mulle-geometry](https://github.com/MulleUI/mulle-geometry)             | 🔭 Basic geometric types

Install **mulle-layout** into `/usr/local` with [cmake](https://cmake.org):

``` sh
cmake -B build \
      -DCMAKE_INSTALL_PREFIX=/usr/local \
      -DCMAKE_PREFIX_PATH=/usr/local \
      -DCMAKE_BUILD_TYPE=Release &&
cmake --build build --config Release &&
cmake --install build --config Release
```


## Author

[Nat!](https://mulle-kybernetik.com/weblog) for Mulle kybernetiK  



