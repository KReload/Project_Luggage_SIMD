#include <omp.h>
#include "simdutil.h"

#define CHUNK 16

vuint8** dilatation_SSE(vuint8** M, int nrl, int nrh, int ncl, int nch, int dim);
vuint8** erosion_SSE(vuint8** M, int nrl, int nrh, int ncl, int nch, int dim);
vuint8** fermeture_SSE(vuint8** M, int nrl, int nrh, int ncl, int nch, int dim);
vuint8** ouverture_SSE(vuint8** M, int nrl, int nrh, int ncl, int nch, int dim);


vuint8** dilatation_SSE3x3_elemVertical(vuint8** M, int nrl, int nrh, int ncl, int nch);
vuint8** dilatation_SSE3x3_elemHorizontal(vuint8** M, int nrl, int nrh, int ncl, int nch);
vuint8** erosion_SSE3x3_elemVertical(vuint8** M, int nrl, int nrh, int ncl, int nch);
vuint8** erosion_SSE3x3_elemHorizontal(vuint8** M, int nrl, int nrh, int ncl, int nch);
