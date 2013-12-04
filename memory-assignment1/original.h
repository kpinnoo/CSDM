//----------------------------------------------------------
// Original.h
// Authors: Michiel D'Haene and Peter Bertels
//----------------------------------------------------------
// Cavity detector - original
//----------------------------------------------------------

#define VERSION "original"


void cavityDetection (byte in[1280][1280], byte out[1280][1280]) {
	byte gtemp[1280][1280], gauss[1280][1280];
	byte edge[1280][1280], accux, accuy;
	int k, l, x, y, diff;

	// Gaussian blur: horizontal filter
	for (x = 0; x < 1280; x++) {
		for (y = 0; y < 1280; y++) {
			gtemp[x][y] = in[x][y]; 
			mem(gtemp); mem(in);
		}
	}
	for (x = 1; x < 1279; x++) {
		for (y = 0; y < 1280; y++) {
			accux = 0;
			for (k = -1; k <= 1; k++) {
				accux += filter(k, in[x+k][y]);
				mem(in);
			}
			gtemp[x][y] = accux;
			mem(gtemp);
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
			accuy = 0;
			for (k = -1; k <= 1; k++) {
				accuy += filter(k, gtemp[x][y+k]);
				mem(gtemp);
			}
			gauss[x][y] = accuy;
			mem(gauss);
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
						diff = gauss[x+k][y+l] - gauss[x][y];
						diff = diff < 0 ? -diff : diff;
						edge[x][y] = diff > edge[x][y] ? diff : edge[x][y];
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