#include "nrdef.h"
#include "nrutil.h"
#include <omp.h>

#define CHUNK 16

uint8** dilatation(uint8** M, int nrl, int nrh, int ncl, int nch, int dim);
uint8** erosion(uint8** M, int nrl, int nrh, int ncl, int nch, int dim);
uint8** ouverture(uint8** M, int nrl, int nrh, int ncl, int nch, int dim);
uint8** fermeture(uint8** M, int nrl, int nrh, int ncl, int nch, int dim);
