from PIL import Image
import numpy as np
import colorsys
import random

G = (0,255,0)
B = (0,0,0)
E = (34,32,52)

NW = (255,255,255)
NWS = (203,219,252)

# DB32 PALLETE
# pallete used in fishtude
PALLETE = [
    (34,32,32),
    (69,40,60),
    (102,57,49),
    (143,86,59),
    (223,113,38),
    (217,160,102),
    (238,195,154),
    (251,242,54),
    (153,229,80),
    (106,190,48),
    (55,148,110),
    (75,105,47),
    (82,75,36),
    (50,60,57),
    (63,63,116),
    (48,96,130),
    (91,110,225),
    (99,155,255),
    (95,205,228),
    (203,219,252),
    (255,255,255),
    (155,173,183),
    (132,126,135),
    (105,106,106),
    (89,86,82),
    (118,66,138),
    (172,50,50),
    (217,87,99),
    (215,123,186),
    (143,151,41),
    (138,111,48)
]

FISH = [
   [G,G,G,G,G,G,G,G],
   [G,G,G,G,G,G,G,G],
   [G,B,G,B,B,B,B,G],
   [B,G,B,G,G,G,E,B],
   [G,B,G,G,G,G,G,B],
   [B,G,B,G,G,G,G,B],
   [G,B,G,B,B,B,B,G],
   [G,G,G,G,G,G,G,G]
]

AREA = [
    (3,1),(3,3),(3,4),(3,5),
    (4,2),(4,3),(4,4),(4,5),(4,6),
    (5,1),(5,3),(5,4),(5,5),(5,6)
]

TOP = [
    (3,1),(3,3),(3,4),(3,5),
    (4,2),(4,3),(4,4),(4,5),(4,6)
]

BOTTOM = [
    (5,1),(5,3),(5,4),(5,5),(5,6)
]

PATTERN = {
    'point': [],
    'flower': [],
    'half': [
        (3,1),(3,3),(3,4),
        (4,2),(4,3),(4,4),
        (5,1),(5,3),(5,4)
    ],
    'diagonal1': [
        (3,3),
             (4,4),
                 (5,5)
    ],
    'diagonal2': [
                  (3,5),
             (4,4),
        (5,3)
    ],
    'point4': [
        (3,3),(3,5),
        (5,3),(5,5)
    ],
    'nemo1': [
        (3,3),(3,5),
        (4,3),(4,5),
        (5,3),(5,5)
    ],
    'nemo2': [
        (3,4),
        (4,4),
        (5,4)
    ],
    'tiger1': [
        (3,3),(3,5),
        (4,3),(4,5)
    ],
    'tiger2': [
             (3,3),(3,5),
        (4,2),(4,4)
    ],
    'bandersnatch': [
              (3,4),
        (4,3),(4,4),(4,5),
        (5,3),      (5,5)
    ],
    'z': [
              (3,4),(3,5),
              (4,4),
        (5,3),(5,4)
    ],
    'x': [
        (3,3),    (3,5),
             (4,4),
        (5,3),    (5,5)
    ],
    'o': [
        (3,1),(3,3),(3,4),
        (4,2),      (4,4),
        (5,1),(5,3),(5,4)
    ],
    '<': [
             (3,4),
        (4,3),
             (5,4)
    ],
    '>': [
        (3,4),
             (4,5),
        (5,4)
    ]
}

# --------------------------------------------------------------------- #

def rgb2hsv(rgb):
    return colorsys.rgb_to_hsv(rgb[0] / 255.0, rgb[1] / 255.0, rgb[2] / 255.0)

def hsv2rgb(hsv):
    t = colorsys.hsv_to_rgb(hsv[0], hsv[1], hsv[2])
    return (t[0] * 255.0, t[1] * 255.0, t[2] * 255.0)

def _rgb_r_brightness(rgb, v):
    hsv = list(rgb2hsv(rgb))
    hsv[2] = max(hsv[2] - (v / 100), 0)
    
    return hsv2rgb(hsv)

def _rgb_m_sv(rgb):
    hsv = list(rgb2hsv(rgb))
    hsv[1] = random.random()
    hsv[2] = random.random()

    return hsv2rgb(hsv)

def _color(mix = False):
    color = _rgb_m_sv(random.choice(PALLETE)) if mix else random.choice(PALLETE)
    return (color, _rgb_r_brightness(color, random.randrange(4,16)) if random.random() < 0.7 else color)

# --------------------------------------------------------------------- #

def _fish_base():
    fish = FISH.copy()
    color, shadow =  _color()

    for (y, x) in TOP:
        fish[y][x] = color

    if random.random() < 0.9:
        for (y, x) in BOTTOM:
            fish[y][x] = shadow

    return fish

def _fish_pattern(fish):
    color, shadow = _color(True)
    key, value = random.choice(list(PATTERN.items()))

    if key.startswith('nemo') and random.random() < 0.75:
        color = (255,255,255)
        shadow = (203,219,252)

    if key == 'point':
        (y,x) = random.choice(AREA)
        fish[y][x] = _rgb_m_sv(random.choice(PALLETE))

    if key == 'flower':
        (y,x) = random.choice(AREA)
        center = random.choice(PALLETE)
        sides = _rgb_m_sv(center)

        fish[y][x] = center
        fish[y+1][x] = sides if (y+1,x) in AREA else fish[y+1][x]
        fish[y-1][x] = sides if (y-1,x) in AREA else fish[y-1][x]
        fish[y][x+1] = sides if (y,x+1) in AREA else fish[y][x+1]
        fish[y][x-1] = sides if (y,x-1) in AREA else fish[y][x-1]

    for (y, x) in value:
        fish[y][x] = shadow if ((y,x) in BOTTOM) else color

    return _fish_pattern(fish) if random.random() < 0.25 else fish

# --------------------------------------------------------------------- #

out = [
    [],
    [],
    [],
    [],
    [],
    [],
    [],
    []
]

for i in range(16):
    fish = _fish_base()
    fish = _fish_pattern(fish)

    for j in range(8):
        out[j] += fish[j]

out = Image.fromarray(np.array(out, dtype=np.uint8))
out.save("./out.bmp")
