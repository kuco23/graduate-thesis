import os
from pathlib import Path
import cv2

cdir = Path().cwd()
for file in cdir.iterdir():
    print(file)
    if file.suffix == '.ppm':
        image = cv2.imread(str(file))
        cv2.imwrite(str(file.with_suffix('.png')), image)
        os.unlink(file)