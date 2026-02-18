from PIL import Image
import sys

def bmp_to_craw(bmp_path, c_varname, width=320, height=200):
    # Ouvrir l'image
    im = Image.open(bmp_path)
    
    # Vérifier mode
    if im.mode != 'P':
        raise ValueError("L'image doit être en palette (8-bit)")
    
    # Redimensionner si nécessaire
    if im.size != (width, height):
        im = im.resize((width, height), Image.NEAREST)

    # Palette (PIL retourne 768 octets 0-255)
    pal = im.getpalette()[:256*3]  # R,G,B

    # Pixels
    pixels = list(im.getdata())

    # Génération C
    c_code = f"// Generated from {bmp_path}\n"
    c_code += f"unsigned char {c_varname}_palette[256*3] = {{\n"
    
    for i in range(256):
        r,g,b = pal[i*3:i*3+3]
        c_code += f"0x{r:02X}, 0x{g:02X}, 0x{b:02X}, "
        if (i+1) % 8 == 0:
            c_code += "\n"
    c_code += "};\n\n"

    c_code += f"unsigned char {c_varname}_pixels[{width*height}] = {{\n"
    for i, px in enumerate(pixels):
        c_code += f"{px}, "
       # if (i+1) % 16 == 0:
           # c_code += "\n"
    c_code += "};\n"

    return c_code

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python bmp_to_craw.py image.bmp varname")
        sys.exit(1)
    
    bmp_file = sys.argv[1]
    varname = sys.argv[2]

    c_output = bmp_to_craw(bmp_file, varname)

    out_file = varname + ".h"
    with open(out_file, "w") as f:
        f.write(c_output)

    print(f"Generated {out_file} (palette + pixels)")
