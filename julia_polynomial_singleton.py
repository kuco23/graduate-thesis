from functools import reduce
from os import mkdir
from os.path import isdir

def drawJuliaPolynomialPPM_escapetime(
    coefs, dims, range_re, range_im, color, file
):
    iterlim = 50
    L = 1.0001
    
    n = len(coefs) - 1
    an = abs(coefs[0])
    C = sum(map(abs, coefs)) - an
    eps = max(1, 2 * C / 2, pow(2 * L / an, 1 / (n-1)))

    (ReS, ReT), (ImS, ImT) = range_re, range_im

    def horner(z):
        return reduce(lambda x, y: z * x + y, coefs)

    def converganceIterations(z):
        count = 0
        while count < iterlim and abs(z) <= eps:
            z = horner(z)
            count += 1
        return count

    def converganceRGB(count):
        if count == iterlim: return (0,0,0)
        base = (255 * count) // iterlim
        return tuple(map(
            lambda c: int(c / 255 * base),
            color
        ))

    def coordsToPoint(i, j):
        x = i / dims * (ReT - ReS) + ReS
        y = j / dims * (ImT - ImS) + ImS
        return complex(x, y)

    ppmHeader = ('P3\n', f'{dims} {dims}\n', '255\n')
    for h in ppmHeader: file.write(h)

    for j in range(dims):
        file.write('\n')
        for i in range(dims):
            point = coordsToPoint(i, j)
            count = converganceIterations(point)
            file.write('{0} {1} {2}  '.format(
                *converganceRGB(count)
            ))


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
        '-re', metavar = 'real axis range',
        type = int, nargs = 2,
        default = [-2, 2]
    )
    args.add_argument(
        '-im', metavar = 'imaginary axis range',
        type = int, nargs = 2,
        default = [-2, 2]
    )
    args.add_argument(
        '-nm', metavar = 'file name',
        type = str, default = 'julia'
    )
    args.add_argument(
        '-rgb', metavar = 'image color',
        type = int, nargs = 3,
        default = [255, 0, 0]
    )
    vals = args.parse_args()
    coefs = list(map(complex, vals.coefs.split()))
    
    if not isdir('images'): mkdir('images')
    filename = f'images/{vals.nm}.ppm'
    open(filename, 'a').close()
    with open(filename, 'w', encoding='ascii') as file:
        drawJuliaPolynomialPPM_escapetime(
            coefs, vals.d, vals.re, vals.im,
            vals.rgb, file
        )
