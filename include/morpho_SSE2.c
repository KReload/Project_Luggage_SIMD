#include "morpho_SSE2.h"

void copy_vui8matrix_vui8matrix(vuint8 **X, int i0, int i1, int j0, int j1, vuint8 **Y)
{
    int i, j;

    for(i=i0; i<=i1; i++) {
        for(j=j0; j<=j1; j++) {
            Y[i][j] = X[i][j];
        }
    }
}

vuint8** dilatation_SSE(vuint8** M, int nrl, int nrh, int ncl, int nch, int dim)
{
  int r = dim/2;
  vuint8 max = init_vuint8((uint8)0);
  vuint8 zero = init_vuint8((uint8)0);
  vuint8 cmp0, cmp1, cmp2, cmp3;
  vuint8 vk, vp, vnrl, vncl, vnrh, vnch;

  vuint8** tmp = vui8matrix(nrl-r, nrh+r, ncl-r, nch+r);
  vuint8** output = vui8matrix(nrl, nrh, ncl, nch);
  copy_vui8matrix_vui8matrix(M, nrl, nrh, ncl, nch, tmp);
  
  for(int i = nrl; i < nrh; i++)
    {
      for(int j = ncl; j < nch; j++)
	{
	  for(int k = i-r; k < i+(r+1); k++)
	    {
	      for(int p = j-r; p < j+(r+1); p++)
		{
		  max =  _mm_max_epu8(tmp[k][p],max);
		}
	    }
	  _mm_store_si128((__m128i*)&output[i][j],max);
	  max = zero;
	}
    }
	
  return output;
}


vuint8** erosion_SSE(vuint8** M, int nrl, int nrh, int ncl, int nch, int dim)
{
  int r = dim/2;
  vuint8 min = init_vuint8((uint8)255);
  vuint8 nzero = init_vuint8((uint8)255);
  vuint8 cmp0, cmp1, cmp2, cmp3;
  vuint8 vk, vp, vnrl, vncl, vnrh, vnch;
  
  vuint8** tmp = vui8matrix(nrl-r, nrh+r, ncl-r, nch+r);
  vuint8** output = vui8matrix(nrl, nrh, ncl, nch);
  copy_vui8matrix_vui8matrix(M, nrl, nrh, ncl, nch, tmp);
  
   for(int i = 0; i < nrh; i++)
    {
      for(int j = 0; j < nch; j++)
	{
	  for(int k = i-r; k < i+(r+1); k++)
	    {
	      for(int p = j-r; p < j+(r+1); p++)
		{
		  min = _mm_min_epu8(tmp[k][p],min);
		}
	    }
	  _mm_store_si128((__m128i*)&output[i][j],min);
	  min = nzero;
	}
    }
   
   return output;
}

vuint8** fermeture_SSE(vuint8** M, int nrl, int nrh, int ncl, int nch, int dim)
{
  vuint8** output;
  output = dilatation_SSE(M, nrl, nrh, ncl, nch, dim);
  output = erosion_SSE(output, nrl, nrh, ncl, nch, dim);

  return output;
}

vuint8** ouverture_SSE(vuint8** M, int nrl, int nrh, int ncl, int nch, int dim)
{
  vuint8** output;
  output = erosion_SSE(M, nrl, nrh, ncl, nch, dim);
  output = dilatation_SSE(output, nrl, nrh, ncl, nch, dim);

  return output;
}
