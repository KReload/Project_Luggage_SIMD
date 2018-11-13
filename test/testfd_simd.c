#include "../include/framediff_SSE2.h"

int main(void)
{
  uint8 theta = 10;
  int nrl = 0;
  int nrh = 5;
  int ncl = 0;
  int nch = 3;
  vuint8** I0 = vui8matrix(int nrl, int nrh, int ncl, int nch);
  vuint8** I1 = vui8matrix(int nrl, int nrh, int ncl, int nch);

  for(int i = nrl; i < nrh; i++)
    {
      for(int j = ncl; j < nch; j++)
	{
	  I0[i][j] = 255;
	  I1[i][j] = 0;
	}
    }

  vuint8**E = frameDifference(I0, I1, nrl,nrh,ncl,nch,theta);

  
  for(int i = nrl; i < nrh; i++)
    {
      for(int j = ncl; j < nch; j++)
	{
	  printf("E[%d][%d] = %d\n",i,j,E[i][j]);
	}
    }
  
}
