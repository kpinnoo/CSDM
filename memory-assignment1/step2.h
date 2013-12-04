//----------------------------------------------------------
// Step2.h
// Authors: Michiel D'Haene and Peter Bertels
//----------------------------------------------------------
// Cavity detector - step 2
//----------------------------------------------------------

#define VERSION "step 2"

void cavityDetection (byte in[1280][1280], byte out[1280][1280]) {
	byte gtemp[1280][1280], gauss[1280][1280];
	byte edge[1280][1280], accuy[1280][1278];
	int k, l, x, y;

	// Gaussian blur: horizontal filter
	for (x = 0; x < 1280; x++) {
		for (y = 0; y < 1280; y++) {
			if(x==0 || x==1279) {
				gtemp[x][y] = in[x][y];
				mem(gtemp); mem(in);
			}
		}
	}
	for (x = 1; x < 1279; x++) {
		for (y = 0; y < 1280; y++) {
			accuy[x][y] = filter(-1, in[x-1][y]);
			mem(accuy); mem(in);
			for (k = 0; k <= 1; k++) {
				gtemp[x][y] = gtemp[x][y] + filter(k, in[x+k][y]);
				mem(gtemp); mem(gtemp); mem(in);
			}
			gauss[x][y] = accuy[x][y-1];
			mem(gauss); mem(accuy);
		}
	}

	// Gaussian blur: vertical filter
	for (y = 0; y < 1280; y++) {
		for (x = 0; x < 1280; x++) {
			if(y==0 || y==1279) {
				gauss[x][y] = gtemp[x][y];
				mem(gauss); mem(gtemp);
			}
		}
	}
	for (y = 1; y < 1279; y++) {
		for (x = 0; x < 1280; x++) {
			//gauss[x][y] = filter(-1, gtemp[x][y-1])+filter(0, gtemp[x][y])+filter(1, gtemp[x][y+1]);
			//mem(gauss); mem(gtemp); mem(gtemp); mem(gtemp);
			gauss[x][y] = filter(-1,gtemp[x][y-1]);
			mem(gtemp); mem(gtemp);
			for (k = 0; k <= 1; k++) {
				gauss[x][y] = gauss[x][y] + filter(k, gtemp[x][y+k]);
				mem(gauss); mem(gauss); mem(gtemp);
			}
		}
	}

	// Edge detection
	for (x = 0; x < 1280; x++) {
		for (y = 0; y < 1280; y++) {
			if(x==0 || y==0 || x==1279 || y==1279) {
				edge[x][y] = gauss[x][y];
				mem(edge); mem(gauss);
			}
		}
	}
	for (x = 1; x < 1279; x++) {
		for (y = 1; y < 1279; y++) {
			edge[x][y] = maxDiff(gauss[x][y], gauss[x-1][y-1], edge[x][y]);
			mem(edge); mem(gauss); mem(gauss); mem(edge);
			for (k = -1; k <= 1; k++) {
				for (l = -1; l <= 1; l++) {
					if (k != 0 || l != 0) {
						if(k!=-1 && l!=-1) {
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
			if(x==0 || x==1 || y==0 || y==1 || x==1278 || x==1279 || y==1278 || y==1279) {
				out[x][y] = 255;
				mem(out);
			}
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