#include <stdlib.h>
#include <stdio.h>
#include "../include/validation.h"

int main(int argc, char* argv[])
{
  long nrl, nrh, ncl, nch;
  
  uint32** ROC = ui32matrix(0, 2, 0, 2);
  uint8** IVT = LoadPGM_ui8matrix("hall32_VT.pgm",&nrl,&nrh,&ncl,&nch);
  uint8** ISD = LoadPGM_ui8matrix("../exe/hallSDE/hall000032E.pgm",&nrl,&nrh,&ncl,&nch);
 
  calculMatriceROC(ROC, ISD, IVT, nrl, nrh, ncl, nch);
  float mcc = calculMCC(ROC);
  
  display_ui32matrix(ROC,0,1,0,1,"%d ","Matrice ROC : ");
  printf("\nMCC = %f\n",mcc); 
  
  free_ui8matrix(IVT, nrl, nrh, ncl, nch);
  free_ui8matrix(ISD, nrl, nrh, ncl, nch);
  free_ui32matrix(ROC, 0, 2, 0, 2);
  return 0;
}
