#include "../include/framediff.h"

int main(void) {
  //Prend 2 images et applique l'algo frame difference
  long nrl, nrh, ncl,nch;
  uint8 theta = 70;
  
  uint8** img = LoadPGM_ui8matrix("../hall/hall000030.pgm",&nrl,&nrh,&ncl,&nch);
  uint8** img2 = LoadPGM_ui8matrix("../hall/hall000031.pgm",&nrl,&nrh,&ncl,&nch);
  uint8** O = ui8matrix(nrl, nrh, ncl, nch);
  uint8** E = ui8matrix(nrl, nrh, ncl, nch);

  frameDifference(img, img2, O, E, nrl, nrh, ncl, nch, theta);
  
  SavePGM_ui8matrix(O, nrl, nrh, ncl, nch, "hallO.pgm");
  SavePGM_ui8matrix(E, nrl, nrh, ncl, nch, "hallE.pgm");
   
  return 0;
}
