DIMS = 1000
RANGEX = (-2, 2)
RANGEY = (-2, 2)
COLOR = (255, 0, 0)

PPM_FILE = 'julia.ppm'
PPM_HEADER = ('P3\n', f'{DIMS} {DIMS}\n', '255\n')

C = complex(-1.2, -0.2)
ITERLIM = 100
ITEREPS = max(abs(C), 2)
ITERFUN = lambda z: z ** 2 + C

def pointConvergance(point):
    count = 0
    while count < ITERLIM and abs(point) <= ITEREPS:
        point = ITERFUN(point)
        count += 1
    return count

def setPoint(i, j):
    (a, b), (c, d) = RANGEX, RANGEY
    x = i / DIMS * (b - a) + a
    y = j / DIMS * (d - c) + c
    return complex(x, y)

open(PPM_FILE, 'a').close()
file = open(PPM_FILE, 'w', encoding='ascii')
for h in PPM_HEADER: file.write(h)

for j in range(DIMS):
    file.write('\n')
    for i in range(DIMS):
        point = setPoint(i, j)
        count = pointConvergance(point)
        base = (255 * count) // ITERLIM * (count < ITERLIM)
        r, g, b = map(lambda c: (c / 255) * base, COLOR)
        file.write(f'{r} {g} {b}  ')

file.flush()
file.close()
