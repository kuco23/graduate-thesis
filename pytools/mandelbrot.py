"""
draw one instance of the mandelbrot set,
with custom parameters
"""

from math import log
from cmath import sqrt
from matplotlib import cm
from matplotlib.colors import Normalize
import numpy as np

overflow = 10**10
quadabs = lambda z: z.real * z.real + z.imag * z.imag

def drawMandelbrotPPM(
    ppm, center, radius, alg, px, cmap, order
):
    iterlim = 600
    cx, cy = center
    ReS, ReT = cx - radius, cx + radius
    ImS, ImT = cy - radius, cy + radius

    ppm.writelines(['P3\n', f'{px} {px}\n', '255\n'])
    colormap = cm.get_cmap(cmap)
    if order == 'reversed': colormap = colormap.reversed()

    def pointGenerator():
        dim = dre = 2 * radius / px
        dz = complex(dre, 0)
        zk = complex(ReS, ImS)
        for i in range(px):
            for j in range(px):
                zk += dz
                yield zk
            zk = complex(ReS, zk.imag + dim)
            
    def inCardioidCircle(point):
        t = sqrt(1 - 4 * point)
        return (
            abs(1 - t) < 1 or
            abs(1 + t) < 1 or
            abs(1 + point) < 0.25
        )

    def escapetime():
        gradient = [
            ' '.join(map(str, map(round, rgba[:3])))
            for rgba in 255 * colormap(np.linspace(0, 1, iterlim))
        ] + ['0 0 0']
        
        def escapetimeCount(c):
            ck, count = complex(0, 0), 0
            while count < iterlim and quadabs(ck) <= 4:
                ck = ck * ck + c
                count += 1
            return count

        points = pointGenerator()
        for i in range(px):
            ppm.write('\n')
            for j in range(px):
                count = escapetimeCount(next(points))
                ppm.write(gradient[count] + '  ')

    def dem():
        pixels = np.empty((px, px), dtype=float)
            
        def demCount(c):
            ck, dk = c, 1
            for _ in range(iterlim):
                if max(
                    abs(ck.real) + abs(ck.imag),
                    abs(dk.real) + abs(dk.imag)
                ) > overflow: break
                dk = 2 * ck * dk + 1
                ck = ck * ck + c
            absck = abs(ck)
            if absck <= 2: return 0
            else:
                estimate = log(absck, 2) * absck / abs(dk)
                return -log(estimate)

        points = pointGenerator()
        for j in range(px):
            for i in range(px):
                pixels[i][j] = demCount(next(points))

        M = pixels.max()
        pixels[pixels == 0] = M
        norm = Normalize(pixels.min(), M)
        gradient = colormap(norm(pixels))
        for j in range(px):
            ppm.write('\n')
            for i in range(px):
                rgb = map(round, 255 * gradient[i][j][:3])
                ppm.write(' '.join(map(str, rgb)) + '  ')

    locals()[alg]()
        

if __name__ == '__main__':

    from argparse import ArgumentParser

    args = ArgumentParser()
    args.add_argument(
        '-c', metavar='center point',
        type=float, nargs=2,
        default=(0, 0)
    )
    args.add_argument(
        '-r', metavar='radius',
        type=float, default=2,
    )
    args.add_argument(
        '-px', metavar='n x n pixels',
        type=int, default=1000,
        help='image pixels'
    )
    args.add_argument(
        '-alg', metavar='algorithm',
        type=str, default='escapetime',
        choices = ['escapetime', 'dem'],
        help='algorithm name'
    )
    args.add_argument(
        '-cm', metavar='colormap',
        type=str, default='viridis',
        help='color mapping'
    )
    args.add_argument(
        '-cmo', metavar='colormap order',
        type=str, default='reversed',
        choices = ['normal', 'reversed'],
        help='color mapping order'
    )
    args.add_argument(
        '-fn', metavar='file name',
        type=str, default='mandelbrot',
        help='file name'
    )
    vals = args.parse_args()

    ppm_file = vals.fn + '.ppm'
    open(ppm_file, 'a').close()
    with open(ppm_file, 'w', encoding='ascii') as ppm:
        drawMandelbrotPPM(
            ppm, vals.c, vals.r, vals.alg,
            vals.px, vals.cm, vals.cmo
        )
