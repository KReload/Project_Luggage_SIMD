

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <stddef.h>
#include <ctype.h> 
#include <math.h> /* fabs */


#include "../include/vnrutil.h"
#include "../include/simdutil.h"

#define N_OCTET 16

void test_mullo_epi8() {
    //Max values
    vuint8 n = init_vuint8(5);
    vuint8 value = init_vuint8(255);
    vuint16*lo = malloc(sizeof(vuint8));
    vuint16*hi= malloc(sizeof(vuint8));
    mullo_epi8(n,value,lo,hi);
    
    vuint16 res= init_vuint16(1275);

    display_vuint16(_mm_cmpeq_epi16(res,*lo),"%x \n","TEST SI 0xFFFF valide lo: \n");
    display_vuint16(_mm_cmpeq_epi16(res,*hi),"%x \n","TEST SI 0xFFFF valide hi: \n");


}

void printResultTest(char*desctest, int test) {
    printf("\t\t%s",desctest);
    if(test){
        printf("\u2714\n");
    } else {
        printf("\u274c\n");
    }
}

//Test sub_abs_epi8 takes only unsigned
void test_sub_abs() {
    printf("\nTesting sub_abs_epi8\n");
    vuint8 left[1];
    vuint8 right[1];

    uint8*vecl = (uint8*) left;
    uint8*vecr = (uint8*) right;

    _mm_store_si128(left, sub_abs_epi8(init_vuint8(5),init_vuint8(255)));
    _mm_store_si128(right, sub_abs_epi8(init_vuint8(255),init_vuint8(5)));

    int checkTest = 1;

    for(int i = 0; i< N_OCTET;i++) {
        if(vecl[i]!=vecr[i]){
            checkTest = 0;
        }
    }

    printResultTest("\tsub_abs_epi8(a,b) should be equal to sub_abs_epi(b,a): ", checkTest);

}

void test_sel_si128(){
    printf('\nTesting sel_si128');
    //sel_si128(_m)
}

int main(){
    test_sub_abs();
    //test_mullo_epi8();
    /*//vuint8** test = vui8matrix(0,3,0,3);
    vuint8 n = init_vuint8(5);
    vuint8 value = init_vuint8(255);
    vuint8 zeros= init_vuint8(0);
    vuint16 n_lo= _mm_unpacklo_epi8(n,zeros);
    vuint16 n_hi= _mm_unpackhi_epi8(n,zeros);
    vuint16 value_lo= _mm_unpacklo_epi8(value,zeros);
    vuint16 value_hi= _mm_unpackhi_epi8(value,zeros);

    //vuint8 a = _mm_set_epi16(1,2,3,4,5,6,7,65535);
    //vuint8 b = _mm_set_epi16(1,2,3,4,5,6,7,3);
    
    vuint16 c = _mm_mullo_epi16(n_lo,value_lo);
    vuint16 d = _mm_mullo_epi16(n_hi,value_hi);


    display_vuint16(n_lo,"%d \t", "n_lo:\t");
    printf("\n");
    display_vuint16(c,"%d \t", "LOW:\t");
    printf("\n");


    display_vuint16(d,"%d \t", "HIGH:\t");
    printf("\n");*/

    

    return 0;
}