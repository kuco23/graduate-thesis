from pathlib import Path
from argparse import ArgumentParser
import cv2

args = ArgumentParser()
args.add_argument('name', metavar='video name', type=str)
vals = args.parse_args()

direct = Path().cwd() / 'images'

ppms = list(filter(
    lambda file: file.suffix == '.ppm',
    direct.iterdir()
))
ppms.sort(key = lambda ppm: int(ppm.stem.split('_')[-1]))
imgs = map(lambda ppm: cv2.imread(str(ppm)), ppms)

img_sample = cv2.imread(str(ppms[0]))
height, width, layers = img_sample.shape
fourc = cv2.VideoWriter_fourcc(*'DIVX')
video = cv2.VideoWriter(
    vals.name + '.avi',
    fourc,
    15, (width, height)
)

for img in imgs: video.write(img)
video.release()
