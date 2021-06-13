# gfx-v4

## About
A simple 2D graphics library in C for educational purposes. Originally written by [Prof Douglas Thain](https://www3.nd.edu/~dthain/courses/cse20211/fall2013/gfx/).  

Compared to Prof Thain's original implementation, this library implements the following additions:
- [x] Bresenham's line drawing algorithm
- [x] a couple of triangle fills
- [x] midpoint circle drawing algorithm
- [x] translations and rotations  
- [x] Triangle fill by colour smearing from the vertices  

Planned for the future:
- [ ] Unit tests
- [ ] Bezier curves

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

Clean generated executables and object files with:
```
make clean
```
