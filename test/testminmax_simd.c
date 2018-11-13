#include <stdio.h>
#include "../include/vnrutil.h"
#include "../include/simdutil.h"
#include <stddef.h>
#include <ctype.h> 

#define MAX_SSE(a,b) (sel_si128((vuint8)_mm_cmplt_epi8((__m128i)a,(__m128i)b),b,a))
#define MIN_SSE(a,b) (sel_si128((vuint8)_mm_cmplt_epi8((__m128i)a,(__m128i)b),a,b))


vuint8 sel_si128(vuint8 vcontrole,vuint8 a, vuint8 b){
  return _mm_or_si128(_mm_and_si128((__m128i)vcontrole,(__m128i)a),_mm_andnot_si128((__m128i)vcontrole,(__m128i)b));
}


int main(void)
{
  vuint8 unit = init_vuint8((uint8)1);
  vuint8 zero = init_vuint8((uint8)0);
  vuint8 min = init_vuint8((uint8)10); //random value
  vuint8 max = init_vuint8((uint8)11); //random value
  vuint8 expect = init_vuint8((uint8)12); //random value
   
  min = MIN_SSE(unit,zero);
  expect = zero;
  display_vuint8(min, "%d ", "\nMIN(1,0) = ");
  
  min = MIN_SSE(zero,unit);
  expect = zero;
  display_vuint8(min, "%d ", "\nMIN(0,1) = ");

  min = MIN_SSE(unit,unit);
  expect = unit;
  display_vuint8(min, "%d ", "\nMIN(1,1) = ");
  
  max = MAX_SSE(unit,zero);
  expect = unit;
  display_vuint8(max, "%d ", "\nMAX(1,0) = ");
  
  max = MAX_SSE(zero,unit);
  expect = unit;
  display_vuint8(max, "%d ", "\nMAX(0,1) = ");
  
  max = MAX_SSE(zero,zero);
  expect = zero;
  display_vuint8(max, "%d ", "\nMAX(0,0) = ");
  
  return 0;
}
