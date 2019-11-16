

Complex Polynomial Dynamics
-------------------------------------


### Basics
An effort at drawing various types of fractals, 
through the iteration of complex polynomials.

Thus far plotting was done for the Mandelbrot set
and Julia sets of various custom polynomials.
There is also an option to convert multiple 
images of Julia sets across a parametrized set
of complex polynomials (usually with coefficients
being defined as a complex path) to a video.

Programs producing multiple images (later used to
be merged into a video) are written in **C++**,  while
the programs used for producing a single image
are in **Python** and accept terminal provided
arguments for single image customization.

### The Math

Algorithms take advantage of a theorem, which states
that if |z| > max(|C|, 2), then iterations of map
z -> z ^ 2 + C will diverge towards infinity, 
and a more complex equation which gives 
an estimation for custom polynomial's convergence
radius.

### To do

- [x] Julia video implementation for custom polynomial
- [ ] Mandelbrot zoom 
- [x] Python single image drawing utility for Julia sets
- [x] Python single image drawing utility for the Mandelbrot set
- [ ] Theoretic explanation of the Julia video
- [ ] Theoretic proofs of the theorems used
- [ ] Embedding latex images onto README.md