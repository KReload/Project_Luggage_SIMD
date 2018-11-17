#include "../include/testutil.h"
#include "../include/mouvement.h"

void testFD() {
    printf("\n Testing frame difference \n");
    int theta = 10;
    int checkTest = 1;

    uint8** I0 = (uint8**)malloc(sizeof(uint8*)*5);
    uint8** I1 = (uint8**)malloc(sizeof(uint8*)*5);
    uint8** O = (uint8**)malloc(sizeof(uint8*)*5);
    uint8** E = (uint8**)malloc(sizeof(uint8*)*5);
    for(int i = 0;i<5;i++) {
        I0[i] = (uint8*)malloc(sizeof(uint8)*5);
        I1[i] = (uint8*)malloc(sizeof(uint8)*5);
        O[i] = (uint8*)malloc(sizeof(uint8)*5);
        E[i] = (uint8*)malloc(sizeof(uint8)*5);

        for(int j=0;j<5;j++) {
            I0[i][j] = i;
            I1[i][j] = 4 - i;
        }
    }
    frameDifference(I0, I1, O, E, 0, 5, 0, 5,(uint8)theta);
    for(int i = 0; i<5 ;i++) {
        for(int j=0;j<5;j++) {
            if(O[i][j]!=4 && i == 0) {
                //|4 - 0|
                printf("%d cond1", O[i][j]);
                checkTest = 0;
            } else if(O[i][j]!=2 && i == 1) {
                //|3 - 1|
                printf("%d cond2", O[i][j]);
                checkTest = 0;
            } else if(O[i][j]!=0 && i == 2) {
                //|2 - 2|
                printf("%d cond3", O[i][j]);
                checkTest = 0;
            } else if(O[i][j]!=2 && i == 3) {
                //|1 - 3|
                printf("%d cond4", O[i][j]);
                checkTest = 0;
            } else if(O[i][j]!=4 && i == 4) {
                //|0 - 4|
                printf("%d cond5", O[i][j]);
                checkTest = 0;
            }
        }
    }
    printResultTest("it should return the right O matrix 4 2 0 2 4 for each column", checkTest);
    checkTest = 1;
    for(int i = 0; i<5 ;i++) {
        for(int j=0;j<5;j++) {
            if(E[i][j]!= 0) {
                checkTest = 0;
            } 
        }
    }
    printResultTest("it should return the right E matrix 0 0 0 0 0 for each column", checkTest);
    checkTest = 1;

    for(int i = 0;i<5;i++) {
        for(int j=0;j<5;j++) {
            I0[i][j] = i;
            I1[i][j] = 4 - i + 9;
        }
    }
    frameDifference(I0, I1, O, E, 0, 5, 0, 5,(uint8)theta);
    for(int i = 0; i<5 ;i++) {
        for(int j=0;j<5;j++) {
            if(E[i][j]!=255 && i == 0) {
                //|13 - 0|
                printf("%d cond1", O[i][j]);
                checkTest = 0;
            } else if(E[i][j]!=255 && i == 1) {
                //|12 - 1|
                printf("%d cond2", O[i][j]);
                checkTest = 0;
            } else if(E[i][j]!=0 && i == 2) {
                //|11 - 2|
                printf("%d cond3", O[i][j]);
                checkTest = 0;
            } else if(E[i][j]!=0 && i == 3) {
                //|10 - 4|
                printf("%d cond4", O[i][j]);
                checkTest = 0;
            } else if(E[i][j]!=0 && i == 4) {
                //|9 - 5|
                printf("%d cond5", O[i][j]);
                checkTest = 0;
            }
        }
    }
    printResultTest("it should return the right E matrix 255 255 0 0 0 for each column", checkTest);
    checkTest = 1;

    for(int i = 0;i<5;i++) {
        free(I0[i]);
        free(I1[i]);
        free(E[i]);
        free(O[i]);
    }
    free(I0);
    free(I1);
    free(E);
    free(O);
}

uint8 sd_step1(uint8 MPrec, uint8 INow) {
    uint8 MNow;
    if(MPrec < INow)
	    MNow = MPrec + 1;
	else if(MPrec > INow)
	    MNow = MPrec - 1;
	else
	    MNow = MPrec;
    return MNow;
}

uint8 sd_step2(uint8 MNow, uint8 INow) {
    return abs(MNow - INow);
}

uint8 sd_step3(uint8 VPrec,uint8 ONow) {
    uint8 res;
    if(VPrec < N*ONow)
        res = VPrec + 1;
    else if(VPrec > N*ONow)
	    res= VPrec - 1;
    else
	    res= VPrec;
    res= MAX(MIN(res,VMAX),VMIN); //Clamp
    return res;
}

uint8 sd_step4(uint8 ONow, uint8 VNow){
    if(ONow < VNow)
	    return 0;
	  else
	    return 255; 
}

void testSD(){
    printf("\n Testing sigma delta step 1\n");
    
    int MPrec = 5;
    int INow = 4;
    int checkTest = 1;
    
    if(sd_step1(MPrec, INow)!=4) {
        checkTest = 0;
    }
    printResultTest("M1 should be equal to 4 for M0 = 5 and I1 = 4", checkTest);
    checkTest = 1;
    if(sd_step1(4, 6)!=5) {
        checkTest = 0;
    }
    printResultTest("M1 should be equal to 5 for M0 = 4 and I1 = 6", checkTest);
    checkTest = 1;
    if(sd_step1(4, 4)!=4) {
        checkTest = 0;
    }
    printResultTest("M1 should be equal to 4 for M1 = 4 and I1 = 4", checkTest);
    checkTest = 1;
	
    printf("\n Testing sigma delta step 2\n");

    if(sd_step2(4, 4)!=0) {
        checkTest = 0;
    }
    printResultTest("O1 should be equal to 0 for M1 = 4 and I1 = 4", checkTest);
    checkTest = 1;

    if(sd_step2(4, 2)!=2) {
        checkTest = 0;
    }
    printResultTest("O1 should be equal to 0 for M1 = 4 and I1 = 2", checkTest);
    checkTest = 1;

    if(sd_step2(2, 4)!=2) {
        checkTest = 0;
    }
    printResultTest("O1 should be equal to 0 for M1 = 2 and I1 = 4", checkTest);
    checkTest = 1;

    printf("\n Testing sigma delta step 3\n");
    if(sd_step3(4, 2)!=4) {
        checkTest = 0;
    }
    printResultTest("V1 should be equal to 4 for V0 = 4 and O1 = 2", checkTest);
    checkTest = 1;
    if(sd_step3(4, 3)!=5) {
        checkTest = 0;
    }
    printResultTest("V1 should be equal to 5 for V0 = 4 and O1 = 3", checkTest);
    checkTest = 1;
    if(sd_step3(4, 0)!=3) {
        checkTest = 0;
    }
    printResultTest("V1 should be equal to 3 for V0 = 4 and O1 = 0", checkTest);
    checkTest = 1;
    if(sd_step3(1, 0)!=1) {
        checkTest = 0;
    }
    printResultTest("V1 should be equal to 1 for V0 = 1 and O1 = 0", checkTest);
    checkTest = 1;
    if(sd_step3(254, 255)!=254) {
        checkTest = 0;
    }
    printResultTest("V1 should be equal to 254 for V0 = 254 and O1 = 255", checkTest);
    checkTest = 1;
    printf("\n Testing sigma delta step 4\n");
    //sd_step4(uint8 ONow, uint8 VNow)
    if(sd_step4(20, 30)!=0) {
        checkTest = 0;
    }
    printResultTest("E1 should be equal to 0 for O1 = 20 and V1 = 30", checkTest);
    checkTest = 1;
    if(sd_step4(30, 20)!=255) {
        checkTest = 0;
    }
    printResultTest("E1 should be equal to 255 for O1 = 30 and V1 = 20", checkTest);
    checkTest = 1;
    if(sd_step4(30, 30)!=255) {
        checkTest = 0;
    }
    printResultTest("E1 should be equal to 255 for O1 = 30 and V1 = 30", checkTest);
    checkTest = 1;



//   //Step 2 : O1 Computation
//   

//   //Step 3 : V1 Update and Clamping
//   for(int i = 0; i < nrh; i++)
//     {
//       for(int j = 0; j < nch; j++)
// 	{
// 	  
// 	}
//     }

//   //Step 4 : E1 Estimation
//   for(int i = 0; i < nrh; i++)
//     {
//       for(int j = 0; j < nch; j++)
// 	{
// 	  
// 	}
//     }

}

int main() {
    testFD();
    testSD();
}