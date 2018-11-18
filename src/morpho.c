#include "../include/morpho.h"
#include <stdio.h>



uint8** dilatation(uint8** M, int nrl, int nrh, int ncl, int nch, int dim)
{
  int r = dim/2;
  int max = 0;
  
  uint8** output = ui8matrix(nrl, nrh, ncl, nch);
  uint8** tmp = ui8matrix(nrl-r,nrh+r,ncl-r,nch+r);

  copy_ui8matrix_ui8matrix(M, nrl, nrh, ncl, nch, tmp);
    
  for(int i = 0; i < nrh; i++)
    {
      for(int j = 0; j < nch; j++)
	{
	  if(dim == 3)
	    {
	      output[i][j] = tmp[i][j-1] | tmp[i][j] | tmp[i][j+1] |
		tmp[i-1][j-1] | tmp[i-1][j] | tmp[i-1][j+1] |
		tmp[i+1][j-1] | tmp[i+1][j] | tmp[i+1][j+1];
	    }
	  else if(dim == 5)
	    {
	      output[i][j] = tmp[i-2][j-2] | tmp[i-2][j-1] | tmp[i-2][j] | tmp[i-2][j+1] | tmp[i-2][j+2] |
		tmp[i-1][j-2] | tmp[i-1][j-1] | tmp[i-1][j] | tmp[i-1][j+1] | tmp[i-1][j+2] |
		tmp[i][j-2] | tmp[i][j-1] | tmp[i][j] | tmp[i][j+1] | tmp[i][j+2] |
		tmp[i+1][j-2] | tmp[i+1][j-1] | tmp[i+1][j] | tmp[i+1][j+1] | tmp[i+1][j+2] |
		tmp[i+2][j-2] | tmp[i+2][j-1] | tmp[i+2][j] | tmp[i+2][j+1] | tmp[i+2][j+2];
	    }
	}
    }
  
  return output;
}

uint8** erosion(uint8** M, int nrl, int nrh, int ncl, int nch, int dim)
{
  int r = dim/2;
  int min = 255;

  uint8** output = ui8matrix(nrl, nrh, ncl, nch);
  uint8** tmp = ui8matrix(nrl-r,nrh+r,ncl-r,nch+r);

  copy_ui8matrix_ui8matrix(M, nrl, nrh, ncl, nch, tmp);
    
   
  for(int i = 0; i < nrh; i++)
    {
      for(int j = 0; j < nch; j++)
	{
	  if(dim == 3)
	    {
	      output[i][j] = tmp[i][j-1] & tmp[i][j] & tmp[i][j+1] &
		tmp[i-1][j-1] & tmp[i-1][j] & tmp[i-1][j+1] &
		tmp[i+1][j-1] & tmp[i+1][j] & tmp[i+1][j+1];
	    }
	  else if(dim == 5)
	    {
	      output[i][j] = tmp[i-2][j-2] & tmp[i-2][j-1] & tmp[i-2][j] & tmp[i-2][j+1] & tmp[i-2][j+2] &
		tmp[i-1][j-2] & tmp[i-1][j-1] & tmp[i-1][j] & tmp[i-1][j+1] & tmp[i-1][j+2] &
		tmp[i][j-2] & tmp[i][j-1] & tmp[i][j] & tmp[i][j+1] & tmp[i][j+2] &
		tmp[i+1][j-2] & tmp[i+1][j-1] & tmp[i+1][j] & tmp[i+1][j+1] & tmp[i+1][j+2] &
		tmp[i+2][j-2] & tmp[i+2][j-1] & tmp[i+2][j] & tmp[i+2][j+1] & tmp[i+2][j+2];
	    }
	}
    }

  return output;
}

uint8** fermeture(uint8** M, int nrl, int nrh, int ncl, int nch, int dim)
{
  uint8** output;
  output = dilatation(M, nrl, nrh, ncl, nch, dim);
  output = erosion(output, nrl, nrh, ncl, nch, dim);

  return output;
}

uint8** ouverture(uint8** M, int nrl, int nrh, int ncl, int nch, int dim)
{
  uint8** output;
  output = erosion(M, nrl, nrh, ncl, nch, dim);
  output = dilatation(output, nrl, nrh, ncl, nch, dim);
  
  return output;
}
