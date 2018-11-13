#include "simdutil.h"

vuint8** dilatation_SSE(vuint8** M, int nrl, int nrh, int ncl, int nch, int dim);
vuint8** erosion_SSE(vuint8** M, int nrl, int nrh, int ncl, int nch, int dim);
vuint8** fermeture_SSE(vuint8** M, int nrl, int nrh, int ncl, int nch, int dim);
vuint8** ouverture_SSE(vuint8** M, int nrl, int nrh, int ncl, int nch, int dim);
