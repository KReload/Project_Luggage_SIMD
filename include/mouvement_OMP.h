#include <omp.h>
#include <stdlib.h>
#include "nrdef.h"
#include "nrutil.h"

#define FILENAMESIZE 22
#define NIMAGES 300
#define VMIN 1
#define VMAX 254
#define N 2
#define CHUNK 16


void frameDifference(uint8** I0, uint8** I1,uint8** O, uint8** E, long nrl, long nrh, long ncl, long nch, uint8 theta);

void sigmaDelta(uint8** I0, uint8** I1, uint8** M0, uint8** M1, uint8** V0, uint8** V1, uint8** O1, uint8** E1, long nrl, long nrh, long ncl, long nch);
