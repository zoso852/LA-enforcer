# bmp_to_rle_with_wh_clean.py
# usage: python bmp_to_rle_with_wh_clean.py input.bmp output.txt
# Format:
#   <TOTAL_ELEMENTS>\n
#   width,height,
#   y_start,len,color,...,254,...,255

import sys, struct, os

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
    return width, height, pixels

def trim_columns(pixels,width,height):
    left = 0
    while left<width:
        if any(pixels[y][left]!=0 for y in range(height)): break
        left +=1
    right = width-1
    while right>=0:
        if any(pixels[y][right]!=0 for y in range(height)): break
        right -=1
    if left>right: return 0,0,[]
    new_width = right-left+1
    new_pixels = [row[left:right+1] for row in pixels]
    return new_width,height,new_pixels

def bmp_to_rle_with_wh(input_bmp,output_txt):
    width,height,pixels = load_bmp_8bit(input_bmp)
    new_width,new_height,pix = trim_columns(pixels,width,height)
    out = [new_width,new_height]
    for x in range(new_width):
        y=0
        while y<new_height:
            while y<new_height and pix[y][x]==0: y+=1
            if y>=new_height: break
            y_start=y
            vals=[]
            while y<new_height and pix[y][x]!=0:
                vals.append(pix[y][x])
                y+=1
            sub=0
            total=len(vals)
            while sub<total:
                col=vals[sub]
                cnt=1
                sub+=1
                while sub<total and vals[sub]==col:
                    cnt+=1
                    sub+=1
                global_y=y_start+(sub-cnt)
                out.append(global_y)
                out.append(cnt)
                out.append(col)
        out.append(254)
    if out and out[-1]==254: out.pop()
    out.append(255)
    total_elements=len(out)
    with open(output_txt,"w") as f:
        f.write(str(total_elements)+"\n")
        f.write(",".join(str(x) for x in out))
    print("generated:",os.path.abspath(output_txt))
    print("total elements =",total_elements)

if __name__=="__main__":
    if len(sys.argv)<3:
        print("usage: python bmp_to_rle_with_wh_clean.py input.bmp output.txt")
        sys.exit(1)
    bmp_to_rle_with_wh(sys.argv[1],sys.argv[2])
