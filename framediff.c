#include <stdlib.h>
#include "include/nrdef.h"
#include "include/nrutil.h"

#define THETA 10


void frameDifference(char* filename0, char* filename1)
{
  long nrl, nrh, ncl, nch;
  uint8** I0 = LoadPGM_ui8matrix(filename0, &nrl, &nrh, &ncl, &nch);
  uint8** I1 = LoadPGM_ui8matrix(filename1, &nrl, &nrh, &ncl, &nch);
  uint8** O = ui8matrix(nrl, nrh, ncl, nch);
  uint8** E = ui8matrix(nrl, nrh, ncl, nch);
  uint8 theta = THETA;
  
  for(int i = 0; i < nrh; i++)
    {
      for(int j = 0; j < nch; j++)
	{
	  O[i][j] = abs(I1[i][j] - I0[i][j]);
	  if(O[i][j] < theta)
	    E[i][j] = 0;
	  else
	    E[i][j] = 1; //ou 255
	}
    }
}

int main(void)
{
  return 0;
}
