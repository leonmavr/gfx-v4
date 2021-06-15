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
