#include <stdlib.h>
#include <stdio.h>
#include "include/morpho.h"
#include "include/framediff.h"

#define FILENAMESIZE 22
#define NIMAGES 300


void detectionMouvement()
{
  long nrl, nrh, ncl, nch;
  uint8 theta = 200;
  char* filename0 = (char*) malloc(sizeof(char) * FILENAMESIZE);
  char* filename1 = (char*) malloc(sizeof(char) * FILENAMESIZE);
  uint8** I0 = LoadPGM_ui8matrix(filename0, &nrl, &nrh, &ncl, &nch);
  uint8** I1 = LoadPGM_ui8matrix(filename1, &nrl, &nrh, &ncl, &nch);
  uint8** O = ui8matrix(nrl, nrh, ncl, nch);
  uint8** E = ui8matrix(nrl, nrh, ncl, nch);

  for(int i = 0; i < NIMAGES - 1; i++)
    {
      sprintf(filename0,"./hall/hall%06d.pgm",i);
      sprintf(filename1,"./hall/hall%06d.pgm",i+1);
      I0 = LoadPGM_ui8matrix(filename0, &nrl, &nrh, &ncl, &nch);
      I1 = LoadPGM_ui8matrix(filename1, &nrl, &nrh, &ncl, &nch);
      O = ui8matrix(nrl, nrh, ncl, nch);
      E = ui8matrix(nrl, nrh, ncl, nch);
      frameDifference(I0, I1, O, E, nrl, nrh, ncl, nch, theta);
    }
}

int main(void)
{
  return 0;
}
