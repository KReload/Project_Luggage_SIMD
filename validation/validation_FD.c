#include <stdlib.h>
#include <stdio.h>
#include "../include/validation.h"

int main(int argc, char* argv[])
{
  long nrl, nrh, ncl, nch;
  
  uint32** ROC = ui32matrix(0, 2, 0, 2);
  uint8** IVT;
  uint8** IFD;
  char* filename0 = (char*) malloc(sizeof(char) * 13);
  char* filename1 = (char*) malloc(sizeof(char) * 30);

  for(int i = 32; i < 92; i+=20)
    {
      printf("+===================================================+");
      printf("IMAGE %d :\n", i);
      sprintf(filename0,"hall%d_VT.pgm",i);
      sprintf(filename1,"../exe/hallFDE/hall0000%dE.pgm",i);

      IVT = LoadPGM_ui8matrix(filename0,&nrl,&nrh,&ncl,&nch);
      IFD = LoadPGM_ui8matrix(filename1,&nrl,&nrh,&ncl,&nch);
 
      calculMatriceROC(ROC, IFD, IVT, nrl, nrh, ncl, nch);
      float mcc = calculMCC(ROC);
      
      display_ui32matrix(ROC,0,1,0,1,"%d ","Matrice ROC : ");
      printf("\nMCC = %f\n",mcc); 
    }
  free_ui8matrix(IVT, nrl, nrh, ncl, nch);
  free_ui8matrix(IFD, nrl, nrh, ncl, nch);
  free_ui32matrix(ROC, 0, 2, 0, 2);
  return 0;
}
