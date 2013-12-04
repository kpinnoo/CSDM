//----------------------------------------------------------
// Step1.h
// Authors: Michiel D'Haene and Peter Bertels
//----------------------------------------------------------
// Cavity detector - step 1
//----------------------------------------------------------

#define VERSION "step 1"

void cavityDetection (byte in[1280][1280], byte out[1280][1280]) {
	byte gtemp[1280][1280], gauss[1280][1280];
	byte edge[1280][1280], accux[1278][1280], accuy[1280][1278];
	int k, l, x, y;

	// Gaussian blur: horizontal filter
	for (x = 0; x < 1280; x++) {
		for (y = 0; y < 1280; y++) {
			gtemp[x][y] = in[x][y];
			mem(gtemp); mem(in);
		}
	}
	for (x = 1; x < 1279; x++) {
		for (y = 0; y < 1280; y++) {
			accux[x-1][y] = 0;
			mem(accux);
			for (k = -1; k <= 1; k++) {
				accux[x-1][y] = accux[x-1][y] + filter(k, in[x+k][y]);
				mem(accux); mem(accux); mem(in);
			}
			gtemp[x][y] = accux[x-1][y];
			mem(gtemp); mem(accux);
		}
	}

	// Gaussian blur: vertical filter
	for (y = 0; y < 1280; y++) {
		for (x = 0; x < 1280; x++) {
			gauss[x][y] = gtemp[x][y];
			mem(gauss); mem(gtemp);
		}
	}
	for (y = 1; y < 1279; y++) {
		for (x = 0; x < 1280; x++) {
			accuy[x][y-1] = 0;
			mem(accuy);
			for (k = -1; k <= 1; k++) {
				accuy[x][y-1] = accuy[x][y-1] + filter(k, gtemp[x][y+k]);
				mem(accuy); mem(accuy); mem(gtemp);
			}
			gauss[x][y] = accuy[x][y-1];
			mem(gauss); mem(accuy);
		}
	}

	// Edge detection
	for (x = 0; x < 1280; x++) {
		for (y = 0; y < 1280; y++) {
			edge[x][y] = gauss[x][y];
			mem(edge); mem(gauss);
		}
	}
	for (x = 1; x < 1279; x++) {
		for (y = 1; y < 1279; y++) {
			edge[x][y] = 0;
			mem(edge);
			for (k = -1; k <= 1; k++) {
				for (l = -1; l <= 1; l++) {
					if (k != 0 || l != 0) {
						edge[x][y] = maxDiff(gauss[x][y], gauss[x+k][y+l], edge[x][y]);
						mem(gauss); mem(gauss);
						mem(edge); mem(edge);
					}
				}
			}
		}
	}

	// Cavity detection
	for (x = 0; x < 1280; x++) {
		for (y = 0; y < 1280; y++) {
			out[x][y] = 255;
			mem(out);
		}
	}
	for (x = 2; x < 1278; x++) {
		for (y = 2; y < 1278; y++) {
			out[x][y] = 255;
			mem(out);
			for (k = -1; k <= 1; k++) {
				for (l = -1; l <= 1; l++) {
					if (k != 0 || l != 0) {
						mem(edge); mem(edge);
						if (edge[x][y] < edge[x+k][y+l]) {
							out[x][y] = 0;
							mem(out);
						}
					}
				}
			}
		}
	}

	return;
}

//----------------------------------------------------------