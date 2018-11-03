#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <stddef.h>
#include <ctype.h> 
#include <math.h> /* fabs */


#include "include/vnrutil.h"
#include "include/simdutil.h"

#define THETA 10




vuint8** frameDifference(char* filename0, char* filename1, long* nrl,long* nrh,long* ncl, long* nch)
{
  vuint8** I0 = LoadPGM_vui8matrix(filename0, nrl, nrh, ncl, nch);
  vuint8** I1 = LoadPGM_vui8matrix(filename1, nrl, nrh, ncl, nch);
  vuint8** O = vui8matrix(*nrl, *nrh, *ncl, *nch);
  vuint8** E = vui8matrix(*nrl, *nrh, *ncl, *nch);

  //vecteur 16x8 bits chaque octet contient THETA
  vuint8 theta = init_vuint8((uint8)THETA);
  vuint8 low = init_vuint8((uint8)0);
  vuint8 high = init_vuint8((uint8)255);
  vuint8 C,ab,ba;
  vuint8 D;
  
  for(int i = 0; i < *nrh; i++)
    {
      for(int j = 0; j < *nch; j++)
      {
        ab = _mm_subs_epu8((__m128i)I1[i][j],(__m128i)I0[i][j]);
        ba = _mm_subs_epu8((__m128i)I0[i][j],(__m128i)I1[i][j]);
        O[i][j] = _mm_or_si128(ab,ba);
        
        C = (vuint8)_mm_cmplt_epi8((__m128i)theta,(__m128i)O[i][j]);

        D = _mm_or_si128(_mm_and_si128((__m128i)C,(__m128i)high),_mm_andnot_si128((__m128i)C,(__m128i)low));

        _mm_store_si128((__m128i*)&E[i][j],D);

      }
    }
  
  return E;
}



int main(void)
{
  long nrl, nrh, ncl,nch;
  //vuint8**img = LoadPGM_vui8matrix("./hall/hall000000.pgm",&nrl,&nrh,&ncl,&nch);
  //vuint8**img2 = LoadPGM_vui8matrix("./hall/hall000000.pgm",&nrl,&nrh,&ncl,&nch);

  vuint8**E = frameDifference("./hall/hall000000.pgm", "./hall/hall000001.pgm", &nrl,&nrh,&ncl,&nch);

  SavePGM_vui8matrix(E, nrl, nrh, ncl, nch, "hall1SIMDtest.pgm");

}
