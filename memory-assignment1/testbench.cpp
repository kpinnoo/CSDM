//----------------------------------------------------------
// Testbench.cpp
// Authors: Michiel D'Haene and Peter Bertels
//----------------------------------------------------------
// Cavity detector - testbench
//----------------------------------------------------------

#include <time.h>
#include <stdio.h>

//----------------------------------------------------------

#define NUMBER_OF_TEST_RUNS   5
//#define COUNT_MEMORY_ACCESSES
int in_counter, gtemp_counter, gauss_counter, edge_counter, out_counter;
int accux_counter, accuy_counter, buffer_counter, memory_counter;

//----------------------------------------------------------

#include "helperfunctions.h"
#include "original.h"

//----------------------------------------------------------

int main (void) {
	clock_t begin, einde;

	byte in[1280][1280], out[1280][1280];
	readBMP (in, "ct-scan.bmp");

	in_counter = 0; gtemp_counter = 0; gauss_counter = 0; edge_counter = 0; out_counter = 0;
	accux_counter = 0; accuy_counter = 0; buffer_counter = 0; memory_counter = 0;

	printf ("Version: %s\n", VERSION); 
	printf ("----------------------------------------------------\n");

#ifdef COUNT_MEMORY_ACCESSES
	cavityDetection (in, out);
	if (in_counter != 0) printf ("in:       %7d accesses\n", in_counter);
	if (gtemp_counter != 0) printf ("gtemp:    %7d accesses\n", gtemp_counter);
	if (gauss_counter != 0) printf ("gauss:    %7d accesses\n", gauss_counter);
	if (edge_counter != 0) printf ("edge:     %7d accesses\n", edge_counter);
	if (out_counter != 0) printf ("out:      %7d accesses\n", out_counter);
	if (accux_counter != 0) printf ("accux:    %7d accesses\n", accux_counter);
	if (accuy_counter != 0) printf ("accuy:    %7d accesses\n", accuy_counter);
	if (memory_counter != 0) printf ("memory: %7d accesses\n", memory_counter);
	printf ("----------------------------------------------------\n");
	printf ("Total:   %7d accesses\n", in_counter + gtemp_counter + gauss_counter + edge_counter + out_counter + accux_counter + accuy_counter + memory_counter);
	if (buffer_counter != 0) printf ("buffer:   %7d accesses\n", buffer_counter);
#else
	begin = clock();
	for (int i = 0; i < NUMBER_OF_TEST_RUNS; i++) cavityDetection (in, out);
	einde = clock();
	printf ("Execution time: %.1f ms\n", 1000 * (double) (einde - begin) / (double) CLOCKS_PER_SEC / (double) NUMBER_OF_TEST_RUNS);
#endif
	printf ("----------------------------------------------------\n");
	writeBMP (out, "output.bmp");
	diffBMP ("output.bmp", "output-correct.bmp", "output-diff.bmp");
	printf ("\n");

	return 0;
}

//----------------------------------------------------------