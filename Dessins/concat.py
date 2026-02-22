import os

INPUT_DIR = "./images"  # dossier avec les BMP
OUTPUT_FILE = "intro.dat"

bmp_files = sorted(f for f in os.listdir(INPUT_DIR) if f.lower().endswith(".bmp"))

with open(OUTPUT_FILE, "wb") as out_file:
    for bmp_name in bmp_files:
        bmp_path = os.path.join(INPUT_DIR, bmp_name)
        print(f"Processing {bmp_name}...")

        with open(bmp_path, "rb") as f:
            data = f.read()

            # Header BMP = 54 octets
            header = data[:54]
            pixel_data = data[54:]

            # Largeur et hauteur
            width = int.from_bytes(header[18:22], byteorder='little')
            height = int.from_bytes(header[22:26], byteorder='little')

            # Palette 256 couleurs * 4 octets = 1024 octets
            palette = data[54:54+1024]
            out_file.write(palette)  # écrire la palette avant l'image

            # Taille de la ligne avec padding (alignée sur 4 octets)
            row_size = ((width + 3) // 4) * 4

            # Copier l'image ligne par ligne, inversion pour top-down
            for y in range(height):
                start = (height - 1 - y) * row_size
                end = start + row_size
                out_file.write(pixel_data[start:end])

print(f"Done! {len(bmp_files)} images + palettes concaténées dans {OUTPUT_FILE}.")
