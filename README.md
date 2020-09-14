

The Mandelbrot Set And The DEM Algorithms
-------------------------------------

### Intro

This repo's purpose was to enable fractal representations for my graduate thesis. The searching for Julia sets consisted of first defining a path along, which coefficients of a polynomial were defined. This creates a series of Julia images, which are joint into a video. Those programs are written in **C++**, inside the folder *cpptools*. When a set of coefficients, yielding a desirable Julia set, was discovered, the drawing was made using the *fractal_script.py* (written in **Python**) program inside the *pytools* folder. The program enables a variety of arguments for plotting the Julia and Mandelbrot sets.

For a deeper understanding of the Mandelbrot and Julia sets, repository features my graduate paper (in Slovene). It includes some of the theorems, that enable derivation of the *DEM* algorithm and some other approximations used by the written algorithms.

### The Math

The main two theorems, which enables the so-called escape radius to be calculated are the theorems 4.7 and 5.9 inside *diploma.pdf*. They are independent of the rest of the paper. The DEM algorithm is explained in section 6 and relies on most of the theory, derived before.

### Examples

You can try calling the *fractal_script.py* fractal utility from the terminal with  the following commands

- `python fractal_script.py "1 0 -0.7510894579318156+0.11771693494277351j" -cm cubehelix -cmo normal`
- ` python fractal_script.py "1 0 -0.7510894579318156+0.11771693494277351j" -cm cubehelix -cmo normal -cpc 91 4 0.25`
- ` python fractal_script.py "1 0 0.1567002004882749+0.6527033090669409j" -cm magma -it 1000 -cmo normal -cpc 85 2 0.25`
- `python fractal_script.py mandelbrot -cm gist_stern -it 200`
- `python fractal_script.py mandelbrot -alg escapetime -cm gist_stern -it 100 -sh True`

The files created are in the *PPM* format. For conversion into *PNG*, *JPG* or other, you can use [GIMP](https://www.gimp.org/).

### To do

- [x] Julia video implementation for custom polynomial
- [x] Mandelbrot zoom 
- [x] Python single image drawing utility for Julia sets
- [x] Python single image drawing utility for the Mandelbrot set
- [x] Theoretic proofs of the theorems used
- [ ] Embedding latex images onto README.md