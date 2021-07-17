# gfx-v4

## About
A simple 2D graphics library in C for educational purposes. It's written on top of the X desktop manager. It's able to draw lines (straight or curvy), circles, and triangles. Originally written by [Prof Douglas Thain](https://www3.nd.edu/~dthain/courses/cse20211/fall2013/gfx/).  

Compared to Prof Thain's original implementation, this library implements the following additions:
- [x] Bresenham's line drawing algorithm
- [x] a couple of triangle fills
- [x] midpoint circle drawing algorithm
- [x] point translations and rotations
- [x] Triangle fill by colour smearing from the vertices
- [x] Bezier curves (by De Chasteljau's algorithm)

Planned for the future:
- [ ] A directory which contains examples that showcase each algo I implemented instead of a central `example.c` file
- [ ] Unit tests

If some algorithm is not straightforward, the source code is complemented by [these notes](https://github.com/0xLeo/journal/tree/master/graphics).

## How to compile and run
On Linux, compile with
```
make
```
Run with
```
./example
```
This should output the following drawing:  

<img src="https://github.com/0xLeo/gfx-v4/blob/master/assets/example_c.png " height="175" />

Clean generated executables and object files with:
```
make clean
```
To change the output on the screen, edit `example.c`. Below is a more advanced drawing that demonstrates the library's capabilities.  

![](https://github.com/0xLeo/gfx-v4/blob/master/assets/triangle_fill.gif)
