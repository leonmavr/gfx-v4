# gfx-v4

## About
A simple 2D graphics library in C for educational purposes. Originally written by [Prof Douglas Thain](https://www3.nd.edu/~dthain/courses/cse20211/fall2013/gfx/).  


The library has been modified to implement
* Bresenham's line drawing algorithm
* a couple of triangle fills
* translations and rotations

A Makefile was added as well.

For the future I'm planning to implement
* interpolate the colour of each triangle pixel from its vertices
* Bresenham's circle drawing algorithm


## Folder structure
Source code is found in src directory. The animation played when the project is compiled and run is found in `example.c`.
You can define your own animations there. Try creating an analogue clock.  

By default, 3 triangles spinning clockwise about their centroids are rendered and shown.  


## How to compile
On Linux, compile with
```
make
```
Run with
```
./example
```
If everything is compiled correctly, you should see the following animation in a new window:
![demo](https://raw.githubusercontent.com/0xLeo/gfx-v4/master/assets/demo_triangles.gif)

Clean generated executables and object files with:
```
make clean
```
