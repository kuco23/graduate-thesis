"""
draw one instance of the mandelbrot 
set with custom parameters
"""

iterlim = 40
iterfun = lambda z, c: z * z + c

def drawMandelbrotPPM(file, px, re, im, col):
    (r1, r2), (i1, i2) = re, im

    def setPoint(i, j):
        x = i / px * (r2 - r1) + r1
        y = j / px * (i2 - i1) + i1
        return complex(x, y)

    def pointConvergance(point):
        eps = max(2, abs(point))
        z, count = complex(0, 0), 0
        while count < iterlim and abs(z) <= eps:
            z = iterfun(z, point)
            count += 1
        return count
    
    def getGradientBase(count):
        if count == iterlim:
            return 0
        return (255 * count) // iterlim

    ppm_header = ['P3\n', f'{px} {px}\n', '255\n']
    for h in ppm_header: file.write(h)
        
    for j in range(px):
        file.write('\n')
        for i in range(px):
            point = setPoint(i, j)
            count = pointConvergance(point)
            base = getGradientBase(count)
            r, g, b = map(lambda c: (c / 255) * base, col)
            file.write(f'{r} {g} {b}  ')
            

if __name__ == '__main__':

    from argparse import ArgumentParser

    args = ArgumentParser()
    args.add_argument(
        '-re', metavar='real axis range',
        type=float, nargs=2,
        default=(-2, 2)
    )
    args.add_argument(
        '-im', metavar='imaginary axis range',
        type=float, nargs=2,
        default=(-2, 2)
    )
    args.add_argument(
        '-rgb', metavar='color gradient',
        type=float, nargs=3,
        default=(256, 0, 0)
    )
    args.add_argument(
        '-fn', metavar='file name',
        type=str, default='mandelbrot'
    )
    args.add_argument(
        '-px', metavar='n x n pixels',
        type=int, default=1000
    )
    vals = args.parse_args()

    ppm_file = vals.fn + '.ppm'
    open(ppm_file, 'a').close()
    with open(ppm_file, 'w', encoding='ascii') as f:
        drawMandelbrotPPM(
            f, vals.px, vals.re, vals.im, vals.rgb
        )
