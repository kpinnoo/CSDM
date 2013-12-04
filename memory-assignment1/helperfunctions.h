//----------------------------------------------------------
// Helperfunctions.h
// Authors: Michiel D'Haene and Peter Bertels
//----------------------------------------------------------
// Cavity detector - helperfunctions
//----------------------------------------------------------

#include <stdio.h>
#include <math.h>
#define byte   unsigned char

//----------------------------------------------------------

#ifdef COUNT_MEMORY_ACCESSES
	#define combine(x,y)   x ## y
	#define mem(a)    combine(a,_counter)++;
#else
    #define mem(a)
#endif

//----------------------------------------------------------

byte maxDiff (byte buur, byte centrum, byte edge) {
	int diff = buur - centrum;
	diff = diff < 0 ? -diff : diff;
	if (diff > edge) return diff;
	else return edge;
}

//----------------------------------------------------------

byte filter (int position, byte pixel) {
	byte result = 0;
	switch (position) {
	case -1: result = 2*pixel/7; break;
	case 0: result = 3*pixel/7; break;
	case 1: result = 2*pixel/7; break;
	default: result = 0; break;
	}
	return result;
}

//----------------------------------------------------------

void readBMP (byte image[1280][1280], char *file) {
	int i, x, y;
	FILE *bw;
	fopen_s(&bw, file, "rb");
	for (i = 0; i < 54 + 256*4; i++) fgetc (bw);
	for (y = 1279; y >= 0; y--) {
		for (x = 0; x < 1280; x ++) {
			image[x][y] = (byte) fgetc (bw);
		}
	}
	fclose (bw);
}

//----------------------------------------------------------

void writeBMP (byte image[1280][1280], char *file) {
	int i, x, y;
	FILE *bw;
	fopen_s(&bw, file, "wb");
	fprintf (bw, "%c%c%c%c%c%c%c%c", 66, 77, 154, 144, 1, 0, 0, 0);
	fprintf (bw, "%c%c%c%c%c%c%c%c", 0, 0, 54, 4, 0, 0, 40, 0);
	fprintf (bw, "%c%c%c%c%c%c%c%c", 0, 0, 0, 5, 0, 0, 0, 5);
	fprintf (bw, "%c%c%c%c%c%c%c%c", 0, 0, 1, 0, 8, 0, 0, 0);
	fprintf (bw, "%c%c%c%c%c%c%c%c", 0, 0, 0, 144, 1, 0, 35, 11);
	fprintf (bw, "%c%c%c%c%c%c%c%c", 0, 0, 35, 11, 0, 0, 0, 1);
	fprintf (bw, "%c%c%c%c%c%c", 0, 0, 0, 1, 0, 0);
	for (i = 0; i < 256; i++) fprintf (bw, "%c%c%c%c", i, i, i, 0);
	for (y = 1279; y >= 0; y--) {
		for (x = 0; x < 1280; x ++) {
			fprintf (bw, "%c", image[x][y]);
		}
	}
	fclose (bw);
}

//----------------------------------------------------------

void diffBMP (char *file1, char* file2, char *diffFile) {
	int x, y;
	bool identical = true;
	byte image1[1280][1280], image2[1280][1280], diff[1280][1280];

	readBMP (image1, file1);
	readBMP (image2, file2);

	for (x = 0; x < 1280; x++) {
		for (y = 0; y < 1280; y++) {
			diff[x][y] = (byte) sqrt((double) 256* (double) fabs(image1[x][y] - (double) image2[x][y]));
			if (image1[x][y] != image2[x][y]) identical = false;
		}
	}

	if (identical) printf ("Diff: %s and %s are identical\n", file1, file2);
	else printf ("Diff: %s is wrong\n", file1);

	writeBMP (diff, diffFile);
}

//----------------------------------------------------------
