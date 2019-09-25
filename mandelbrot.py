ITERCOUNT = 40
EPS = 2

PIXELS = 600
RANGEX = (-2, 2)
RANGEY = (-2, 2)

BASE_GRADIENT = (255, 111, 204)

PPM_FILE = 'mandelbrot.ppm'
PPM_HEADER = ['P3\n', f'{PIXELS} {PIXELS}\n', '255\n']

open(PPM_FILE, 'a').close()
file = open(PPM_FILE, 'w', encoding='ascii')
for h in PPM_HEADER: file.write(h)

iterFun = lambda z, c: z * z + c

def pointConvergance(point):
    z, count = complex(0, 0), 0
    while count < ITERCOUNT and abs(z) <= EPS:
        z = iterFun(z, point)
        count += 1
    return count

def setPoint(i, j):
    (a, b), (c, d) = RANGEX, RANGEY
    x = i / PIXELS * (b - a) + a
    y = j / PIXELS * (d - c) + c
    return complex(x, y)

for j in range(PIXELS):
    file.write('\n')
    for i in range(PIXELS):
        point = setPoint(i, j)
        count = pointConvergance(point)
        base = (255 * count) // ITERCOUNT * (count < ITERCOUNT)
        r, g, b = map(lambda c: (c / 255) * base, BASE_GRADIENT)
        file.write(f'{r} {g} {b}  ')

file.flush()
file.close()
