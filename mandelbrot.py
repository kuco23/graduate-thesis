import sys
import os

ITERCOUNT = 40
PIXELS = 1000
BASE_GRADIENT = (255, 255, 255)
PPM_HEADER = ['P3\n', f'{PIXELS} {PIXELS}\n', '255\n']

open('mandelbrot.ppm', 'a').close()
file = open('mandelbrot.ppm', 'w', encoding='ascii')

for h in PPM_HEADER:
    file.write(h)

mbFun = lambda z, c: z * z + c

def pointConvergance(point):
    z, count = complex(0, 0), 0
    while count < ITERCOUNT and abs(z) <= 2:
        z = mbFun(z, point)
        count += 1
    return count

def setPoint(i, j):
    x = 2.4 * i / PIXELS - 1.8
    y = 2.4 * j / PIXELS - 1.2
    return complex(x, y)

for i in range(PIXELS):
    file.write('\n')
    for j in range(PIXELS):
        point = setPoint(i, j)
        count = pointConvergance(point)
        base = (255 * count) // ITERCOUNT if count < ITERCOUNT else 0
        r, g, b = map(lambda c: (c / 255) * base, BASE_GRADIENT)
        rgb = f'{r} {g} {b}  '
        file.write(rgb)

file.flush()
file.close()
