#include <stdio.h>
#include <stdlib.h>

// Simple export of XNU font from header into binary file
// by akasaka, 2018


// Get the font from:
// https://opensource.apple.com/source/xnu/xnu-3789.70.16/osfmk/console/iso_font.c.auto.html

#include "iso_font.h"

#include "psf.h"
#include "consts_actual.h"

// Apple's iso_font.h is horizontally reversed -- so we need to reverse all the bytes in there while writing the font


static unsigned char lookup[16] = {
0x0, 0x8, 0x4, 0xc, 0x2, 0xa, 0x6, 0xe,
0x1, 0x9, 0x5, 0xd, 0x3, 0xb, 0x7, 0xf, };

unsigned char reverse(unsigned char n) {
   // Reverse the top and bottom nibble then swap them.
   return (lookup[n&0b1111] << 4) | lookup[n>>4];
}


void main(void) {
	struct psf2_header * head = malloc(sizeof(struct psf2_header));
	
	head->magic[0] = PSF2_MAGIC0;
	head->magic[1] = PSF2_MAGIC1;
	head->magic[2] = PSF2_MAGIC2;
	head->magic[3] = PSF2_MAGIC3;
	
	head->headersize = sizeof(struct psf2_header);
	head->length = ISO_CHAR_MAX;
	head->height = ISO_CHAR_HEIGHT;
	head->width = ISO_CHAR_WIDTH;
	head->charsize = head->height * ((head->width + 7) / 8);
	
	FILE * file = fopen(FILENAME, "wb");
	if(file == NULL) {
		printf("Error opening output file.\n");
		exit(1);
	}
	
	fwrite(head, sizeof(struct psf2_header), 1, file);
	
	for(unsigned char i = 0; i < ISO_CHAR_MAX; i++) {
		printf("Writing character #%i\n", i);
		for(unsigned char x = 0; x < ISO_CHAR_HEIGHT; x++) {
			unsigned char original = iso_font[ i*ISO_CHAR_HEIGHT + x ];
			unsigned char rev = reverse(original);
			printf("%i -> %i\n",original,rev);
			fwrite(&rev, 1, 1, file);
		}
	}
	
	
	fclose(file);
	printf("Success.\n");
	exit(0);
}
