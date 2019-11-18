from math import hypot
from hashlib import sha256
from functools import reduce
from pathlib import Path
from argparse import ArgumentParser

from julia_polynomial_singleton import (
    drawJuliaPolynomialPPM_escapetime
)

parser = ArgumentParser('map a name to fractal')
parser.add_argument('name', metavar='name', type=str)
vals = parser.parse_args()

cbin = Path().cwd() / 'nameimgs'
filename = vals.name + '.ppm'

hsh = sha256(vals.name.encode('utf-8'))
key = hsh.hexdigest()

coefs = list(map(
    lambda l: complex(*map(
        lambda c: int(c, 16), [l[:8], l[8:]]
    )),          
    [key[i * 16: (i+1) * 16] for i in range(4)]
))

norm = reduce(hypot, map(abs, coefs))
coefs = list(map(lambda z: 1.2 * z / norm, coefs))

with open(cbin / filename, 'w') as file:
    drawJuliaPolynomialPPM_escapetime(
        coefs, 1200, None, None, [255, 0, 0], file
    )
