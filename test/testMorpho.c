#include "../include/morpho.h"
#include <stdio.h>

int main(void) {
  //Prend une image et fais erosion, dilatation, ouverture, fermeture
  long nrl, nrh, ncl,nch;
  int dim = 3;
  
  
  uint8**img = LoadPGM_ui8matrix("../hall/hall000000.pgm",&nrl,&nrh,&ncl,&nch);
  uint8**imgErosion = erosion(img, nrl, nrh, ncl, nch, dim); 
  uint8**imgDilatation = dilatation(img, nrl, nrh, ncl, nch, dim);
  uint8**imgOuverture = ouverture(img, nrl, nrh, ncl, nch, dim);
  uint8**imgFermeture = fermeture(img, nrl, nrh, ncl, nch, dim);
  
  SavePGM_ui8matrix(imgErosion, nrl, nrh, ncl, nch, "hall0Erosion.pgm");
  SavePGM_ui8matrix(imgDilatation, nrl, nrh, ncl, nch, "hall0Dilatation.pgm");
  SavePGM_ui8matrix(imgOuverture, nrl, nrh, ncl, nch, "hall0Ouverture.pgm");
  SavePGM_ui8matrix(imgFermeture, nrl, nrh, ncl, nch, "hall0Fermeture.pgm");

  free_ui8matrix(img, nrl, nrh, ncl, nch);
  return 0;
}
