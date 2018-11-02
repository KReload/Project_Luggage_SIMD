#include "sigdelta.h"

void sigmaDelta(uint8** I0, uint8** I1, uint8** M0, uint8** M1, uint8** V0, uint8** V1, uint8** O1, uint8** E1, long nrl, long nrh, long ncl, long nch)
{
  //Step 1 : M1 estimation
  for(int i = 0; i < nrh; i++)
    {
      for(int j = 0; j < nch; j++)
	{
	  if(M0[i][j] < I1[i][j])
	    M1[i][j] = M0[i][j] + 1;
	  else if(M0[i][j] > I1[i][j])
	    M1[i][j] = M0[i][j] - 1;
	  else
	    M1[i][j] = M0[i][j];
	}
    }

  //Step 2 : O1 Computation
  for(int i = 0; i < nrh; i++)
    {
      for(int j = 0; j < nch; j++)
	O1[i][j] = abs(M1[i][j] - I1[i][j]);
    }

  //Step 3 : V1 Update and Clamping
  for(int i = 0; i < nrh; i++)
    {
      for(int j = 0; j < nch; j++)
	{
	  if(V0[i][j] < N*O1[i][j])
	    V1[i][j] = V0[i][j] + 1;
	  else if(V0[i][j] > N*O1[i][j])
	    V1[i][j] = V0[i][j] - 1;
	  else
	    V1[i][j] = V0[i][j];
	  V1[i][j] = MAX(MIN(V1[i][j],VMAX),VMIN); //Clamp
	}
    }

  //Step 4 : E1 Estimation
  for(int i = 0; i < nrh; i++)
    {
      for(int j = 0; j < nch; j++)
	{
	  if(O1[i][j] < V1[i][j])
	    E1[i][j] = 0;
	  else
	    E1[i][j] = 255; 
	}

    }

}
