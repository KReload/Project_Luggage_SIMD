#include "../include/bench_SSE2.h"

#define FILENAMESIZE 27
#define NIMAGES 300

void calculMatriceROC(float64** ROC, uint8** img, uint8** img_vt, long nrl, long nrh, long ncl, long nch)
{
  double vp = 0.0, vn = 0.0, fp = 0.0, fn = 0.0;
  for(int i = 0; i < nrh; i++)
    {
      for(int j = 0; j < nch; j++)
	{
	  if(img[i][j] == img_vt[i][j])
	    {
	      if(img_vt[i][j] == 0)
		vn++;
	      else
		vp++;
	    }
	  else
	    {
	      if(img_vt[i][j] == 255)
		fn++;
	      else
		fp++;
	    }
	}
    }

  ROC[0][0] += (float64) vp;
  ROC[0][1] += (float64) fn;
  ROC[1][0] += (float64) fp;
  ROC[1][1] += (float64) vn;
}

double calculMCC(float64** ROC)
{
  double mcc;
  double vp = ROC[0][0];
  double fn = ROC[0][1];
  double fp = ROC[1][0];
  double vn = ROC[1][1];
  
  mcc = (vp*vn - fp*fn) / sqrt((vp+fp)*(vp+fn)*(vn+fp)*(vn+fn));
  
  return mcc;
}

void benchDetectionMouvementFDSSE2()
{
clock_t startFD, endFD, startTot, endTot, startMorpho, endMorpho;
  double cpuTimeSD = 0.0;
  double cpuTimeTot = 0.0;
  double cpuTimeMorpho = 0.0;

  long nrl, nrh, ncl, nch;
  int dim = 3;
  char filename0[255];
  char filename1[255];
  char filenameE[255];
  char filenameO[255];
  vuint8** I0 = LoadPGM_vui8matrix("../hall/hall000000.pgm", &nrl, &nrh, &ncl, &nch);
  vuint8** I1;
  vuint8** O = vui8matrix(nrl, nrh, ncl, nch);
  vuint8** E = vui8matrix(nrl, nrh, ncl, nch);
  startTot = clock();
  for(int i = 0; i < NIMAGES - 1; i++)
    {
      
      sprintf(filename0,"../hall/hall%06d.pgm",i);
      sprintf(filename1,"../hall/hall%06d.pgm",i+1);
      I0 = LoadPGM_vui8matrix(filename0, &nrl, &nrh, &ncl, &nch);
      I1 = LoadPGM_vui8matrix(filename1, &nrl, &nrh, &ncl, &nch);

      startTot = clock();
      startFD = clock();
      frameDifference(I0,I1,O,E,nrl,nrh,ncl,nch);
      endFD = clock();
      cpuTimeSD += ((double) (endFD-startFD))/ CLOCKS_PER_SEC * 1000;
      
      startMorpho = clock();
      E = ouverture_SSE(E, nrl, nrh, ncl, nch, dim);
      E = fermeture_SSE(E, nrl, nrh, ncl, nch, dim);
      endMorpho = clock();
      endTot = clock();
      cpuTimeMorpho += ((double) (endMorpho-startMorpho))/CLOCKS_PER_SEC * 1000;
      cpuTimeTot += ((double) (endTot-startTot))/CLOCKS_PER_SEC * 1000;
            
      sprintf(filenameO,"../output/hallSSE/FDO/hall%06dO.pgm",i);
      sprintf(filenameE,"../output/hallSSE/FDE/hall%06dE.pgm",i+1);
      
      SavePGM_vui8matrix(O, nrl, nrh, ncl, nch, filenameO);
      SavePGM_vui8matrix(E, nrl, nrh, ncl, nch, filenameE);

    }
  printf("Temps passé dans l'algo FD : %f ms\n", cpuTimeSD);
  printf("Temps passé dans les algos de morphologies : %f ms\n", cpuTimeMorpho);
  printf("Temps total : %f ms\n", cpuTimeTot);

  // free_ui8matrix(O1, nrl+2, nrh+2, ncl+2, nch+2);
  // free_ui8matrix(E1, nrl+2, nrh+2, ncl+2, nch+2);
  free_vui8matrix(I0, nrl, nrh, ncl, nch);
  free_vui8matrix(I1, nrl, nrh, ncl, nch);

}
void benchDetectionMouvementSDSSE2()
{
  clock_t startSD, endSD, startTot, endTot, startMorpho, endMorpho;
  double cpuTimeSD = 0.0;
  double cpuTimeTot = 0.0;
  double cpuTimeMorpho = 0.0;

  long nrl, nrh, ncl, nch;
  int dim = 3;
  char filename0[255];
  char filename1[255];
  char filenameE[255];
  char filenameO[255];
  vuint8** I0;
  vuint8** I1;
  vuint8** M0 = LoadPGM_vui8matrix("../hall/hall000000.pgm",&nrl,&nrh,&ncl,&nch);
  vuint8** O1 = vui8matrix(nrl, nrh, ncl, nch);
  vuint8** E1 = vui8matrix(nrl, nrh, ncl, nch);
  vuint8** M1 = vui8matrix(nrl, nrh, ncl, nch);
  vuint8** V0 = vui8matrix(nrl, nrh, ncl, nch);
  vuint8** V1 = vui8matrix(nrl, nrh, ncl, nch);
  startTot = clock();
  for(int i = 0; i < NIMAGES - 1; i++)
    {
      
      sprintf(filename0,"../hall/hall%06d.pgm",i);
      sprintf(filename1,"../hall/hall%06d.pgm",i+1);
      I0 = LoadPGM_vui8matrix(filename0, &nrl, &nrh, &ncl, &nch);
      I1 = LoadPGM_vui8matrix(filename1, &nrl, &nrh, &ncl, &nch);

      startTot = clock();
      startSD = clock();
      sigmaDelta(I0, I1, M0, M1, V0, V1, O1, E1, nrl, nrh, ncl, nch);
      endSD = clock();
      cpuTimeSD += ((double) (endSD-startSD))/ CLOCKS_PER_SEC * 1000;

      startMorpho = clock();
      E1 = ouverture_SSE(E1, nrl, nrh, ncl, nch, dim);
      E1 = fermeture_SSE(E1, nrl, nrh, ncl, nch, dim);
      E1 = ouverture_SSE(E1, nrl, nrh, ncl, nch, 5);
      E1 = fermeture_SSE(E1, nrl, nrh, ncl, nch, 5);
      endMorpho = clock();
      endTot = clock();
      cpuTimeMorpho += ((double) (endMorpho-startMorpho))/CLOCKS_PER_SEC * 1000;
      cpuTimeTot += ((double) (endTot-startTot))/CLOCKS_PER_SEC * 1000;
      
      copy_vui8matrix_vui8matrix(V1, nrl, nrh, ncl, nch, V0);
      copy_vui8matrix_vui8matrix(M1, nrl, nrh, ncl, nch, M0);
      
      
      sprintf(filenameO,"../output/hallSSE/SDO/hall%06dO.pgm",i);
      sprintf(filenameE,"../output/hallSSE/SDE/hall%06dE.pgm",i+1);
      
      SavePGM_vui8matrix(O1, nrl, nrh, ncl, nch, filenameO);
      SavePGM_vui8matrix(E1, nrl, nrh, ncl, nch, filenameE);

    }
  printf("Temps passé dans l'algo SD : %f ms\n", cpuTimeSD);
  printf("Temps passé dans les algos de morphologies : %f ms\n", cpuTimeMorpho);
  printf("Temps total : %f ms\n", cpuTimeTot);

  free_vui8matrix(O1, nrl, nrh, ncl, nch);
  free_vui8matrix(E1, nrl, nrh, ncl, nch);
  free_vui8matrix(I0, nrl, nrh, ncl, nch);
  free_vui8matrix(I1, nrl, nrh, ncl, nch);
  free_vui8matrix(V0, nrl, nrh, ncl, nch);
  free_vui8matrix(V1, nrl, nrh, ncl, nch);
  free_vui8matrix(M0, nrl, nrh, ncl, nch);
  free_vui8matrix(M1, nrl, nrh, ncl, nch);

}


void benchQualitatifFDSSE2()
{
  long nrl, nrh, ncl, nch;
  
  float64** ROC = f64matrix(0, 2, 0, 2);
  ROC[0][0] = 0.0;
  ROC[0][1] = 0.0;
  ROC[1][0] = 0.0;
  ROC[1][1] = 0.0;
  uint8** IVT;
  uint8** IFD;
  char filename0[255];
  char filename1[255];
  double mcc = 0.0;
  for(int i = 12; i < NIMAGES; i+=20)
    {
      sprintf(filename0,"../IVT/hall%d_VT.pgm",i);
      sprintf(filename1,"../output/hallSSE/FDE/hall%06dE.pgm",i);

      IVT = LoadPGM_ui8matrix(filename0,&nrl,&nrh,&ncl,&nch);
      IFD = LoadPGM_ui8matrix(filename1,&nrl,&nrh,&ncl,&nch);
 
      calculMatriceROC(ROC, IFD, IVT, nrl, nrh, ncl, nch);
    }

  mcc = calculMCC(ROC);
  display_f64matrix(ROC,0,1,0,1,"%f ","Matrice ROC : ");
  printf("\nMCC = %f\n",mcc); 

  free_ui8matrix(IVT, nrl, nrh, ncl, nch);
  free_ui8matrix(IFD, nrl, nrh, ncl, nch);
  free_f64matrix(ROC, 0, 2, 0, 2);
}

void benchQualitatifSDSSE2()
{
  long nrl, nrh, ncl, nch;
  double mcc;
  float64** ROC = f64matrix(0, 2, 0, 2);
  ROC[0][0] = 0.0;
  ROC[0][1] = 0.0;
  ROC[1][0] = 0.0;
  ROC[1][1] = 0.0;
  uint8** IVT;
  uint8** ISD;
  char filename0[255];
  char filename1[255];
  for(int i = 12; i < NIMAGES; i+=20)
    {
      sprintf(filename0,"../IVT/hall%d_VT.pgm",i);
      sprintf(filename1,"../output/hallSSE/SDE/hall%06dE.pgm",i);

      IVT = LoadPGM_ui8matrix(filename0,&nrl,&nrh,&ncl,&nch);
      ISD = LoadPGM_ui8matrix(filename1,&nrl,&nrh,&ncl,&nch);

      calculMatriceROC(ROC, ISD, IVT, nrl, nrh, ncl, nch);
   }
  
  mcc = calculMCC(ROC);
  display_f64matrix(ROC,0,1,0,1,"%f ","Matrice ROC : ");
  printf("\nMCC = %f\n",mcc); 

  free_ui8matrix(IVT, nrl, nrh, ncl, nch);
  free_ui8matrix(ISD, nrl, nrh, ncl, nch);
  free_f64matrix(ROC, 0, 2, 0, 2);
}

int main(int argc, char* argv[])
{
  printf("+==========================+\nFrame Difference SSE :\n");
  benchDetectionMouvementFDSSE2();
  benchQualitatifFDSSE2();

  printf("+==========================+\nSigma Delta SSE :\n");
  benchDetectionMouvementSDSSE2();
  benchQualitatifSDSSE2();
  return 0;
}
