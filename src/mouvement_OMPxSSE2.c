

#include "../include/mouvement_OMPxSSE2.h"

#define FILENAMESIZE 40
#define NIMAGES 300

#define THETA 6
#define N 2

#define VMAX 254
#define VMIN 1

void** frameDifference(vuint8** I0, vuint8** I1, vuint8**O, vuint8**E, long nrl,long nrh,long ncl, long nch)
{
  
  vuint8 tempO;
  vuint8 tempE;
  vuint8 IPrec;
  vuint8 INow;
  //vecteur 16x8 bits chaque octet contient THETA
  vuint8 theta = init_vuint8(THETA);
  vuint8 low = init_vuint8(0);
  vuint8 high = init_vuint8(255);
  vuint8 c,d,ab,ba;
#pragma omp parallel for schedule(dynamic, CHUNK) private(INow,IPrec,tempO,c,d)
  for(int i = 0; i < nrh; i++)
    {
      for(int j = 0; j < nch; j++)
      {
        INow = _mm_load_si128(&I1[i][j]);
        IPrec = _mm_load_si128(&I0[i][j]);
        //|I0 - I1|
        tempO = sub_abs_epi8(INow,IPrec);
        
        _mm_store_si128(&O[i][j], tempO);
        //Theta < O
        c = _mm_cmplt_epu8(tempO,theta);

        d = sel_si128(c,low,high);

        _mm_store_si128(&E[i][j],d);

      }
    }
  
}


vuint8** sigmaDelta(vuint8** I0, vuint8** I1, vuint8** M0, vuint8** M1, vuint8** V0, vuint8** V1, vuint8** O1, vuint8** E1, long nrl,long nrh, long ncl, long nch)
{

  vuint8 NmulOt = init_vuint8(0);

  vuint8 MNow, INow, VNow,MPrec, IPrec, VPrec, ONow;


  vuint8 c,d,a,b,k,l;
#pragma omp parallel for schedule(dynamic, CHUNK) private(NmulOt,MNow, INow, VNow,MPrec, IPrec, VPrec, ONow,c,d,a,b,k,l)
  for(int i = 0; i < nrh; i++)
    {
      for(int j = 0; j < nch; j++)
      {
  
        MPrec = _mm_load_si128(&M0[i][j]);
        VPrec = _mm_load_si128(&V0[i][j]);
        INow = _mm_load_si128(&I1[i][j]);

        NmulOt = init_vuint8(0);

  //Step 1 : M1 estimation
        //macro compare less than pour unsigned
        c = _mm_cmplt_epu8(MPrec,INow);
        d = _mm_cmplt_epu8(INow,MPrec);

        k = _mm_adds_epu8(MPrec, init_vuint8(1));
        l = _mm_subs_epu8(MPrec, init_vuint8(1));

        //Condition c M0 > I1, store le résultat k sinon store le résultat de la condition d M0 < I1 qui renvoie l sinon renvoie M0[i][j]
        MNow = sel_si128(c,k,sel_si128(d,l,MPrec));

        _mm_store_si128(&M1[i][j], MNow);

  //Step 2 : O1 Computation

        // Ot = |Mt - It|
        ONow = sub_abs_epi8(MNow,INow);

	      _mm_store_si128(&O1[i][j], ONow);


  //Step 3 : V1 Update and Clamping

        for(int k = 0;k<N;k++) {
          NmulOt = _mm_adds_epu8(NmulOt,ONow);
        }

        c = _mm_cmplt_epu8(VPrec,NmulOt);
        
        d = _mm_cmplt_epu8(NmulOt,VPrec);
  
        k = _mm_adds_epu8(VPrec, init_vuint8(1));
        l = _mm_subs_epu8(VPrec, init_vuint8(1));

        //Selection
        VNow = sel_si128(c,k,sel_si128(d,l,VPrec));
        
        //Clamping
        VNow = _mm_max_epu8(_mm_min_epu8(VNow, init_vuint8(VMAX)), init_vuint8(VMIN));
        
        //On store le clamping
        _mm_store_si128(&V1[i][j],VNow);
        

    //Step 4 : E1 Estimation

        c = _mm_cmplt_epu8(ONow,VNow);
        
        k = init_vuint8(0);
        l = init_vuint8(255);

        _mm_store_si128(&E1[i][j], sel_si128(c,k,l));
      }
    }

  

  return E1;

}
