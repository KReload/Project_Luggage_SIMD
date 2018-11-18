#include "../include/mouvement.h"

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


void sigmaDelta(uint8** I0, uint8** I1, uint8** M0, uint8** M1, uint8** V0, uint8** V1, uint8** O1, uint8** E1, long nrl, long nrh, long ncl, long nch)
{
  for(int i = 0; i < nrh; i++)
    {
      for(int j = 0; j < nch; j++)
	{
	  //Step 1 : M1 estimation
	  if(M0[i][j] < I1[i][j])
	    M1[i][j] = M0[i][j] + 1;
	  else if(M0[i][j] > I1[i][j])
	    M1[i][j] = M0[i][j] - 1;
	  else
	    M1[i][j] = M0[i][j];

	  //Step 2 : O1 Computation
	  O1[i][j] = abs(M1[i][j] - I1[i][j]);

	  //Step 3 : V1 Update and Clamping
	  if(V0[i][j] < N*O1[i][j])
	    V1[i][j] = V0[i][j] + 1;
	  else if(V0[i][j] > N*O1[i][j])
	    V1[i][j] = V0[i][j] - 1;
	  else
	    V1[i][j] = V0[i][j];
	  V1[i][j] = MAX(MIN(V1[i][j],VMAX),VMIN); //Clamp

	  //Step 4 : E1 Estimation
	  if(O1[i][j] < V1[i][j])
	    E1[i][j] = 0;
	  else
	    E1[i][j] = 255; 
	}
    }

}
