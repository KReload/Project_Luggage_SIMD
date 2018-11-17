

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <stddef.h>
#include <ctype.h> 
#include <math.h> /* fabs */


#include "../include/vnrutil.h"
#include "../include/simdutil.h"

#include "../include/mouvement_SSE2.h"
#include "../include/morpho_SSE2.h"

#include "../include/testutil.h"




//Test sub_abs_epi8 takes only unsigned
void test_sub_abs() {
    printf("\nTesting sub_abs_epi8\n");
    vuint8 left[1];
    vuint8 right[1];

    uint8*vecl = (uint8*) left;
    uint8*vecr = (uint8*) right;

    _mm_store_si128(left, _mm_sad_epu8(init_vuint8(5),init_vuint8(255)));
    _mm_store_si128(right, _mm_sad_epu8(init_vuint8(255),init_vuint8(5)));

    int checkTest = 1;

    for(int i = 0; i< N_OCTET;i++) {
        if(vecl[i]!=vecr[i]){
            checkTest = 0;
        }
    }

    printResultTest("sub_abs_epi8(a,b) should be equal to sub_abs_epi(b,a)", checkTest);

    _mm_store_si128(left, sub_abs_epi8(init_vuint8(5),init_vuint8(255)));
    _mm_store_si128(right, sub_abs_epi8(init_vuint8(255),init_vuint8(5)));

    checkTest = 1;

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
/*
void test_frame_difference() {
     Image à T - 1
    1  0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
    2  0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
    3  0   100 100 100 0   0   0   0   0   0   0   0   0   0   0   0
    4  0   100 255 100 0   0   0   0   0   0   0   0   0   0   0   0
    5  0   100 255 100 0   0   0   0   0   0   0   0   0   0   0   0
    6  0   100 255 100 0   0   0   0   0   0   0   0   0   0   0   0
    7  0   100 255 100 0   0   0   0   0   0   0   0   0   0   0   0
    8  0   100 255 100 0   0   0   0   0   0   0   0   0   0   0   0
    9  0   100 100 100 0   0   0   0   0   0   0   0   0   0   0   0
    10 0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
    11 0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
    12 0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
    13 0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
    14 0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
    15 0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
    16 0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
    
    */

    /* Image à T
    1  0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
    2  0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
    3  0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
    4  0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
    5  0   100 100 100 0   0   0   0   0   0   0   0   0   0   0   0
    6  0   100 255 100 0   0   0   0   0   0   0   0   0   0   0   0
    7  0   100 255 100 0   0   0   0   0   0   0   0   0   0   0   0
    8  0   100 255 100 0   0   0   0   0   0   0   0   0   0   0   0
    9  0   100 255 100 0   0   0   0   0   0   0   0   0   0   0   0
    10 0   100 255 100 0   0   0   0   0   0   0   0   0   0   0   0
    11 0   100 100 100 0   0   0   0   0   0   0   0   0   0   0   0
    12 0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
    13 0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
    14 0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
    15 0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
    16 0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0

    */

   /* RES

    1 0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
    2 0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
    3 0   255 255 255 0   0   0   0   0   0   0   0   0   0   0   0
    4 0   255 255 255 0   0   0   0   0   0   0   0   0   0   0   0
    5 0   0   255 0   0   0   0   0   0   0   0   0   0   0   0   0
    6 0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
    7 0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
    8 0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
    9 0   0   255 0   0   0   0   0   0   0   0   0   0   0   0   0
    100   255 255 255 0   0   0   0   0   0   0   0   0   0   0   0
    110   255 255 255 0   0   0   0   0   0   0   0   0   0   0   0
    120   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
    130   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
    140   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
    150   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
    160   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0

    

   vuint8*imgnow = (vuint8*)malloc(sizeof(vuint8)*16);
   vuint8*imgprec = (vuint8*)malloc(sizeof(vuint8)*16);

    imgnow[0] = init_vuint8(0);
    imgnow[1] = init_vuint8(0);
    imgnow[2] = init_vuint8(0);
    imgnow[3] = init_vuint8(0);
    imgnow[4] = _mm_set_epi8(0, 100,100,100,0,0,0,0,0,0,0,0,0,0,0,0);
    imgnow[5] = _mm_set_epi8(0, 100,255,100,0,0,0,0,0,0,0,0,0,0,0,0);
    imgnow[6] = _mm_set_epi8(0, 100,255,100,0,0,0,0,0,0,0,0,0,0,0,0);
    imgnow[7] = _mm_set_epi8(0, 100,255,100,0,0,0,0,0,0,0,0,0,0,0,0);
    imgnow[8] = _mm_set_epi8(0, 100,255,100,0,0,0,0,0,0,0,0,0,0,0,0);
    imgnow[9] = _mm_set_epi8(0, 100,255,100,0,0,0,0,0,0,0,0,0,0,0,0);
    imgnow[10] = _mm_set_epi8(0, 100,100,100,0,0,0,0,0,0,0,0,0,0,0,0);
    imgnow[11] = init_vuint8(0);
    imgnow[12] = init_vuint8(0);
    imgnow[13] = init_vuint8(0);
    imgnow[14] = init_vuint8(0);
    imgnow[15] = init_vuint8(0);

    imgprec[0] = init_vuint8(0);
    imgprec[1] = init_vuint8(0);
    imgprec[2] = _mm_set_epi8(0, 100,100,100,0,0,0,0,0,0,0,0,0,0,0,0);
    imgprec[3] = _mm_set_epi8(0, 100,255,100,0,0,0,0,0,0,0,0,0,0,0,0);
    imgprec[4] = _mm_set_epi8(0, 100,255,100,0,0,0,0,0,0,0,0,0,0,0,0);
    imgprec[5] = _mm_set_epi8(0, 100,255,100,0,0,0,0,0,0,0,0,0,0,0,0);
    imgprec[6] = _mm_set_epi8(0, 100,255,100,0,0,0,0,0,0,0,0,0,0,0,0);
    imgprec[7] = _mm_set_epi8(0, 100,255,100,0,0,0,0,0,0,0,0,0,0,0,0);
    imgprec[8] = _mm_set_epi8(0, 100,100,100,0,0,0,0,0,0,0,0,0,0,0,0);
    imgprec[9] = init_vuint8(0);
    imgprec[10] = init_vuint8(0);
    imgprec[11] = init_vuint8(0);
    imgprec[12] = init_vuint8(0);
    imgprec[13] = init_vuint8(0);
    imgprec[14] = init_vuint8(0);
    imgprec[15] = init_vuint8(0);
    
    vuint8**E = frameDifference(imgprec,imgnow,0, 16, 0, 1);

    display_vui8vector(E,0,16, "%d ", "vui8vector");

    free(imgnow);
    free(imgprec);
}
*/

void test_lt(){
    printf("\nTesting less than macro\n");
    vuint8 a = init_vuint8(255);
    vuint8 b = init_vuint8(0);

    vuint8 res[1];

    _mm_store_si128(res,_mm_cmplt_epu8(a,b));

    uint8* test = (uint8*)res;

    int checkTest = 1;

    for(int i=0;i<16;i++) {
        if(test[i]!=0) {
            checkTest = 0;
        }
    }

    printResultTest("255 < 0 should return 00000000*16", checkTest);

    a = init_vuint8(0);
    b = init_vuint8(255);

    _mm_store_si128(res,_mm_cmplt_epu8(a,b));

    test = (uint8*)res;

    checkTest = 1;

    for(int i=0;i<16;i++) {
        if(test[i]!=255) {
            checkTest = 0;
        }
    }

    printResultTest("0 < 255 should return 11111111*16", checkTest);

    a = init_vuint8(0);
    b = init_vuint8(129);

    _mm_store_si128(res,_mm_cmplt_epu8(a,b));

    test = (uint8*)res;

    checkTest = 1;

    for(int i=0;i<16;i++) {
        if(test[i]!=255) {
            checkTest = 0;
        }
    }

    printResultTest("0 < 129 should return 11111111*16", checkTest);

    a = init_vuint8(129);
    b = init_vuint8(0);

    _mm_store_si128(res,_mm_cmplt_epu8(a,b));

    test = (uint8*)res;

    checkTest = 1;

    for(int i=0;i<16;i++) {
        if(test[i]!=0) {
            checkTest = 0;
        }
    }

    printResultTest("129 < 0 should return 00000000*16", checkTest);

    a = init_vuint8(127);
    b = init_vuint8(0);

    _mm_store_si128(res,_mm_cmplt_epu8(a,b));

    test = (uint8*)res;

    checkTest = 1;

    for(int i=0;i<16;i++) {
        if(test[i]!=0) {
            checkTest = 0;
        }
    }

    printResultTest("127 < 0 should return 00000000*16", checkTest);

    a = init_vuint8(0);
    b = init_vuint8(127);

    _mm_store_si128(res,_mm_cmplt_epu8(a,b));

    test = (uint8*)res;

    checkTest = 1;

    for(int i=0;i<16;i++) {
        if(test[i]!=255) {
            checkTest = 0;
        }
    }

    printResultTest("0 < 127 should return 11111111*16", checkTest);


}


// void test_dilatation()
// {
//   vuint8**img = vui8matrix(0, 10, 0, 10);

//   for(int i = 0; i < 10; i++)
//     {
//       for(int j = 0; j < 10; j++)
// 	{
// 	  if(i == 5 && j == 5)
// 	    img[i][j] = init_vuint8((uint8)1);
// 	  else
// 	    img[i][j] = init_vuint8((uint8)0);
// 	}
//     }

//   display_vui8matrix (img, 0, 10, 0, 10, "%d ", "IMG :");
//   printf("===================================\n");
//   vuint8**M = dilatation_SSE(img,0, 10, 0, 10, 3);
//   display_vui8matrix (M, 0, 10, 0, 10, "%d ", "M DILATE :");
// }


//Fais un décalage de imm bit, et complète avec les bits précédents
void test_shift_right_add_prec_si128() {
    printf("Test shift right add prec:\n");
    vuint8 a = init_vuint8(200);
    vuint8 b = init_vuint8(100);
    vuint8 c[1]; 
    uint8* test = (uint8*)c;
    c[0]= shift_right_add_prec_si128(a,b,2);

    int checkTest = 1;
    if(test[0] != 100) {
        checkTest = 0;
    } else if (test[1] != 100) {
        checkTest = 0;
    } else {
        for(int i=2;i<16;i++) {
            if(test[i]!=200) {
                checkTest = 0;
            }
        }
    }

    printResultTest("Testing if (200*16),(100*16),2 return (100*2,200*14)", checkTest);

}

void test_shift_left_add_next_si128() {
    printf("Test shift left add next:\n");
    vuint8 a = init_vuint8(200);
    vuint8 b = init_vuint8(100);
    vuint8 c[1]; 
    uint8* test = (uint8*)c;
    c[0]= shift_left_add_next_si128(a,b,2);

    int checkTest = 1;
    if(test[15] != 100) {
        checkTest = 0;
    } else if (test[14] != 100) {
        checkTest = 0;
    } else {
        for(int i=0;i<14;i++) {
            if(test[i]!=200) {
                checkTest = 0;
            }
        }
    }

    printResultTest("Testing if (200*16),(100*16),2 return (100*2,200*14)", checkTest);

}


int main(){
    test_sub_abs();
    test_sel_si128();
    test_lt();
    test_shift_right_add_prec_si128();
    test_shift_left_add_next_si128();

    
    return 0;
}
