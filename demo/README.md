# demos

#### ⛺ layout demos

The demos require MulleUI to run.


## layout

This is a showcase of the various layout algorithms. The demo is using
"partitioning", which is the cutting up of a rectangle into rows and columns.

First the window space is paritioned into five rows, and then each row is
partioned into the required amount of columns.

The first row demos "contentMode", which is the placement of a rectangle
within another rectangle.

The second and third rows are demos of "placement" a rectangle is placed in
relationship to another rectangle with disregard to the surrounding space.

The fourth row is a demo of placing two rectangles adjacent in a surrounding
space with some alignment.

The fifth row demos "manual" layouting. Rectangles are added in column row
fashion (the last rectangle won't fit).

![layout](layout.png)
[src/main-layout.m]()


## Build and Run

This is a [mulle-sde](https://mulle-sde.github.io/) project.

It comes with its own virtual environment and list of dependencies.
To fetch and build everything say:

```
mulle-sde craft
```

Then run the executables found in `kitchen/Debug`.
