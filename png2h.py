#!/usr/bin/env python
# convert 16x16 indexed .png file with max 16 colors to header file data for tile

from PIL import Image
import sys

DIMENSIONS = (16, 16)
MAXCOLORS = 16

if len(sys.argv) != 3:
    sys.stderr.write('Syntax: %s <pngfile> <name>\n' % sys.argv[0])
    sys.exit(1)

name = sys.argv[2]

try:
    img = Image.open(sys.argv[1])
except:
    sys.stderr.write('Error reading %s\n' % sys.argv[1])
    sys.exit(1)

if img.size != DIMENSIONS:
    sys.stderr.write('Warning: %s has dimensions %dx%d. Are you sure this is correct?\n'
                         % (sys.argv[1], img.size[0], img.size[1]))

if img.size[0] % 2 or img.size[1] % 2 or img.mode != 'P' or len(img.palette.palette) > MAXCOLORS * 3:
    sys.stderr.write('%s is not in a valid format\n' % sys.argv[1])
    sys.exit(1)

# engine expects row major format
img = img.transpose(Image.TRANSPOSE)
pixels = img.tobytes()
elems = img.size[0] / 2

alpha = True if img.palette.mode == 'RGBA' else False

sys.stdout.write('static const unsigned char tile_%s[] PROGMEM = {' % name)
ofs = 0
for pair in [pixels[i:i+2] for i in range(0, len(pixels), 2)]:
    b = ord(pair[0]) + ord(pair[1]) * 16
    if not alpha:
        b += 0x11
    sys.stdout.write('%s0x%02x,' % ('\n\t' if ofs % elems == 0 else ' ', b))
    ofs += 1

sys.stdout.write('\n};\n\n')

c = 0 if alpha else 1
pal = img.palette.tobytes()
for rgb in [pal[i:i+3] for i in range(0, len(pal), 3)]:
    r = ord(rgb[0]) >> 3
    g = ord(rgb[1]) >> 2
    b = ord(rgb[2]) >> 3
    color = (r << 11) + (g << 5) + b
    if not alpha or c > 0:
        sys.stdout.write('#define %s_COLOR%d\t 0x%04x\n' % (name.upper(), c, color))
    c += 1
