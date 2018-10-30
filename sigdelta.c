#include "nrdef.h"
#include "sigdelta.h"
#include "nrutil.h"
#include <math.h>


void sigmaDelta(char* filename1, char* filename2)
{
  long nrl,nrh,ncl,nch;
  uint8** I0 = LoadPGM_ui8matrix(filename1,&nrl,&nrh,&ncl,&nch);
  uint8** I1 = LoadPGM_ui8matrix(filename2,&nrl,&nrh,&ncl,&nch);
  uint8** M0 = I0;
  uint8** M1 = ui8matrix(nrl,nrh,ncl,nch);
  uint8** V0 = ui8matrix(nrl,nrh,ncl,nch);
  uint8** V1 = ui8matrix(nrl,nrh,ncl,nch);
  uint8** O1 = ui8matrix(nrl,nrh,ncl,nch);
  uint8** E1 = ui8matrix(nrl,nrh,ncl,nch);

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
	    M1[i][j] = V0[i][j] - 1;
	  else
	    V1[i][j] = V0[i][j];
	  V1[i][j] = max(min(V1[i][j],VMAX),VMIN); //Clamp
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
	    E1[i][j] = 1;
	}

    }

}


int main(void)
{
  return 0;
}
