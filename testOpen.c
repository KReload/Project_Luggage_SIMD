#include "include/nrdef.h"
#include "include/nrutil.h"
#include <stdio.h>


int main(void) {
  //Prend une image et inverse les niveaux de gris
  long nrl, nrh, ncl,nch;
  uint8**img = LoadPGM_ui8matrix("./hall/hall000000.pgm",&nrl,&nrh,&ncl,&nch);
  for(int i = 0; i<nrh;i++) {
    for(int j = 0; j<nch;j++) {
      img[i][j] = 255 - img[i][j];
    }
  }

  SavePGM_ui8matrix(img, nrl, nrh, ncl, nch, "hall0Inv.pgm");

  return 0;
}
