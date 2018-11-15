

#include "include/mouvement_SSE2.h"

#define FILENAMESIZE 40
#define NIMAGES 300

#define THETA 10
#define N 4

#define VMAX 255
#define VMIN 0

/*vuint8 sub_abs_epi8(vuint8 a, vuint8 b){
  return _mm_or_si128(_mm_subs_epu8((__m128i)a,(__m128i)b),_mm_subs_epu8((__m128i)b,(__m128i)a));
}

vuint8 sel_si128(vuint8 vcontrole,vuint8 a, vuint8 b){
  return _mm_or_si128(_mm_and_si128((__m128i)vcontrole,(__m128i)a),_mm_andnot_si128((__m128i)vcontrole,(__m128i)b));
}
*/
vuint8** frameDifference(vuint8** I0, vuint8** I1, long* nrl,long* nrh,long* ncl, long* nch)
{
  
  vuint8** O = vui8matrix(*nrl, *nrh, *ncl, *nch);
  vuint8** E = vui8matrix(*nrl, *nrh, *ncl, *nch);

  //vecteur 16x8 bits chaque octet contient THETA
  vuint8 theta = init_vuint8((uint8)THETA);
  vuint8 low = init_vuint8((uint8)0);
  vuint8 high = init_vuint8((uint8)255);
  vuint8 c,d,ab,ba;
  
  for(int i = 0; i < *nrh; i++)
    {
      for(int j = 0; j < *nch; j++)
      {
        O[i][j] = sub_abs_epi8(I0[i][j],I1[i][j]);
        
        c = (vuint8)_mm_cmplt_epu8((__m128i)theta,(__m128i)O[i][j]);

        d = sel_si128(c,high,low);

        _mm_store_si128((__m128i*)&E[i][j],d);

      }
    }
  
  return E;
}


vuint8** sigmaDelta(vuint8** I0, vuint8** I1, vuint8** M0, vuint8** M1, vuint8** V0, vuint8** V1, vuint8** O1, vuint8** E1, long nrl,long nrh, long ncl, long nch)
{

  vuint8 NmulOtHi = init_vuint8(0);
  vuint8 NmulOtLo = init_vuint8(0);
  vuint8 NmulOt = init_vuint8(0);

  vuint8 MNow, INow, VNow,MPrec, IPrec, VPrec, ONow;


  vuint8 c,d,a,b,k,l;
  //Step 1 : M1 estimation
  for(int i = 0; i < nrh; i++)
    {
      for(int j = 0; j < nch; j++)
      {
        MPrec = _mm_load_si128(&M0[i][j]);
        VPrec = _mm_load_si128(&V0[i][j]);
        INow = _mm_load_si128(&I1[i][j]);

        NmulOt = init_vuint8(0);

        c = _mm_cmplt_epu8(MPrec,INow);
        d = _mm_cmplt_epu8(INow,MPrec);

        k = _mm_adds_epu8(MPrec, init_vuint8(1));
        l = _mm_subs_epu8(MPrec, init_vuint8(1));
        //Condition c M0 > I1, store le résultat k sinon store le résultat de la condition d M0 < I1 qui renvoie l sinon renvoie M0[i][j]

        _mm_store_si128(&M1[i][j],sel_si128(c,k,sel_si128(d,l,M0[i][j])));


  //Step 2 : O1 Computation
        ONow = sub_abs_epi8(M1[i][j],I1[i][j]);

	      _mm_store_si128(&O1[i][j], ONow);


  //Step 3 : V1 Update and Clamping
        // NmulOtHi = init_vuint8(0);
        // NmulOtLo = init_vuint8(0);
        // for(int k =0;k< N;k++) {
        //   NmulOtHi = _mm_adds_epu16(NmulOtHi, _mm_unpackhi_epi8(O1[i][j], init_vuint8(0)));
        //   NmulOtLo = _mm_adds_epu16(NmulOtLo, _mm_unpacklo_epi8(O1[i][j], init_vuint8(0)));
        // }

        //display_vuint8(ONow, "%d ", "\nOnow:");
        for(int k = 0;k<N;k++) {
          NmulOt = _mm_adds_epu8(NmulOt,ONow);
        }
        //display_vuint8(NmulOt, "%d ", "\nMul:");
        //Unpack, fait la comparaison et pack le résultat fini par un clamp
        // c = _mm_cmplt_epu8to16(V0[i][j], NmulOtHi,NmulOtLo);
        // d = _mm_cmpgt_epu8to16(V0[i][j], NmulOtHi,NmulOtLo);

        c = _mm_cmplt_epu8(VPrec,NmulOt);
        
        d = _mm_cmplt_epu8(NmulOt,VPrec);
  
        k = _mm_adds_epu8(VPrec, init_vuint8((uint8)1));
        l = _mm_subs_epu8(VPrec, init_vuint8((uint8)1));

        VNow = sel_si128(c,k,sel_si128(d,l,VPrec));
        
        //display_vuint8(V1[i][j], "%d ", "\nvalue after: ");
        
        _mm_store_si128((__m128i*)&V1[i][j],
          _mm_max_epu8(_mm_min_epu8(VNow, init_vuint8((uint8)VMAX)), init_vuint8((uint8)VMIN)));
        

    //Step 4 : E1 Estimation

        c = _mm_cmplt_epu8(ONow,VNow);
        

        k = init_vuint8((uint8)0);
        l = init_vuint8((uint8)255);

        _mm_store_si128((__m128i*)&E1[i][j], sel_si128(c,k,l));
      }
    }

  return E1;

}


/*
int main(void)
{
  long nrl, nrh, ncl,nch;
  //vuint8**img = LoadPGM_vui8matrix("./hall/hall000000.pgm",&nrl,&nrh,&ncl,&nch);
  //vuint8**img2 = LoadPGM_vui8matrix("./hall/hall000000.pgm",&nrl,&nrh,&ncl,&nch);

  char* filename0 = (char*) malloc(sizeof(char) * FILENAMESIZE);
  char* filename1 = (char*) malloc(sizeof(char) * FILENAMESIZE);
  char* filename2 = (char*) malloc(sizeof(char) * FILENAMESIZE);

  for(int i = 0; i < NIMAGES - 1; i++)
  {
    
    sprintf(filename0,"./hall/hall%06d.pgm",i);
    sprintf(filename1,"./hall/hall%06d.pgm",i+1);
    sprintf(filename2,"./hallSSE/hallSIMD_FD%06d.pgm",i+1);
    vuint8** I0 = LoadPGM_vui8matrix(filename0, &nrl, &nrh, &ncl, &nch);
    vuint8** I1 = LoadPGM_vui8matrix(filename1, &nrl, &nrh, &ncl, &nch);
    
    //vuint8**E = frameDifference(I0, I1, &nrl,&nrh,&ncl,&nch);
    vuint8**E = sigmaDelta(I0, I1, nrl,nrh,ncl,nch);
      
    SavePGM_vui8matrix(E, nrl, nrh, ncl, nch, filename2);

    
    
  }
  

  free(filename0);
  free(filename1);
  free(filename2);
  return 0;

}*/
