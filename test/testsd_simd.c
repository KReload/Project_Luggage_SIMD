

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <stddef.h>
#include <ctype.h> 
#include <math.h> /* fabs */


#include "../include/vnrutil.h"
#include "../include/simdutil.h"

#include "../include/mouvement_SSE2.h"

#define N_OCTET 16
/*
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


}*/

void printResultTest(char*desctest, int test) {
    printf("\t\t%s: ",desctest);
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

    printResultTest("sub_abs_epi8(a,b) should be equal to sub_abs_epi(b,a)", checkTest);

}

void test_sel_si128(){
    printf("\nTesting sel_si128\n");
    vuint8 res[1];
    uint8*test = (uint8*)res;
    int checkTest = 1;
    _mm_store_si128(res, sel_si128(init_vuint8(2),init_vuint8(1),init_vuint8(0)));
    //vcontrole 00000010 x 16
    //v1        00000001 x16
    //v2        00000000 x16
    //res       00000000 x16
    for(int i = 0;i<N_OCTET;i++){
        if(test[i]){
            checkTest = 0;
        }
    }

    printResultTest("sel_si128(init_vuint8(2),init_vuint8(1),init_vuint8(0)) should return a 0-initialized vuint8 vector", checkTest);
    checkTest = 1;

    _mm_store_si128(res, sel_si128(init_vuint8(1),init_vuint8(1),init_vuint8(0)));

    //vcontrole 00000001 x 16
    //v1        00000001 x16
    //v2        00000000 x16
    //res       00000001 x16
    for(int i = 0;i<N_OCTET;i++){
        if(test[i]!=1){
            checkTest = 0;
        }
    }

    printResultTest("sel_si128(init_vuint8(1),init_vuint8(1),init_vuint8(0)) should return a 1-initialized vuint8 vector", checkTest);
    checkTest = 1;

    _mm_store_si128(res, sel_si128(init_vuint8(3),init_vuint8(7),init_vuint8(2)));

    //vcontrole 00000011 x 16
    //v1        00000111 x16
    //v2        00000010 x16
    //res       00000000 x16
    for(int i = 0;i<N_OCTET;i++){
        if(test[i]!=3){
            checkTest = 0;
        }
    }

    printResultTest("sel_si128(init_vuint8(3),init_vuint8(7),init_vuint8(2)) should return a 3-initialized vuint8 vector", checkTest);
    checkTest = 1;

    _mm_store_si128(res, sel_si128(init_vuint8(3),init_vuint8(3),init_vuint8(252)));

    //vcontrole 00000011 x 16
    //v1        00000011 x16
    //v2        11111100 x16
    //res       11111111 x16
    for(int i = 0;i<N_OCTET;i++){
        if(test[i]!=255){
            checkTest = 0;
        }
    }
    printResultTest("sel_si128(init_vuint8(3),init_vuint8(3),init_vuint8(252)) should return a 255-initialized vuint8 vector", checkTest);

}

int main(){
    test_sub_abs();
    test_sel_si128();
    //test_mullo_epi8();
    
    return 0;
}