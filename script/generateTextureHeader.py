import struct
import math
from io import SEEK_CUR, SEEK_SET

C_STATIC_ARRAY = "const uint8_t {}[{} * {} * 4] = {{{}}}"

def bitmapRawArray(name, width, height, bytes_per_pixel, row_size, data):
    raw = ""
    for y in range(height - 1, -1, -1): # <- top-to-bottom
        for x in range(width):
            offset = int((x*bytes_per_pixel) + (y*row_size))

            b = data[offset]
            g = data[offset+1]
            r = data[offset+2]
            a = data[offset+3]
           
            raw += "{}{},{},{},{}".format('' if (x == 0 and y == height - 1) else ',', a, b, g, r)

    return C_STATIC_ARRAY.format(name, width, height, raw)

def readBitmap(src, name):
    file = open(src, 'rb') # <- read-binary-mode
    
    if file.read(2).decode() != 'BM':
        raise Exception("{} not is a bitmap file.".format(src))

    file.seek(10, SEEK_SET)

    offset = struct.unpack("I",file.read(4))[0]

    file.seek(4, SEEK_CUR)

    width = struct.unpack("I", file.read(4))[0]
    height = struct.unpack("I", file.read(4))[0]

    file.seek(2, SEEK_CUR)
    
    bit_per_pixel = struct.unpack("H", file.read(2))[0]

    if bit_per_pixel != 32:
        raise Exception("{} not have alpha mask.".format(src))

    bytes_per_pixel = int(bit_per_pixel / 8)
    row_size = math.floor((bit_per_pixel * width + 31) / 32) * 4
    
    file.seek(offset, SEEK_SET)

    data = file.read(width * height * bytes_per_pixel)    

    file.close()

    return bitmapRawArray(name, width, height, bytes_per_pixel, row_size, data)

bitmap = dict(
        player = "../assets/player.bmp",
        fish = "../assets/fish.bmp",
        cloud = "../assets/cloud.bmp",
        passive = "../assets/passives.bmp",
        number = "../assets/number.bmp",
        background = "../assets/background.bmp",
        ui_x = "../assets/ui_x.bmp",
        ui_z = "../assets/ui_z.bmp",
        coin = "../assets/coin.bmp",
        fishing_frame = "../assets/fishing_frame.bmp",
        fishing_pointer = "../assets/fishing_pointer.bmp",
        passive_pointer = "../assets/passive_pointer.bmp"
    )

source = "#pragma once\n#include <stdint.h>\n"

for b in bitmap:
    raw = readBitmap(bitmap[b], b.upper() + "_PIXEL_ARRAY")
    source += "{};\n".format(raw)

out = open("./texture.h", "w+")
out.write(source)
out.close()
