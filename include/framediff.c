#include "framediff.h"

void frameDifference(uint8** I0, uint8** I1,uint8** O, uint8** E, long nrl, long nrh, long ncl, long nch, uint8 theta)
{ 
  for(int i = 0; i < nrh; i++)
    {
      for(int j = 0; j < nch; j++)
	{
	  O[i][j] = abs(I1[i][j] - I0[i][j]);
	  if(O[i][j] < theta)
	    E[i][j] = 0;
	  else
	    E[i][j] = 255;
	}
    }
}
