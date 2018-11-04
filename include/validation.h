#include <math.h>
#include "nrdef.h"
#include "nrutil.h"

void calculMatriceROC(uint32** ROC, uint8** img, uint8** img_vt, long nrl, long nrh, long ncl, long nch);
double calculMCC(uint32** ROC);
