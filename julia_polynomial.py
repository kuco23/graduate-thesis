from functools import reduce
from os import mkdir
from os.path import isdir

class PolynomialJuliaImage:

    def __init__(
        self, koefs,
        dims, rangex, rangey,
        color
    ):
        self.coefs = coefs
        self.deg = len(coefs) - 1
        
        self.dims = dims
        self.rangex = rangex
        self.rangey = rangey
        self.color = color

        self._lim = 50
        self._lmbd = 1.00001

        an = abs(coefs[0])
        C = sum(map(abs, coefs)) - an
        lmbdeps = pow(2 * self._lmbd / an, 1 / (self.deg-1))
        self._eps = max(1, 2 * C / 2, lmbdeps)

    def _horner(self, z):
        return reduce(lambda x, y: z * x + y, self.coefs)

    def _converganceIterations(self, z):
        count = 0
        while count < self._lim and abs(z) <= self._eps:
            z = self._horner(z)
            count += 1
        return count

    def _converganceRGB(self, count):
        if count == self._lim: return (0,0,0)
        base = (255 * count) // self._lim
        return tuple(map(
            lambda c: int(c / 255 * base),
            self.color
        ))

    def _coordsToPoint(self, i, j):
        (a, b), (c, d) = self.rangex, self.rangey
        x = i / self.dims * (b - a) + a
        y = j / self.dims * (d - c) + c
        return complex(x, y)

    def _writeHeaderPPM(self, file):
        dim = self.dims
        ppmHeader = ('P3\n', f'{dim} {dim}\n', '255\n')
        for h in ppmHeader: file.write(h)

    def _drawPPM_escapetime(self, file):
        self._writeHeaderPPM(file)
        for j in range(self.dims):
            file.write('\n')
            for i in range(self.dims):
                point = self._coordsToPoint(i, j)
                count = self._converganceIterations(point)
                file.write('{0} {1} {2}  '.format(
                    *self._converganceRGB(count)
                ))

    def _drawPPM_backwardsiter(self, file):
        self._writeHeaderPPM(file)
        ...

    def drawPPM(self, file):
        self._drawPPM_escapetime(file)
                
if __name__ == '__main__':

    from argparse import ArgumentParser

    args = ArgumentParser()
    args.add_argument(
        'coefs', metavar = 'polynomial coefficients',
        type = str
    )
    args.add_argument(
        '-d', metavar = 'image dimensions',
        type = int, default = 800
    )
    args.add_argument(
        '-rx', metavar = 'range x',
        type = int, nargs = 2,
        default = [-2, 2]
    )
    args.add_argument(
        '-ry', metavar = 'range y',
        type = int, nargs = 2,
        default = [-2, 2]
    )
    args.add_argument(
        '-n', metavar = 'file name',
        type = str, default = 'julia'
    )
    args.add_argument(
        '-rgb', metavar = 'image color',
        type = int, nargs = 3,
        default = [255, 0, 0]
    )
    vals = args.parse_args()

    coefs = list(map(complex, vals.coefs.split()))
    image = PolynomialJuliaImage(
        coefs, vals.d,
        vals.rx, vals.ry,
        vals.rgb
    )

    if not isdir('images'): mkdir('images')
    filename = f'images/{vals.n}.ppm'
    open(filename, 'a').close()
    with open(filename, 'w', encoding='ascii') as file:
        image.drawPPM(file)
