from PIL import Image
import sys

def bmp_to_vga_palette(bmp_path, out_path=None):
    img = Image.open(bmp_path)

    if img.mode != "P":
        raise ValueError("Le BMP doit être en mode palette (8 bits indexé)")

    palette = img.getpalette()  # liste de 768 valeurs (256 * RGB)

    if palette is None or len(palette) < 768:
        raise ValueError("Palette invalide")

    # Conversion 0-255 → 0-63
    vga = [c // 4 for c in palette[:768]]

    # Formatage C
    lines = []
    for i in range(0, 768, 12):
        chunk = vga[i:i+12]
        line = ", ".join(f"0x{v:02x}" for v in chunk)
        lines.append("    " + line)

    c_code = (
        "// Palette VGA (256 couleurs)\n"
        "unsigned char palette[256*3] = {\n"
        + ",\n".join(lines) +
        "\n};\n"
    )

    if out_path:
        with open(out_path, "w") as f:
            f.write(c_code)
    else:
        print(c_code)


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python bmp2vga.py image.bmp [out.c]")
    else:
        bmp_to_vga_palette(
            sys.argv[1],
            sys.argv[2] if len(sys.argv) > 2 else None
        )
