# batch_bmp_to_rle_final.py
# usage: python batch_bmp_to_rle_final.py input_folder output_folder

import os
import sys
import struct

def load_bmp_8bit(path):
    with open(path,"rb") as f:
        data = f.read()
    if len(data)<54: raise ValueError("not a valid bmp")
    bfType = struct.unpack_from("<H",data,0)[0]
    if bfType != 0x4D42: raise ValueError("not a BMP file")
    bfOffBits = struct.unpack_from("<I",data,10)[0]
    width = struct.unpack_from("<I",data,18)[0]
    height = struct.unpack_from("<I",data,22)[0]
    bpp = struct.unpack_from("<H",data,28)[0]
    if bpp != 8: raise ValueError("BMP must be 8-bit")
    row_size = (width + 3) & ~3
    pixel_data = data[bfOffBits : bfOffBits + row_size*height]
    pixels = [[0]*width for _ in range(height)]
    for y in range(height):
        src_off = (height-1-y)*row_size
        for x in range(width):
            byte = pixel_data[src_off+x]
            pixels[y][x] = byte if isinstance(byte,int) else ord(byte)
    return width,height,pixels

def bmp_to_rle_keep_empty_cols(input_bmp, output_txt):
    width, height, pixels = load_bmp_8bit(input_bmp)
    out = [width, height]  # largeur et hauteur originales

    for x in range(width):
        y = 0
        while y < height:
            while y < height and pixels[y][x] == 0:
                y += 1
            if y >= height:
                break
            y_start = y
            vals = []
            while y < height and pixels[y][x] != 0:
                vals.append(pixels[y][x])
                y += 1
            sub = 0
            total = len(vals)
            while sub < total:
                col = vals[sub]
                cnt = 1
                sub += 1
                while sub < total and vals[sub] == col:
                    cnt += 1
                    sub += 1
                global_y = y_start + (sub - cnt)
                out.append(global_y)
                out.append(cnt)
                out.append(col)
        # fin de colonne
        out.append(254)

    # supprimer tous les 254 a la fin avant d'ajouter 255
    while out and out[-1] == 254:
        out.pop()
    out.append(255)

    total_elements = len(out)
    with open(output_txt,"w") as f:
        f.write(str(total_elements) + "\n")
        f.write(",".join(str(x) for x in out))
    print("generated:", os.path.abspath(output_txt), "total elements =", total_elements)

def batch_convert(input_folder, output_folder):
    if not os.path.exists(output_folder):
        os.makedirs(output_folder)
    
    files = [f for f in os.listdir(input_folder) if f.lower().endswith(".bmp")]
    if not files:
        print("No BMP files found in", input_folder)
        return
    
    for f in files:
        input_path = os.path.join(input_folder,f)
        output_name = os.path.splitext(f)[0]+".txt"
        output_path = os.path.join(output_folder,output_name)
        print("Converting:",f,"->",output_name)
        bmp_to_rle_keep_empty_cols(input_path,output_path)
    print("Batch conversion done.")

if __name__=="__main__":
    if len(sys.argv)<3:
        print("usage: python batch_bmp_to_rle_final.py input_folder output_folder")
        sys.exit(1)
    input_folder=sys.argv[1]
    output_folder=sys.argv[2]
    batch_convert(input_folder,output_folder)
