# mulle-layout

#### 📏 Rectangle geometry and manual layouting


# 📏 mulle-layout

#### 📏 Rectangle geometry and manual layouting


This library contains functions for manual layouting of rectangles.

| Datatype             | Description                                                      |
|----------------------|------------------------------------------------------------------|
| `CGRectEdge`         | legacy compatibility enum for CADirection                        |
| `CGContentMode`      | an enumeration of various placements like `CGContentModeTopLeft` |
| `struct MulleLayout` | layout rectangles in row column fashion                          |
| `struct MulleGrid`   | layout rectangles in a uniform grid                              |



![Demo Screenshot](demos/layout.png)

Here you can see a demo of the library in action. The available window space
is split into vertical and horizontal slices using "paritioning".

In the first row you see "contentmode" examples of a rectangle inside
another rectangle.

In the second and third row, you see "placement" of a rectangle with fixed
size relative to another rectangle of fixed size.

The fourth row, shows "placement" of a rectangle of "flexible" size
relative to another rectangle of fixes size.

The last row, shows the "layout" code, placing rectangles in column first,
then row fashion.





| Release Version                                       | Release Notes
|-------------------------------------------------------|--------------
| ![Mulle kybernetiK tag](https://img.shields.io/github/tag/mulle-gfx/mulle-layout.svg?branch=release) [![Build Status](https://github.com/mulle-gfx/mulle-layout/workflows/CI/badge.svg?branch=release)](//github.com/mulle-gfx/mulle-layout/actions) | [RELEASENOTES](RELEASENOTES.md) |






### You are here

![Overview](overview.dot.svg)





## Add

**This project is a component of the [mulle-core](//github.com/mulle-core/mulle-core) library. As such you usually will *not* add or install it
individually, unless you specifically do not want to link against
`mulle-core`.**


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
| [mulle-geometry](https://github.com/MulleUI/mulle-geometry)             | 

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



