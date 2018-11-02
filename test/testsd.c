#include "../include/sigdelta.h"
#include <stdio.h>
#define FILENAMESIZE 23
#define NIMAGES 300


int main(void) {
  //Prend 2 images et applique l'algo sigma delta
  long nrl, nrh, ncl,nch;
  char* filename0 = (char*) malloc(sizeof(char) * FILENAMESIZE);
  char* filename1 = (char*) malloc(sizeof(char) * FILENAMESIZE);
  uint8** img;
  uint8** img2;

  uint8** M0 = LoadPGM_ui8matrix("../hall/hall000000.pgm",&nrl,&nrh,&ncl,&nch);
  uint8** O1 = ui8matrix(nrl, nrh, ncl, nch);
  uint8** E1 = ui8matrix(nrl, nrh, ncl, nch);
  uint8** M1 = ui8matrix(nrl, nrh, ncl, nch);
  uint8** V0 = ui8matrix(nrl, nrh, ncl, nch);
  uint8** V1 = ui8matrix(nrl, nrh, ncl, nch);
  
  for(int i = 0; i < nrh; i++)
    {
      for(int j = 0; j < nch; j++)
	V0[i][j] = VMIN;
    }
  
  for(int i = 0; i < NIMAGES - 1; i++)
    {
      printf("i = %d\n",i);
      sprintf(filename0,"../hall/hall%06d.pgm",i);
      sprintf(filename1,"../hall/hall%06d.pgm",i+1);
      img = LoadPGM_ui8matrix(filename0, &nrl, &nrh, &ncl, &nch);
      img2 = LoadPGM_ui8matrix(filename1, &nrl, &nrh, &ncl, &nch);
      sigmaDelta(img, img2, M0, M1, V0, V1, O1, E1, nrl, nrh, ncl, nch);
      
      if(i == 38)
	{
	  SavePGM_ui8matrix(O1, nrl, nrh, ncl, nch, "hallO1.pgm");
	  SavePGM_ui8matrix(E1, nrl, nrh, ncl, nch, "hallE1.pgm");
	  SavePGM_ui8matrix(M0, nrl, nrh, ncl, nch, "hallM0.pgm");
	  SavePGM_ui8matrix(M1, nrl, nrh, ncl, nch, "hallM1.pgm");
	  SavePGM_ui8matrix(V0, nrl, nrh, ncl, nch, "hallV0.pgm");
	  SavePGM_ui8matrix(V1, nrl, nrh, ncl, nch, "hallV1.pgm");
	}
      
      copy_ui8matrix_ui8matrix (V1, nrl, nrh, ncl, nch, V0);
      copy_ui8matrix_ui8matrix (M1, nrl, nrh, ncl, nch, M0);
    }
   
  free_ui8matrix(M0, nrl, nrh, ncl, nch);
  free_ui8matrix(M1, nrl, nrh, ncl, nch);
  free_ui8matrix(V0, nrl, nrh, ncl, nch);
  free_ui8matrix(V1, nrl, nrh, ncl, nch);
  free_ui8matrix(O1, nrl, nrh, ncl, nch);
  free_ui8matrix(E1, nrl, nrh, ncl, nch);
  free_ui8matrix(img, nrl, nrh, ncl, nch);
  free_ui8matrix(img2, nrl, nrh, ncl, nch);
  return 0;
}
