#include "include/morpho.h"
#include <stdio.h>

int main(void) {
  //Prend une image et fais erosion, dilatation, ouverture, fermeture
  long nrl, nrh, ncl,nch;
  int dim = 3;
  int seuil = 110;

  uint8**imgtest = LoadPGM_ui8matrix("./hall/hall000030.pgm",&nrl,&nrh,&ncl,&nch);
  for(int i = 0; i<nrh;i++) {
    for(int j = 0; j<nch;j++) {
      printf("%d ",imgtest[i][j]);
    }
    printf("\n");
  }
  
  uint8**img = LoadPGM_ui8matrix("./hall/hall000000.pgm",&nrl,&nrh,&ncl,&nch);
  uint8**imgErosion = erosion(img, nrl, nrh, ncl, nch, dim, seuil); 
  uint8**imgDilatation = dilatation(img, nrl, nrh, ncl, nch, dim, seuil);
  uint8**imgOuverture = ouverture(img, nrl, nrh, ncl, nch, dim, seuil);
  uint8**imgFermeture = fermeture(img, nrl, nrh, ncl, nch, dim, seuil);
  
  SavePGM_ui8matrix(imgErosion, nrl, nrh, ncl, nch, "hall0Erosion.pgm");
  SavePGM_ui8matrix(imgDilatation, nrl, nrh, ncl, nch, "hall0Dilatation.pgm");
  SavePGM_ui8matrix(imgOuverture, nrl, nrh, ncl, nch, "hall0Ouverture.pgm");
  SavePGM_ui8matrix(imgFermeture, nrl, nrh, ncl, nch, "hall0Fermeture.pgm");
  
  return 0;
}
