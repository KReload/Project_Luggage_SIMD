#include <stdlib.h>
#include <stdio.h>
#include "include/morpho.h"
#include "include/mouvement.h"


void detectionMouvement()
{
  long nrl, nrh, ncl, nch;
  int dim = 3;
  char* filename0 = (char*) malloc(sizeof(char) * FILENAMESIZE);
  char* filename1 = (char*) malloc(sizeof(char) * FILENAMESIZE);
  char* filenameE = (char*) malloc(sizeof(char) * (FILENAMESIZE+2));
  char* filenameO = (char*) malloc(sizeof(char) * (FILENAMESIZE+2));
  uint8** I0;
  uint8** I1;
  uint8** M0 = LoadPGM_ui8matrix("../hall/hall000000.pgm",&nrl,&nrh,&ncl,&nch);
  uint8** O1 = ui8matrix(nrl, nrh, ncl, nch);
  uint8** E1 = ui8matrix(nrl, nrh, ncl, nch);
  uint8** M1 = ui8matrix(nrl, nrh, ncl, nch);
  uint8** V0 = ui8matrix(nrl, nrh, ncl, nch);
  uint8** V1 = ui8matrix(nrl, nrh, ncl, nch);
  
  for(int i = 0; i < NIMAGES - 1; i++)
    {
      sprintf(filename0,"../hall/hall%06d.pgm",i);
      sprintf(filename1,"../hall/hall%06d.pgm",i+1);
      I0 = LoadPGM_ui8matrix(filename0, &nrl, &nrh, &ncl, &nch);
      I1 = LoadPGM_ui8matrix(filename1, &nrl, &nrh, &ncl, &nch);

      sigmaDelta(I0, I1, M0, M1, V0, V1, O1, E1, nrl, nrh, ncl, nch);
      /*
      O1 = fermeture(O1, nrl, nrh, ncl, nch, dim);
      E1 = ouverture(E1, nrl, nrh, ncl, nch, dim);
      E1 = fermeture(E1, nrl, nrh, ncl, nch, dim);
      E1 = ouverture(E1, nrl, nrh, ncl, nch, 5);
      E1 = fermeture(E1, nrl, nrh, ncl, nch, 5);
      */
      sprintf(filenameO,"./hallSDO/hall%06dO.pgm",i);
      sprintf(filenameE,"./hallSDE/hall%06dE.pgm",i+1);
      
      SavePGM_ui8matrix(O1, nrl, nrh, ncl, nch, filenameO);
      SavePGM_ui8matrix(E1, nrl, nrh, ncl, nch, filenameE);

      copy_ui8matrix_ui8matrix (V1, nrl, nrh, ncl, nch, V0);
      copy_ui8matrix_ui8matrix (M1, nrl, nrh, ncl, nch, M0);
    }

  // free_ui8matrix(O1, nrl+2, nrh+2, ncl+2, nch+2);
  // free_ui8matrix(E1, nrl+2, nrh+2, ncl+2, nch+2);
  free_ui8matrix(I0, nrl, nrh, ncl, nch);
  free_ui8matrix(I1, nrl, nrh, ncl, nch);
  free_ui8matrix(V0, nrl, nrh, ncl, nch);
  free_ui8matrix(V1, nrl, nrh, ncl, nch);
  free_ui8matrix(M0, nrl, nrh, ncl, nch);
  free_ui8matrix(M1, nrl, nrh, ncl, nch);

}

int main(int argc, char* argv[])
{
  detectionMouvement();
  return 0;
}
