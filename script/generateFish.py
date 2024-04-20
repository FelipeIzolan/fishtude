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

NEMO = [
    (3,3),(4,3),(5,3),
    (3,5),(4,5),(5,5)
]

HALF = [
    (3,1),(3,3),(3,4),
    (4,2),(4,3),(4,4),
    (5,1),(5,3),(5,4),
]

PATTERN = [
    "flower",
    "nemo",
    "tint",
    "half"
]

# --------------------------------------------------------------------- #

def rgb2hsv(rgb):
    return colorsys.rgb_to_hsv(rgb[0] / 255.0, rgb[1] / 255.0, rgb[2] / 255.0)

def hsv2rgb(hsv):
    t = colorsys.hsv_to_rgb(hsv[0], hsv[1], hsv[2])
    return (round(t[0] * 255), round(t[1] * 255), round(t[2] * 255))

# --------------------------------------------------------------------- #

def _rgb_r_brightness(rgb, v):
    hsv = list(rgb2hsv(rgb))
    hsv[2] = max(hsv[2] - (v / 100), 0)
    
    return hsv2rgb(hsv)

def _rgb_m_sv(rgb):
    hsv = list(rgb2hsv(rgb))
    hsv[1] = random.random()
    hsv[2] = random.random()

    return hsv2rgb(hsv)

# --------------------------------------------------------------------- #

def _fish_base(fish, rgb):
    color = rgb if random.random() > 0.3 else _rgb_m_sv(rgb)
    shadow = _rgb_r_brightness(rgb, random.randrange(2, 16))
    
    fill_chance = random.random()

    for (y, x) in TOP:
        fish[y][x] = color

    for (y, x) in BOTTOM:
        fish[y][x] = shadow if fill_chance > 0.1 else color

def _fish_tint(fish, min=1, max=5, is_nemo=False):
    c = fish[3][4]
    for _ in range(random.randrange(min, max)):
        p = random.choice(AREA)
        
        if is_nemo:
            while p in NEMO:
                p = random.choice(AREA)
        
        fish[p[0]][p[1]] = _rgb_m_sv(c) 

def _fish_nemo(fish):
    for (y, x) in NEMO:
        fish[y][x] = NW if y != 5 else NWS

    _fish_tint(fish, 0, 4, True)

def _fish_flower(fish):
    for _ in range(random.randrange(1,3)):
        (y,x) = random.choice(AREA)
        c = random.choice(PALLETE)
        cm = _rgb_m_sv(c)

        fish[y][x] = c
        fish[y+1][x] = cm if (y+1,x) in AREA else fish[y+1][x]
        fish[y-1][x] = cm if (y-1,x) in AREA else fish[y-1][x]
        fish[y][x+1] = cm if (y,x+1) in AREA else fish[y][x+1]
        fish[y][x-1] = cm if (y,x-1) in AREA else fish[y][x-1]

def _fish_half(fish):
    c = _rgb_m_sv(fish[3][4])
    cs = _rgb_r_brightness(c, random.randrange(4,12))

    for i in range(len(HALF)):
        (y, x) = HALF[i]
        fish[y][x] = c if i % 2 == 0 else cs


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
    fish = FISH.copy()
    rgb = random.choice(PALLETE)
    pattern = random.choice(PATTERN)

    _fish_base(fish, rgb)

    match pattern:
        case "flower": _fish_flower(fish)
        case "nemo": _fish_nemo(fish)
        case "tint": _fish_tint(fish)
        case "half": _fish_half(fish)

    for j in range(8):
        out[j] += fish[j]


out = Image.fromarray(np.array(out, dtype=np.uint8))
out.save("./out.bmp")
