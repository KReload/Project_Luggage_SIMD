#include "nrdef.h"
#include "nrutil.h"


int main(void) {

  long nrl, nrh, ncl,nch;
  uint8**img = LoadPGM_ui8matrix("./hall/hall000000.pgm",&nrl,&nrh,&ncl,&nch);

  return 0;
}
