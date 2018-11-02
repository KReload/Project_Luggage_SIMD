#include "nrdef.h"
#include "nrutil.h"
#include <stdlib.h>

#define VMIN 1
#define VMAX 254
#define N 2

void sigmaDelta(uint8** I0, uint8** I1, uint8** M0, uint8** M1, uint8** V0, uint8** V1, uint8** O1, uint8** E1, long nrl, long nrh, long ncl, long nch);
