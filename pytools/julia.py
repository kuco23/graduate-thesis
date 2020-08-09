"""
draw one instance of the Julia set for a specific
polynomial, with custom parameters
"""

from math import log
from cmath import sqrt
from functools import reduce
from matplotlib import cm
from matplotlib.colors import Normalize
import numpy as np

overflow = 10**10
quadabs = lambda z: z.real * z.real + z.imag * z.imag

def drawJuliaPolynomialPPM(
    ppm, poly, center, radius, alg, px, cmap, order
):
    iterlim = 400
    L = 1.0001
    
    n = len(poly) - 1
    an = abs(poly[0])
    C = sum(map(abs, poly)) - an
    eps = max(1, 2 * C / 2, pow(2 * L / an, 1 / (n-1)))
    eps_quad = pow(eps, 2)

    if not radius: radius = eps
    cx, cy = center
    ReS, ReT = cx - radius, cx + radius
    ImS, ImT = cy - radius, cy + radius

    ppm.writelines(['P3\n', f'{px} {px}\n', '255\n'])
    colormap = cm.get_cmap(cmap)
    if order == 'reversed': colormap = colormap.reversed()

    def horner(p, z):
        return reduce(lambda x, y: z * x + y, p)

    def pointGenerator():
        dim = dre = 2 * radius / px
        dz = complex(dre, 0)
        zk = complex(ReS, ImS)
        for i in range(px):
            for j in range(px):
                zk += dz
                yield zk
            zk = complex(ReS, zk.imag + dim)

    def escapetime():
        gradient = [
            ' '.join(map(str, map(round, rgba[:3])))
            for rgba in 255 * colormap(np.linspace(0, 1, iterlim))
        ] + ['0 0 0']
        
        def escapetimeCount(z):
            zk, count = z, 0
            while count < iterlim and quadabs(zk) <= eps_quad:
                zk = horner(poly, zk)
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
        dpdz = [(n - i) * an for (i, an) in enumerate(poly[:-1])]
            
        def demCount(z):
            zk, dk = z, 1
            for _ in range(iterlim):
                if max(
                    abs(zk.real) + abs(zk.imag),
                    abs(dk.real) + abs(dk.imag)
                ) > overflow: break
                dk = horner(dpdz, zk) * dk
                zk = horner(poly, zk)
            abszk = abs(zk)
            if abszk < eps: return 0
            else:
                estimate = log(abszk, 2) * abszk / abs(dk)
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
        'coefs', metavar = 'polynomial coefficients',
        type = str
    )
    args.add_argument(
        '-c', metavar='center point',
        type=float, nargs=2,
        default=(0, 0)
    )
    args.add_argument(
        '-r', metavar='radius',
        type=float, default=None,
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
        type=str, default='julia',
        help='file name'
    )
    vals = args.parse_args()
    coefs = list(map(complex, vals.coefs.split()))
    
    filename = vals.fn + '.ppm'
    open(filename, 'a').close()
    with open(filename, 'w', encoding='utf-8') as ppm:
        drawJuliaPolynomialPPM(
            ppm, coefs, vals.c, vals.r, vals.alg,
            vals.px, vals.cm, vals.cmo
        )
