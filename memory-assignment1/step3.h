//----------------------------------------------------------
// Step3.h
// Authors: Michiel D'Haene and Peter Bertels
//----------------------------------------------------------
// Cavity detector - step 3
//----------------------------------------------------------

#define VERSION "step 3"

void cavityDetection (byte in[1280][1280], byte out[1280][1280]) {
	byte gtemp[1280][1280], gauss[1280][1280];
	byte edge[1280][1280], accux[1278][1280], accuy[1280][1278];
	int k, l, x, y;

	// Gaussian blur: horizontal filter
	for (x = 0; x < 1280; x++) {
		for (y = 0; y < 1280; y++) {
			if (x == 0 || x == 1279) {
				gtemp[x][y] = in[x][y];
				mem(gtemp); mem(in);
			} else {
				accux[x-1][y] = filter(-1, in[x-1][y]);
				mem(accux); mem(in);
				for (k = 0; k <= 1; k++) {
					accux[x-1][y] = accux[x-1][y] + filter(k, in[x+k][y]);
					mem(accux); mem(accux); mem(in);
				}
				gtemp[x][y] = accux[x-1][y];
				mem(gtemp); mem(accux);
			}
			if(y>0) {
				if (y == 1) {
					gauss[x][0] = gtemp[x][0];
					mem(gauss); mem(gtemp);
				} else if(y==1279) {
					accuy[x][y-2] = filter(-1, gtemp[x][y-2]);
					mem(accuy); mem(gtemp);
					for (k = 0; k <= 1; k++) {
						accuy[x][y-2] = accuy[x][y-2] + filter(k, gtemp[x][y+k-1]);
						mem(accuy); mem(accuy); mem(gtemp);
					}
					gauss[x][y-1] = accuy[x][y-2];
					mem(gauss); mem(accuy);
				} else {
					accuy[x][y-1] = filter(-1, gtemp[x][y-2]);
					mem(accuy); mem(gtemp);
					for (k = 0; k <= 1; k++) {
						accuy[x][y-1] = accuy[x][y-1] + filter(k, gtemp[x][y+k-1]);
						mem(accuy); mem(accuy); mem(gtemp);
					}
					gauss[x][y-1] = accuy[x][y-1];
					mem(gauss); mem(accuy);
				}
			}
			if (y==1279) {
				gauss[x][y] = gtemp[x][y];
				mem(gauss); mem(gtemp);
			}
		}
	}

	// Gaussian blur: vertical filter

	// Edge detection
	for (x = 0; x < 1280; x++) {
		for (y = 0; y < 1280; y++) {
			if (x == 0 || x == 1279 || y == 0 || y == 1279) {
				edge[x][y] = gauss[x][y];
				mem(edge); mem(gauss);
			} else {
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
	}

	// Cavity detection
	for (x = 0; x < 1280; x++) {
		for (y = 0; y < 1280; y++) {
			if (x < 2 || x >= 1278 || y < 2 || y >= 1278) {
				out[x][y] = 255;
				mem(out);
			} else {
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
	}
	return;
}

//----------------------------------------------------------