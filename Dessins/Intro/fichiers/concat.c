#include <stdio.h>
#include <stdlib.h>

// Ouvre les bmp, copie la palette (convertie pour vga) et l'image (lignes inversées pour vga) à la suite dans intro.dat
// !!! Il faut que les palettes aient toutes les 256 couleurs

FILE *write;

void concat(char input_file[])
{
	unsigned int i,j;
	unsigned char header[138];
	unsigned char pal[3];
	unsigned char pal_flip[3];
	unsigned char *image = malloc (64000);
	unsigned char *image_flip = malloc(64000);
	
	FILE *read = fopen(input_file, "rb");
	
	
	fread (header, 1, 138, read);		// HEADER
	
	for (i=0;i<256;i++)				// PALETTE
		{
		fread(pal, 1, 3, read);
		pal_flip[0] = pal[2]>>2; // R
		pal_flip[1] = pal[1]>>2; // G  BMP, couleurs codées de 0 à 255, VGA de 0 à 63
		pal_flip[2] = pal[0]>>2; // B
		fwrite (pal_flip, 1, 3, write);
		fread(pal,1,1,read);
		}
		
	fread(image,1 ,64000, read);		// IMAGE
	
	for (i=0;i<200;i++)
		{
		for (j=0;j<320;j++)
			{
			image_flip[j+(320*i)] = image[j+((199-i)*320)];
			}
		}

	fwrite(image_flip, 1, 64000, write);
	
	
	
	fclose(read);
    
    free(image);
	free(image_flip);
}

int main()
	{
	write = fopen ("intro.dat", "wb");
	concat("0.bmp");
	concat("1.bmp");
	concat("2.bmp");
	concat("3.bmp");
	concat("4.bmp");
	concat("5.bmp");
	fclose(write);

    return 0;
	}
