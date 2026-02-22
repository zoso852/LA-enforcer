import os

INPUT_DIR = "."  # dossier courant
OUTPUT_FILE = "intro.dat"

bmp_files = sorted(f for f in os.listdir(INPUT_DIR) if f.lower().endswith(".bmp"))

with open(OUTPUT_FILE, "wb") as out_file:
    for bmp_name in bmp_files:
        bmp_path = os.path.join(INPUT_DIR, bmp_name)
        print(f"Processing {bmp_name}...")

        with open(bmp_path, "rb") as f:
            data = f.read()

            # Palette BMP = 1024 octets après header
            raw_palette = data[54:54+1024]
            palette = bytearray()
            for i in range(256):
                palette.extend(raw_palette[i*4:i*4+3])  # garder juste RGB
            out_file.write(palette)  # 768 octets

            # Pixels : 320*200 = 64000 octets juste après le header + palette
            pixel_data = data[54+1024:54+1024+320*200]
            out_file.write(pixel_data)

print(f"Done! {len(bmp_files)} images + palettes concaténées dans {OUTPUT_FILE}.")
