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
  vuint8 r = init_vuint8((uint8)dim/2);
  vuint8 max = init_vuint8((uint8)0);
  vuint8 zero = init_vuint8((uint8)0);
  
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
		  max = _mm_max_epi8((__m128i)M[k][p],(__m128i)max)
		  cmp0 = (vuint8)_mm_cmplt_epi8((__m128i)k,(__m128i)nrl);
		  cmp1 = (vuint8)_mm_cmplt_epi8((__m128i)p,(__m128i)ncl);
		  cmp2 = (vuint8)_mm_cmplt_epi8((__m128i)nrh,(__m128i)k);
		  cmp3 = (vuint8)_mm_cmplt_epi8((__m128i)nch,(__m128i)j);
		  max = sel_si128(cmp0,zero,max);
		  max = sel_si128(cmp1,zero,max);
		  max = sel_si128(cmp2,zero,max);
		  max = sel_si128(cmp3,zero,max);
		}
	    }
	  _mm_store_si128((__m128i*)&output[i][j],max);
	  max = 0;
	}
    }
   
   return output;
}


vuint8** erosion_SSE(vuint8** M, int nrl, int nrh, int ncl, int nch, int dim)
{
  vuint8 r = init_vuint8((uint8)dim/2);
  vuint8 min = init_vuint8((uint8)255);
  vuint8 nzero = init_vuint8((uint8)255);
  
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
		  min = _mm_min_epi8((__m128i)M[k][p],(__m128i)min)
		  cmp0 = (vuint8)_mm_cmplt_epi8((__m128i)k,(__m128i)nrl);
		  cmp1 = (vuint8)_mm_cmplt_epi8((__m128i)p,(__m128i)ncl);
		  cmp2 = (vuint8)_mm_cmplt_epi8((__m128i)nrh,(__m128i)k);
		  cmp3 = (vuint8)_mm_cmplt_epi8((__m128i)nch,(__m128i)j);
		  min = sel_si128(cmp0,nzero,min);
		  min = sel_si128(cmp1,nzero,min);
		  min = sel_si128(cmp2,nzero,min);
		  min = sel_si128(cmp3,nzero,min);
		}
	    }
	  _mm_store_si128((__m128i*)&output[i][j],min);
	  min = 255;
	}
    }
   
   return output;
}
