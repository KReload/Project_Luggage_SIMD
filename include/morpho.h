#include "nrdef.h"
#include "nrutil.h"

uint8** dilatation(uint8** M, int nrl, int nrh, int ncl, int nch, int dim, int seuil);
uint8** erosion(uint8** M, int nrl, int nrh, int ncl, int nch, int dim, int seuil);
uint8** ouverture(uint8** M, int nrl, int nrh, int ncl, int nch, int dim, int seuil);
uint8** fermeture(uint8** M, int nrl, int nrh, int ncl, int nch, int dim, int seuil);
