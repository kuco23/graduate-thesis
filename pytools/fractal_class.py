"""
draw one instance of the Julia or Mandelbrot set,
with custom parameters
"""

from math import log, pi, sqrt
import cmath
from functools import reduce
from abc import ABC

from matplotlib import cm
from matplotlib.colors import PowerNorm
import numpy as np

I = 0 + 1j
overflow = 10**10
qabs = lambda z: z.real * z.real + z.imag * z.imag

phisplit = 80
L = 1.0001

class FractalImagePPM(ABC):

    def __init__(
        self, center, radius, eps,
        alg, px, iterlim,
        cmap, cmp, cmo
    ):
        self.px = px
        self.alg = alg
        self.lim = iterlim
        self.ct = center
        self.eps = eps
        self._qeps = eps * eps
        self.rd = radius or self._simulatedRadius()
        self.cm = cm.get_cmap(cmap)
        self.cmp = cmp

        if cmo == 'reversed':
            self.cm = self.cm.reversed()
            
        cx, cy = center
        self.re1, self.re2 = cx - self.rd, cx + self.rd
        self.im1, self.im2 = cy - self.rd, cy + self.rd

    def _iter(self, z, zk):
        raise NotImplementedError

    def _deriv(self, zk, dk):
        raise NotImplementedError
        
    def _pointGenerator(self):
        dim = dre = 2 * self.rd / self.px
        dz = complex(dre, 0)
        zk = complex(self.re1, self.im1)
        for i in range(self.px):
            for j in range(self.px):
                zk += dz
                yield zk
            zk = complex(self.re1, zk.imag + dim)

    def _simulatedRadius(self):
        maxradius = 0
        r = self.eps / self.px
        rquad = r * r
        phi, dphi = 0, 2 * pi / phisplit
        while phi < 2 * pi:
            dz = r * cmath.exp(I * phi)
            z = self.px * dz
            while qabs(z) > rquad:
                count = self._escapetimeCount(z, 3)
                if count > 2: break
                z -= dz
            if qabs(z) > maxradius:
                maxradius = qabs(z)
            phi += dphi
        return sqrt(maxradius) + 2 * r

    def _escapetimeCount(self, z, K=None):
        lim = K or self.lim
        zk, count = z, 0
        while count < lim and qabs(zk) <= self._qeps:
            zk = self._iter(z, zk)
            count += 1
        return count

    def _demCount(self, z):
        zk, dk = z, 1
        for _ in range(self.lim):
            if max(
                abs(zk.real) + abs(zk.imag),
                abs(dk.real) + abs(dk.imag)
            ) > overflow: break
            dk = self._deriv(zk, dk)
            ck = self._iter(zk, z)
        zknorm = abs(zk)
        if zknorm <= self.eps: return 0
        else:
            estimate = log(zknorm, 2) * zknorm / abs(dk)
            return -log(estimate)

    def _escapetime(self, ppm):
        gradient = [
            ' '.join(map(str, map(round, rgba[:3])))
            for rgba in 255 * self.cm(
                pow(np.linspace(0, 1, self.lim), self.cmp)
            )
        ] + ['0 0 0']

        points = self._pointGenerator()
        for i in range(self.px):
            ppm.write('\n')
            for j in range(self.px):
                count = self._escapetimeCount(next(points))
                ppm.write(gradient[count] + '  ')

    def _dem(self, ppm):
        points = self._pointGenerator()
        values = np.empty((self.px, self.px), dtype=float)
        for j in range(self.px):
            for i in range(self.px):
                values[i][j] = self._demCount(next(points))

        m, M = values.min(), values.max()
        values[values == 0] = M
        norm = PowerNorm(self.cmp, m, M)
        gradient = self.cm(norm(values))
        for j in range(self.px):
            ppm.write('\n')
            for i in range(self.px):
                rgb = map(round, 255 * gradient[i][j][:3])
                ppm.write(' '.join(map(str, rgb)) + '  ')

    def draw(self, ppm):
        ppm.writelines(
            ['P3\n', f'{self.px} {self.px}\n', '255\n']
        )
        FractalImagePPM.__dict__['_' + self.alg](self, ppm)


class MandelbrotImagePPM(FractalImagePPM):
    
    def __init__(
        self, center, radius,
        alg, px, iterlim,
        cmap, cmp, cmo
    ):
        super().__init__(
            center, radius or 2.2, 2,
            alg, px, iterlim,
            cmap, cmp, cmo
        )

    def _iter(self, c, ck):
        return ck * ck + c

    def _deriv(self, ck, dk):
        return 2 * ck * dk + 1


class JuliaImagePPM(FractalImagePPM):
    
    def __init__(
        self, poly, center, radius,
        alg, px, iterlim,
        cmap, cmp, cmo
    ):  
        n = len(poly) - 1
        an = abs(poly[0])
        C = sum(map(abs, poly)) - an
        eps = max(1, 2 * C / 2, pow(2 * L / an, 1 / (n-1)))

        self.poly = poly
        self.derivative = [
            (n - i) * an for (i, an) in enumerate(poly[:-1])
        ]

        super().__init__(
            center, radius, eps,
            alg, px, iterlim,
            cmap, cmp, cmo
        )
        
    @staticmethod
    def _horner(poly, z):
        return reduce(lambda x, y: z * x + y, poly)
        
    def _iter(self, z, zk):
        return self._horner(self.poly, zk)

    def _deriv(self, zk, dk):
        return self._horner(self.derivative, zk) * dk


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
        '-fn', metavar='file name',
        type=str, default='fractal',
        help='file name'
    )
    vals = args.parse_args()
    
    ppm_file = vals.fn + '.ppm'
    open(ppm_file, 'a').close()
    with open(ppm_file, 'w', encoding='utf-8') as ppm:
        if vals.poly == 'mandelbrot':
            image = MandelbrotImagePPM(
                vals.c, vals.r,
                vals.alg, vals.px, vals.it,
                vals.cm, vals.cmp, vals.cmo
            )
        else:
            poly = list(map(complex, vals.poly.split()))
            image = JuliaImagePPM(
                poly, vals.c, vals.r,
                vals.alg, vals.px, vals.it,
                vals.cm, vals.cmp, vals.cmo
            )
        image.draw(ppm)
