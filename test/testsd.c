#include "../include/sigdelta.h"

int main(void) {
  //Prend 2 images et applique l'algo sigma delta
  long nrl, nrh, ncl,nch;
  
  uint8** img = LoadPGM_ui8matrix("../hall/hall000030.pgm",&nrl,&nrh,&ncl,&nch);
  uint8** img2 = LoadPGM_ui8matrix("../hall/hall000031.pgm",&nrl,&nrh,&ncl,&nch);
  uint8** O1 = ui8matrix(nrl, nrh, ncl, nch);
  uint8** E1 = ui8matrix(nrl, nrh, ncl, nch);
  uint8** M0 = ui8matrix(nrl, nrh, ncl, nch);
  uint8** M1 = ui8matrix(nrl, nrh, ncl, nch);
  uint8** V0 = ui8matrix(nrl, nrh, ncl, nch);
  uint8** V1 = ui8matrix(nrl, nrh, ncl, nch);
  
  sigmaDelta(img, img2, M0, M1, V0, V1, O1, E1, nrl, nrh, ncl, nch);

  
  SavePGM_ui8matrix(O1, nrl, nrh, ncl, nch, "hallO1.pgm");
  SavePGM_ui8matrix(E1, nrl, nrh, ncl, nch, "hallE1.pgm");
   
  return 0;
}
