#include "sigmaDelta.h"
#include "nrutil.h"

void sigmaDelta(char* filename1, char* filename2)
{
  long nrl,nrh,ncl,nch;
  uint8** I0 = LoadPGM_ui8matrix(char *filename,&nrl,&nrh,&ncl,&nch);
  uint8** I1 = LoadPGM_ui8matrix(char *filename,&nrl,&nrh,&ncl,&nch);
  uint8** M = I0;


}

int main(void)
{
  return 0;
}
