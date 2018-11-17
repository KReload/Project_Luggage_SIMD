#include "../include/testutil.h"
#include "../include/simdutil.h"


void testFDSSE2() {
    printf("\nTesting frame difference in SSE2\n");
    vuint8 theta = init_vuint8(10);
    vuint8 low = init_vuint8(0);
    vuint8 high = init_vuint8(255);
    vuint8 INow = init_vuint8(50);
    vuint8 IPrec = init_vuint8(60);
    vuint8 res[1];

    uint8* test  = (uint8*)res; 

    int checkTest = 1;

    vuint8 tempO = sub_abs_epi8(INow,IPrec);
    vuint8 c = _mm_cmplt_epu8(theta,tempO);

    res[0] = sel_si128(c,high,low);
    for(int i=0;i < N_OCTET;i++) {
        if(test[i]!=0) {
            checkTest = 0;
        }
    }
    printResultTest("it should store the right value in E (thetha:10*16 res: 10*16, E[i][j] = 0", checkTest);

    checkTest = 1;
    //======================================
    
    INow = init_vuint8(50);
    IPrec = init_vuint8(59);
    
    tempO = sub_abs_epi8(INow, IPrec);
    c = _mm_cmplt_epu8(theta,tempO);
    res[0] = sel_si128(c,high,low);

    for(int i=0;i < N_OCTET;i++) {
        if(test[i]!=0) {
            checkTest = 0;
        }
    }
    printResultTest("it should store the right value in E (thetha:10*16 res: 9*16, E[i][j] = 0*16", checkTest);
    checkTest = 1;
    //======================================
    
    INow = init_vuint8(50);
    IPrec = init_vuint8(61);
    
    tempO = sub_abs_epi8(INow, IPrec);
    c = _mm_cmplt_epu8(theta,tempO);
    res[0] = sel_si128(c,high,low);

    for(int i=0;i < N_OCTET;i++) {
        if(test[i]!=255) {
            checkTest = 0;
        }
    }
    printResultTest("it should store the right value in E (thetha:10*16 res: 11*16, E[i][j] = 255*16", checkTest);
    checkTest = 1;
    //======================================
    
    INow = _mm_set_epi8(50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50);
    IPrec = _mm_set_epi8(61,60,61,60,61,60,61,60,61,60,61,60,61,60,61,60);
    
    tempO = sub_abs_epi8(INow, IPrec);
    c = _mm_cmplt_epu8(theta,tempO);
    res[0] = sel_si128(c,high,low);

    for(int i=0;i < N_OCTET;i++) {
        if(i%2==0 && test[i]!=0) {
            checkTest = 0;
        } else if (i%2==1 && test[i]!=255) {
            checkTest = 0;
        }
    }
    printResultTest("it should store the right value in E (thetha:10*16 res: [11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10], E[i][j] = [255 0 255 0 255 0 255 0 255 0 255 0 255 0 255 0]", checkTest);

}

void testSDSSE2_step1() {
    printf("\nTesting sigma delta in SSE2 - step 1\n");
    int checkTest = 1;
    vuint8 INow = _mm_set_epi8(50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50);
    vuint8 MPrec = _mm_set_epi8(40,50,60,40,50,60,40,50,60,40,50,60,40,50,60,40);

    vuint8 c = _mm_cmplt_epu8(MPrec,INow);
    vuint8 d = _mm_cmplt_epu8(INow,MPrec);
    vuint8 k = _mm_adds_epu8(MPrec, init_vuint8(1));
    vuint8 l = _mm_subs_epu8(MPrec, init_vuint8(1));

    vuint8 res[1];
    uint8* test = (uint8*)res;
    //Condition c M0 > I1, store le résultat k sinon store le résultat de la condition d M0 < I1 qui renvoie l sinon renvoie M0[i][j]
    res[0] = sel_si128(d,l,MPrec);
    res[0] = sel_si128(c,k,res[0]);
    for(int i =0; i<N_OCTET;i+=3) {
        if(test[i]!=41) {
            checkTest = 0;
        }
    }
    for(int i =1; i<N_OCTET;i+=3) {
        if(test[i]!=59) {
            printf("%d ",test[i]);
            checkTest = 0;
        }
    }
    for(int i =2; i<N_OCTET;i+=3) {
        if(test[i]!=50) {
            checkTest = 0;
        }
    }
    printResultTest("it should store the right value in M (M[i][j] = [41 50 59 41 50 59 41 50 59 41 50 59 41 50 59 41]", checkTest);
}

void testSDSSE2_step2() {
    printf("\nTesting sigma delta in SSE2 - step 2\n");
    int checkTest = 1;
    vuint8 MNow = _mm_set_epi8(50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50);
    vuint8 INow = _mm_set_epi8(40,50,60,40,50,60,40,50,60,40,50,60,40,50,60,40);

    vuint8 res[1];
    uint8* test = (uint8*)res;
    //Condition c M0 > I1, store le résultat k sinon store le résultat de la condition d M0 < I1 qui renvoie l sinon renvoie M0[i][j]
    res[0] = sub_abs_epi8(MNow,INow);
    for(int i =0; i<N_OCTET;i+=3) {
        if(test[i]!=10) {
            checkTest = 0;
        }
    }
    for(int i =1; i<N_OCTET;i+=3) {
        if(test[i]!=10) {
            printf("%d ",test[i]);
            checkTest = 0;
        }
    }
    for(int i =2; i<N_OCTET;i+=3) {
        if(test[i]!=0) {
            checkTest = 0;
        }
    }

    printResultTest("it should store the right value in O (O[i][j] = [10 0 10 10 0 10 10 0 10 10 0 10 10 0 10 10]", checkTest);

}

void testSDSSE2_step3() {
    printf("\nTesting sigma delta in SSE2 - step 3\n");
    int checkTest = 1;
    int N = 4;
    vuint8 c,d,k,l,NmulOt, ONow, VPrec,VNow;
    vuint8 res[1];
    uint8* test = (uint8*)res;

    ONow = _mm_set_epi8(40,50,40,50,40,50,40,50,40,50,40,50,40,50,40,50);
    NmulOt = init_vuint8(0);
    for(int k = 0;k<N;k++) {
        NmulOt = _mm_adds_epu8(NmulOt,ONow);
    }
    res[0] = NmulOt;
    for(int i=0; i<N_OCTET;i++) {
        if(i%2==0 && test[i]!=200){
            checkTest = 0;
        } else if((i+1)%2==1 && test[i+1]!=160 && (i+1)<N_OCTET){
            checkTest = 0;
        }
    }
    printResultTest("N*Ot[i][j] should return the right value : [160 200 160 200 160 200 160 200 160 200 160 200 160 200 160 200]", checkTest);
    checkTest = 1;
    //=============================================
    ONow = _mm_set_epi8(100,50,100,50,100,50,100,50,100,50,100,50,100,50,100,50);
    NmulOt = init_vuint8(0);
    for(int k = 0;k<N;k++) {
        NmulOt = _mm_adds_epu8(NmulOt,ONow);
    }
    res[0] = NmulOt;
    for(int i=0; i<N_OCTET;i++) {
        if(i%2==0 && test[i]!=200){
            checkTest = 0;
        } else if((i+1)%2==1 && test[i+1]!=255 && (i+1)<N_OCTET){
            checkTest = 0;
        }
    }
    printResultTest("N*Ot[i][j] should return the right value : [255 200 255 200 255 200 255 200 255 200 255 200 255 200 255 200]", checkTest);
    checkTest = 1;
    //=============================================
                        //255 200 255 200 255 200 255 200 255 200 255 200 255 200 255 200
    VPrec = _mm_set_epi8(200,254,190,200,254,190,200,254,190,200,254,190,200,254,190,200);

    c = _mm_cmplt_epu8(VPrec,NmulOt);    
    d = _mm_cmplt_epu8(NmulOt,VPrec);
    k = _mm_adds_epu8(VPrec, init_vuint8(1));
    l = _mm_subs_epu8(VPrec, init_vuint8(1));
    //Selection
    VNow = sel_si128(c,k,sel_si128(d,l,VPrec));
    
    //Clamping
    VNow = _mm_max_epu8(_mm_min_epu8(VNow, init_vuint8(254)), init_vuint8(1));
    res[0] = _mm_cmpeq_epi8(VNow, _mm_set_epi8(201,253,191,200,254,191,201,253,191,200,254,191,201,253,191,200));

    for(int i = 0;i<N_OCTET;i++) {
        if(test[i]!=255) {
            printf("%d %d\n", test[i], i);
            checkTest = 0;
        }
    }

    printResultTest("it should store the right value in V1: [201,191,254,200,191,253,201,191,254,200,191,253,201,191,254,200]",checkTest);

}

void testSDSSE2_step4() {
    printf("\nTesting sigma delta in SSE2 - step 4\n");
    int checkTest = 1;
    vuint8 c, k, l;
    vuint8 VNow = _mm_set_epi8(201,253,191,200,254,191,201,253,191,200,254,191,201,253,191,200);
    vuint8 ONow = _mm_set_epi8(254,200,254,200,254,200,254,200,254,200,254,200,254,200,254,200);

    vuint8 res[1];
    uint8* test = (uint8*)res;

    c = _mm_cmplt_epu8(ONow,VNow);
    
    k = init_vuint8(0);
    l = init_vuint8(255);

    res[0]= sel_si128(c,k,l);
    res[0]= _mm_cmpeq_epi8(res[0], _mm_set_epi8(255,0,255,255,255,255,255,0,255,255,255,255,255,0,255,255));

    for(int i=0;i<N_OCTET;i++) {
        if(test[i]!=255){
            checkTest = 0;
        }
    }
    printResultTest("it should return the right value in E : [255 255 0 255 255 255 255 255 0 255 255 255 255 255 0 255]", checkTest);
}

int main() {
    testFDSSE2();
    testSDSSE2_step1();
    testSDSSE2_step2();
    testSDSSE2_step3();
    testSDSSE2_step4();
    return 0;
}
