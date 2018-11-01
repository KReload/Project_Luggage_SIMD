#include <stdlib.h>
#include "include/nrdef.h"
#include "include/nrutil.h"
#include <emmintrin.h>
#include "include/vnrutil.h"

#define THETA 10


vuint8** load_uint8_to_vui8matrix(uint8**a, long nrl, long nrh, long ncl, long nch) {

  //Convertion de uint8 matrix à vuint8 matrix (qui est une _m128i matrice)
  vuint8** res = vui8matrix(nrl/16, nrh/16 +1, ncl/16, nch/16 + 1);
  uint8* temp;
  
  for(int i=0;i<(nrh-nrl)/16 + 1;i++){
    for(int j=0;j<(nch-ncl)/16 + 1;j++) {
      res[i] = _mm_load_si128(a[i][j*16]);
    }
  }

  return res;
}

void frameDifference(char* filename0, char* filename1)
{
  long nrl, nrh, ncl, nch;
  uint8** I0 = LoadPGM_ui8matrix(filename0, &nrl, &nrh, &ncl, &nch);
  uint8** I1 = LoadPGM_ui8matrix(filename1, &nrl, &nrh, &ncl, &nch);
  vuint8** O = vui8matrix(nrl, nrh, ncl, nch);
  vuint8** E = vui8matrix(nrl, nrh, ncl, nch);

  //vecteur 16x8 bits chaque octet contient THETA
  vuint8 theta = init_vuint8((uint8)THETA);
  
  
  for(int i = 0; i < nrh; i++)
    {
      for(int j = 0; j < nch; j++)
      {
        O[i][j] = abs(_mm_sub_epi8(I1[i][j],I0[i][j]));

        if(_mm_cmplt_epi8(O[i][j], theta))
          _mm_store_si128(E[i][j], 0);
        else
          _mm_store_si128(E[i][j], 1);
      }
    }
}

int main(void)
{
  return 0;
}
