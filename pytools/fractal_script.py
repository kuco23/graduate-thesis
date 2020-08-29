"""
Disclaimer: koda ni clovesko berljiva
Nariši sliko Juliajeve ali Mandelbrotove množice,
pri poljubnih parametrih.
"""

import math, cmath
from functools import reduce
from matplotlib import cm
from matplotlib.colors import Normalize
import numpy as np

I = 0 + 1j
overflow = 10**10
quadabs = lambda z: z.real * z.real + z.imag * z.imag

def diferentiate(poly):
    n, an = len(poly) - 1, poly[0]
    return [
        (n - i) * an for (i, an) in enumerate(poly[:-1])
    ]

def horner(p, z):
    return reduce(lambda x, y: z * x + y, p)

def pointGenerator(ReS, ImS, radius, px):
    dim = dre = 2 * radius / px
    dz = complex(dre, 0)
    zk = complex(ReS, ImS)
    for i in range(px):
        for j in range(px):
            zk += dz
            yield zk
        zk = complex(ReS, zk.imag + dim)

def drawJuliaPolynomialPPM(
    ppm, poly, center, radius, alg, iterlim,
    px, cmap, order, power, shift, perc
):
    phisplit = 80
    L = 1.0001
    
    n = len(poly) - 1
    an = abs(poly[0])
    C = sum(map(abs, poly)) - an
    eps = max(1, 2 * C / 2, pow(2 * L / an, 1 / (n-1)))
    eps_quad = pow(eps, 2)

    def escapetimeCount(z, lim=iterlim):
        zk, count = z, 0
        while count < lim and quadabs(zk) <= eps_quad:
            zk = horner(poly, zk)
            count += 1
        return count
        
    def escapetime():
        gradient = [
            ' '.join(map(str, map(round, rgba[:3])))
            for rgba in 255 * colormap(
                pow(np.linspace(0, 1, iterlim), power)
            )
        ] + ['0 0 0']
        points = pointGenerator()
        for i in range(px):
            ppm.write('\n')
            for j in range(px):
                count = escapetimeCount(next(points))
                ppm.write(gradient[count] + '  ')
            
    def dem():
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
                estimate = math.log2(abszk) * abszk / abs(dk)
                return -math.log2(estimate)

        dpdz = diferentiate(poly)
        points = pointGenerator(ReS, ImS, radius, px)
        pixels = np.empty((px, px), dtype=float)
        for j in range(px):
            for i in range(px):
                pixels[i][j] = demCount(next(points))
                
        m, M = pixels.min(), pixels.max()
        pixels[pixels == 0] = m if shift else M
        normed = Normalize(m, M)(pixels)
    
        if perc is not None:
            p, pf, pc = perc
            q = np.percentile(normed, p)
            filt = normed <= q
            normed[filt] = pow(normed[filt], pf)
            normed[~filt] = pow(normed[~filt], pc)
        elif power != 1:
            normed = pow(normed, power)

        gradient = colormap(normed)
        for j in range(px):
            ppm.write('\n')
            for i in range(px):
                rgb = map(round, 255 * gradient[i][j][:3])
                ppm.write(' '.join(map(str, rgb)) + '  ')

    def simulatedEps():
        maxradius = 0
        r = eps / px
        rquad = r * r
        phi, dphi = 0, 2 * math.pi / phisplit
        while phi < 2 * math.pi:
            dz = r * cmath.exp(I * phi)
            z = px * dz
            while quadabs(z) > rquad:
                count = escapetimeCount(z, 3)
                if count > 2: break
                z -= dz
            if quadabs(z) > maxradius:
                maxradius = quadabs(z)
            phi += dphi
        return math.sqrt(maxradius) + 10 * r

    if not radius: radius = simulatedEps()
    cx, cy = center
    ReS, ReT = cx - radius, cx + radius
    ImS, ImT = cy - radius, cy + radius

    ppm.writelines(['P3\n', f'{px} {px}\n', '255\n'])
    colormap = cm.get_cmap(cmap)
    if order == 'reversed': colormap = colormap.reversed()

    locals()[alg]()


def drawMandelbrotPPM(
    ppm, center, radius, alg, iterlim,
    px, cmap, order, cmp, shift
):
    cx, cy = center
    ReS, ReT = cx - radius, cx + radius
    ImS, ImT = cy - radius, cy + radius

    ppm.writelines(['P3\n', f'{px} {px}\n', '255\n'])
    colormap = cm.get_cmap(cmap)
    if order == 'reversed': colormap = colormap.reversed()

    def escapetime():
        gradient = [
            ' '.join(map(str, map(round, rgba[:3])))
            for rgba in 255 * colormap(
                pow(np.linspace(0, 1, iterlim), cmp)
            )
        ] + ['0 0 0']
        
        def escapetimeCount(c):
            ck, count = complex(0, 0), 0
            while count < iterlim and quadabs(ck) <= 4:
                ck *= ck
                ck += c
                count += 1
            return count

        points = pointGenerator(ReS, ImS, radius, px)
        for i in range(px):
            ppm.write('\n')
            for j in range(px):
                count = escapetimeCount(next(points))
                ppm.write(gradient[count] + '  ')

    def dem():  
        def demCount(c):
            ck, dk = c, 1
            for _ in range(iterlim):
                if max(
                    abs(ck.real) + abs(ck.imag),
                    abs(dk.real) + abs(dk.imag)
                ) > overflow: break
                dk = 2 * ck * dk + 1
                ck *= ck
                ck += c
            absck = abs(ck)
            if absck <= 2: return 0
            else:
                estimate = math.log2(absck) * absck / abs(dk)
                return -math.log2(estimate)

        values = np.empty((px, px), dtype=float)
        points = pointGenerator()
        for j in range(px):
            for i in range(px):
                values[i][j] = demCount(next(points))

        m, M = values.min(), values.max()
        values[values == 0] = m if shift else M
        norm = PowerNorm(cmp, m, M)
        gradient = colormap(norm(values))
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
        'poly', type = str,
        metavar = 'polynomial coefficients'
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
        '-it', metavar='n iterations',
        type=int, default=250,
        help='number of iterations to aproximate limit'
    )
    args.add_argument(
        '-alg', metavar='algorithm',
        type=str, default='dem',
        choices = ['escapetime', 'dem'],
        help='algorithm name'
    )
    args.add_argument(
        '-cm', metavar='colormap',
        type=str, default='viridis',
        help='color mapping'
    )
    args.add_argument(
        '-cmp', metavar='colormap power',
        type=float, default=1,
        help='colormap power normalization'
    )
    args.add_argument(
        '-cmo', metavar='colormap order',
        type=str, default='reversed',
        choices = ['normal', 'reversed'],
        help='color mapping order'
    )
    args.add_argument(
        '-csh', metavar='color shift',
        type=bool, default=False,
        help='color the set interior with the outer most color'
    )
    args.add_argument(
        '-cpc', metavar='colormap percentile',
        type=float, default=None, nargs=3,
        help='percentil of values to lower of which apply cmp'
    )
    args.add_argument(
        '-fn', metavar='file name',
        type=str, default='fractal',
        help='file name'
    )
    vals = args.parse_args()
    
    filename = vals.fn + '.ppm'
    open(filename, 'a').close()
    with open(filename, 'w', encoding='utf-8') as ppm:
        if vals.poly == 'mandelbrot':
            drawMandelbrotPPM(
                ppm, vals.c, vals.r or 2.2, vals.alg, vals.it,
                vals.px, vals.cm, vals.cmo, vals.cmp, vals.csh,
                vals.cpc
            )   
        else:
            poly = list(map(complex, vals.poly.split()))
            drawJuliaPolynomialPPM(
                ppm, poly, vals.c, vals.r, vals.alg, vals.it,
                vals.px, vals.cm, vals.cmo, vals.cmp, vals.csh,
                vals.cpc
            )
