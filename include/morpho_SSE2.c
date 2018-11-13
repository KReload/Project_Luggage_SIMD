#include "morpho.h"
#include <stdio.h>
#include "vnrutil.h"
#include "simdutil.h"
#include <stddef.h>
#include <ctype.h> 

vuint8 sel_si128(vuint8 vcontrole,vuint8 a, vuint8 b){
  return _mm_or_si128(_mm_and_si128((__m128i)vcontrole,(__m128i)a),_mm_andnot_si128((__m128i)vcontrole,(__m128i)b));
}


vuint8** dilatation_SSE(vuint8** M, int nrl, int nrh, int ncl, int nch, int dim)
{
  int r = dim/2;
  vuint8 max = init_vuint8((uint8)0);
  vuint8 zero = init_vuint8((uint8)0);
  vuint8 cmp0, cmp1, cmp2, cmp3;
  vuint8 vk, vp, vnrl, vncl, vnrh, vnch;
  
  //Allocation sortie de taille (r+dim)*(c+dim) pour avoir une bordure
  vuint8** output = vui8matrix(nrl-r, nrh+r, ncl-r, nch+r);
  for(int i = 0; i < nrh; i++)
    {
      for(int j = 0; j < nch; j++)
	{
	  for(int k = i-r; k < i+(r+1); k++)
	    {
	      for(int p = j-r; p < j+(r+1); p++)
		{
		  vk = init_vuint8((uint8)k);
		  vp = init_vuint8((uint8)p);
		  vncl = init_vuint8((uint8)ncl);
		  vnch = init_vuint8((uint8)nch);
		  vnrl = init_vuint8((uint8)nrl);
		  vnrh = init_vuint8((uint8)nrh);

		  max = MAX_SSE(M[k][p],max);

		  cmp0 = _mm_cmplt_epi8(vk,vnrl);
		  cmp1 = _mm_cmplt_epi8(vp,vncl);
		  cmp2 = _mm_cmplt_epi8(vnrh,vk);
		  cmp3 = _mm_cmplt_epi8(vnch,vp);

		  max = sel_si128(cmp0,zero,max);
		  max = sel_si128(cmp1,zero,max);
		  max = sel_si128(cmp2,zero,max);
		  max = sel_si128(cmp3,zero,max);
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
  
  //Allocation sortie de taille (r+dim)*(c+dim) pour avoir une bordure
  vuint8** output = vui8matrix(nrl-r, nrh+r, ncl-r, nch+r);
   for(int i = 0; i < nrh; i++)
    {
      for(int j = 0; j < nch; j++)
	{
	  for(int k = i-r; k < i+(r+1); k++)
	    {
	      for(int p = j-r; p < j+(r+1); p++)
		{
		  vk = init_vuint8((uint8)k);
		  vp = init_vuint8((uint8)p);
		  vncl = init_vuint8((uint8)ncl);
		  vnch = init_vuint8((uint8)nch);
		  vnrl = init_vuint8((uint8)nrl);
		  vnrh = init_vuint8((uint8)nrh);

		  min = MIN_SSE(M[k][p],min);

		  cmp0 = _mm_cmplt_epi8(vk,vnrl);
		  cmp1 = _mm_cmplt_epi8(vp,vncl);
		  cmp2 = _mm_cmplt_epi8(vnrh,vk);
		  cmp3 = _mm_cmplt_epi8(vnch,vp);
		  
		  min = sel_si128(cmp0,nzero,min);
		  min = sel_si128(cmp1,nzero,min);
		  min = sel_si128(cmp2,nzero,min);
		  min = sel_si128(cmp3,nzero,min);
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
