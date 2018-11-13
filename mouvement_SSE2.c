

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
        
        c = (vuint8)_mm_cmplt_epi8((__m128i)theta,(__m128i)O[i][j]);

        d = sel_si128(c,high,low);

        _mm_store_si128((__m128i*)&E[i][j],d);

      }
    }
  
  return E;
}


vuint8** sigmaDelta(vuint8** I0, vuint8** I1, long nrl,long nrh, long ncl, long nch)
{
  vuint8** M0 = I0;
  vuint8** M1 = vui8matrix(nrl,nrh,ncl,nch);
  vuint8** V0 = vui8matrix(nrl,nrh,ncl,nch);
  vuint8** V1 = vui8matrix(nrl,nrh,ncl,nch);
  vuint8** O1 = vui8matrix(nrl,nrh,ncl,nch);
  vuint8** E1 = vui8matrix(nrl,nrh,ncl,nch);

  vuint8 NmulOt;
  vuint8 c,d,a,b,k,l;
  //Step 1 : M1 estimation
  for(int i = 0; i < nrh; i++)
    {
      for(int j = 0; j < nch; j++)
      {

        c = _mm_cmplt_epi8(M0[i][j],I1[i][j]);
        d = _mm_cmplt_epi8(I1[i][j],M0[i][j]);

        k = _mm_add_epi8(M0[i][j], (__m128i)init_vuint8((uint8)1));
        l = _mm_sub_epi8(M0[i][j], (__m128i)init_vuint8((uint8)1));
        //Condition c M0 > I1, store le résultat k sinon store le résultat de la condition d M0 < I1 qui renvoie l sinon renvoie M0[i][j]

        _mm_store_si128((__m128i*)&M1[i][j],
          sel_si128(c,k,sel_si128(d,l,M0[i][j]))
        );
      }
    }

  //Step 2 : O1 Computation
  for(int i = 0; i < nrh; i++)
    {
      for(int j = 0; j < nch; j++)
	      O1[i][j] = sub_abs_epi8(M1[i][j],I1[i][j]);
    }

  //Step 3 : V1 Update and Clamping
  for(int i = 0; i < nrh; i++)
  {
    for(int j = 0; j < nch; j++)
    {
      for(int k =0;k< N;k++) {
        NmulOt = _mm_adds_epu8(NmulOt, O1[i][j]);
      }
      c = _mm_cmplt_epi8(V0[i][j],NmulOt);
      d = _mm_cmplt_epi8(NmulOt,V0[i][j]);

      k = _mm_add_epi8(V0[i][j], (__m128i)init_vuint8((uint8)1));
      l = _mm_sub_epi8(V0[i][j], (__m128i)init_vuint8((uint8)1));

      _mm_store_si128((__m128i*)&V1[i][j],
          sel_si128(c,k,sel_si128(d,l,V0[i][j]))
      );

      _mm_store_si128((__m128i*)&V1[i][j],
        _mm_max_epu8(_mm_min_epu8(V1[i][j], init_vuint8((uint8)VMAX)), init_vuint8((uint8)VMIN))
      );
    /*if(V0[i][j] < N*O1[i][j]) 
      V1[i][j] = V0[i][j] + 1;
    else if(V0[i][j] > N*O1[i][j])
      v1[i][j] = V0[i][j] - 1;
    else
      V1[i][j] = V0[i][j];
    V1[i][j] = MAX(MIN(V1[i][j],VMAX),VMIN); //Clamp*/
    }
  }

  //Step 4 : E1 Estimation

  for(int i = 0; i < nrh; i++)
  {
    for(int j = 0; j < nch; j++)
    {
      c = _mm_cmplt_epi8(O1[i][j],V1[i][j]);

      k = init_vuint8((uint8)0);
      l = init_vuint8((uint8)255);

      _mm_store_si128((__m128i*)&E1[i][j],
          sel_si128(c,k,l)
      );
    }
  }
  /*for(int i = 0; i < nrh; i++)
    {
      for(int j = 0; j < nch; j++)
	{
	  if(O1[i][j] < V1[i][j])
	    E1[i][j] = 0;
	  else
	    E1[i][j] = 1; //ou 255
	}
  
    }
  */

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
