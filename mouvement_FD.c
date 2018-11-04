#include <stdlib.h>
#include <stdio.h>
#include "include/morpho.h"
#include "include/framediff.h"

#define FILENAMESIZE 22
#define NIMAGES 300


void detectionMouvement(uint8 theta)
{
  long nrl, nrh, ncl, nch;
  int dim = 3;
  char* filename0 = (char*) malloc(sizeof(char) * FILENAMESIZE);
  char* filename1 = (char*) malloc(sizeof(char) * FILENAMESIZE);
  char* filenameE = (char*) malloc(sizeof(char) * (FILENAMESIZE+2));
  char* filenameO = (char*) malloc(sizeof(char) * (FILENAMESIZE+2));
  uint8** I0 = LoadPGM_ui8matrix("../hall/hall000000.pgm",&nrl,&nrh,&ncl,&nch);
  uint8** I1;
  uint8** O = ui8matrix(nrl, nrh, ncl, nch);
  uint8** E = ui8matrix(nrl, nrh, ncl, nch);
  
  for(int i = 0; i < NIMAGES - 1; i++)
    {
      sprintf(filename0,"../hall/hall%06d.pgm",i);
      sprintf(filename1,"../hall/hall%06d.pgm",i+1);
      I0 = LoadPGM_ui8matrix(filename0, &nrl, &nrh, &ncl, &nch);
      I1 = LoadPGM_ui8matrix(filename1, &nrl, &nrh, &ncl, &nch);
      
      frameDifference(I0, I1, O, E, nrl, nrh, ncl, nch, theta);
      O = fermeture(O, nrl, nrh, ncl, nch, dim);
      E = fermeture(E, nrl, nrh, ncl, nch, dim);
      E = erosion(E, nrl, nrh, ncl, nch, dim);
    
      sprintf(filenameO,"./hallFDO/hall%06dO.pgm",i);
      sprintf(filenameE,"./hallFDE/hall%06dE.pgm",i+1);
      
      SavePGM_ui8matrix(O, nrl, nrh, ncl, nch, filenameO);
      SavePGM_ui8matrix(E, nrl, nrh, ncl, nch, filenameE);
      
    }
}

int main(int argc, char* argv[])
{
  uint8 theta = atoi(argv[1]);
  detectionMouvement(theta);
  return 0;
}
