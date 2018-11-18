#include "../include/bench_OMP.h"

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


void benchDetectionMouvementSD()
{
  struct timespec startSD, endSD, startTot, endTot, startMorpho, endMorpho;
  double cpuTimeSD = 0.0;
  double cpuTimeTot = 0.0;
  double cpuTimeMorpho = 0.0;
  long nrl, nrh, ncl, nch;
  int dim = 3;
  char filename0[255];
  char filename1[255];
  char filenameE[255];
  char filenameO[255];
  uint8** I0;
  uint8** I1;
  uint8** M0 = LoadPGM_ui8matrix("../hall/hall000000.pgm",&nrl,&nrh,&ncl,&nch);
  uint8** O1 = ui8matrix(nrl, nrh, ncl, nch);
  uint8** E1 = ui8matrix(nrl, nrh, ncl, nch);
  uint8** M1 = ui8matrix(nrl, nrh, ncl, nch);
  uint8** V0 = ui8matrix(nrl, nrh, ncl, nch);
  uint8** V1 = ui8matrix(nrl, nrh, ncl, nch);
  
  for(int i = 0; i < NIMAGES - 1; i++)
    {
      sprintf(filename0,"../hall/hall%06d.pgm",i);
      sprintf(filename1,"../hall/hall%06d.pgm",i+1);
      I0 = LoadPGM_ui8matrix(filename0, &nrl, &nrh, &ncl, &nch);
      I1 = LoadPGM_ui8matrix(filename1, &nrl, &nrh, &ncl, &nch);

      clock_gettime(CLOCK_MONOTONIC, &startTot);
      clock_gettime(CLOCK_MONOTONIC, &startSD);
      sigmaDelta(I0, I1, M0, M1, V0, V1, O1, E1, nrl, nrh, ncl, nch);
      clock_gettime(CLOCK_MONOTONIC, &endSD);
      cpuTimeSD += (double) (endSD.tv_sec - startSD.tv_sec);
      cpuTimeSD += (double) (endSD.tv_nsec - startSD.tv_nsec)/ 1000000000.0;
      
      clock_gettime(CLOCK_MONOTONIC, &startMorpho);
      E1 = ouverture(E1, nrl, nrh, ncl, nch, dim);
      E1 = fermeture(E1, nrl, nrh, ncl, nch, dim);
      E1 = ouverture(E1, nrl, nrh, ncl, nch, 5);
      E1 = fermeture(E1, nrl, nrh, ncl, nch, 5);
      clock_gettime(CLOCK_MONOTONIC, &endMorpho);
      clock_gettime(CLOCK_MONOTONIC, &endTot);

      cpuTimeMorpho += (double) (endMorpho.tv_sec - startMorpho.tv_sec);
      cpuTimeMorpho += (double) (endMorpho.tv_nsec - startMorpho.tv_nsec)/ 1000000000.0;
      cpuTimeTot += (double) (endTot.tv_sec - startTot.tv_sec);
      cpuTimeTot += (double) (endTot.tv_nsec - startTot.tv_nsec)/ 1000000000.0;
         
      sprintf(filenameO,"../output/hallOMP/SDO/hall%06dO.pgm",i);
      sprintf(filenameE,"../output/hallOMP/SDE/hall%06dE.pgm",i+1);
      
      SavePGM_ui8matrix(O1, nrl, nrh, ncl, nch, filenameO);
      SavePGM_ui8matrix(E1, nrl, nrh, ncl, nch, filenameE);

      copy_ui8matrix_ui8matrix (V1, nrl, nrh, ncl, nch, V0);
      copy_ui8matrix_ui8matrix (M1, nrl, nrh, ncl, nch, M0);
    }
  printf("Temps passé dans l'algo SD : %f ms\n", cpuTimeSD*1000);
  printf("Temps passé dans les algos de morphologies : %f ms\n", cpuTimeMorpho*1000);
  printf("Temps total : %f ms\n", cpuTimeTot*1000);
  
  // free_ui8matrix(O1, nrl+2, nrh+2, ncl+2, nch+2);
  // free_ui8matrix(E1, nrl+2, nrh+2, ncl+2, nch+2);
  free_ui8matrix(I0, nrl, nrh, ncl, nch);
  free_ui8matrix(I1, nrl, nrh, ncl, nch);
  free_ui8matrix(V0, nrl, nrh, ncl, nch);
  free_ui8matrix(V1, nrl, nrh, ncl, nch);
  free_ui8matrix(M0, nrl, nrh, ncl, nch);
  free_ui8matrix(M1, nrl, nrh, ncl, nch);
}

void benchDetectionMouvementFD(uint8 theta)
{
  struct timespec startFD, endFD, startTot, endTot, startMorpho, endMorpho;
  double cpuTimeFD = 0.0;
  double cpuTimeTot = 0.0;
  double cpuTimeMorpho = 0.0;
  long nrl, nrh, ncl, nch;
  int dim = 3;
  char filename0[255];
  char filename1[255];
  char filenameE[255];
  char filenameO[255];
  uint8** I0 = LoadPGM_ui8matrix("../hall/hall000000.pgm",&nrl,&nrh,&ncl,&nch);
  uint8** I1;
  uint8** O = ui8matrix(nrl, nrh, ncl, nch);
  uint8** E = ui8matrix(nrl, nrh, ncl, nch);
  
  for(int i = 0; i < NIMAGES - 1; i++)
    {
      sprintf(filename0,"../hall/hall%06d.pgm",i);
      sprintf(filename1,"../hall/hall%06d.pgm",i+1);
      I0 = LoadPGM_ui8matrix(filename0, &nrl, &nrh, &ncl, &nch);
      I1 = LoadPGM_ui8matrix(filename1, &nrl, &nrh, &ncl, &nch);

      clock_gettime(CLOCK_MONOTONIC, &startTot);
      clock_gettime(CLOCK_MONOTONIC, &startFD);
      frameDifference(I0, I1, O, E, nrl, nrh, ncl, nch, theta);
      clock_gettime(CLOCK_MONOTONIC, &endFD);
      cpuTimeFD += (double) (endFD.tv_sec - startFD.tv_sec);
      cpuTimeFD += (double) (endFD.tv_nsec - startFD.tv_nsec)/ 1000000000.0;
      
      clock_gettime(CLOCK_MONOTONIC, &startMorpho);
      E = ouverture(E, nrl, nrh, ncl, nch, dim);
      E = fermeture(E, nrl, nrh, ncl, nch, dim);
      clock_gettime(CLOCK_MONOTONIC, &endMorpho);
      clock_gettime(CLOCK_MONOTONIC, &endTot);

      cpuTimeMorpho += (double) (endMorpho.tv_sec - startMorpho.tv_sec);
      cpuTimeMorpho += (double) (endMorpho.tv_nsec - startMorpho.tv_nsec)/ 1000000000.0;
      cpuTimeTot += (double) (endTot.tv_sec - startTot.tv_sec);
      cpuTimeTot += (double) (endTot.tv_nsec - startTot.tv_nsec)/ 1000000000.0;
      
      sprintf(filenameO,"../output/hallOMP/FDO/hall%06dO.pgm",i);
      sprintf(filenameE,"../output/hallOMP/FDE/hall%06dE.pgm",i+1);
      
      SavePGM_ui8matrix(O, nrl, nrh, ncl, nch, filenameO);
      SavePGM_ui8matrix(E, nrl, nrh, ncl, nch, filenameE);
      
    }
  
  //free_ui8matrix(O, nrl, nrh, ncl, nch);
  //free_ui8matrix(E, nrl, nrh, ncl, nch);
  free_ui8matrix(I0, nrl, nrh, ncl, nch);
  free_ui8matrix(I1, nrl, nrh, ncl, nch);

  printf("Temps passé dans l'algo FD : %f ms\n", cpuTimeFD*1000);
  printf("Temps passé dans les algos de morphologies : %f ms\n", cpuTimeMorpho*1000);
  printf("Temps total : %f ms\n", cpuTimeTot*1000);
}


void benchQualitatifFD()
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
      sprintf(filename1,"../output/hallOMP/FDE/hall%06dE.pgm",i);

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

void benchQualitatifSD()
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
      sprintf(filename1,"../output/hallOMP/SDE/hall%06dE.pgm",i);

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

int main(int argc, char const *argv[])
{
  int theta = 6;
  printf("+==========================+\nFrame Difference OMP :\n");
  benchDetectionMouvementFD(theta);
  benchQualitatifFD();
  
  printf("+==========================+\nSigma Delta OMP :\n");
  benchDetectionMouvementSD();
  benchQualitatifSD();

  
  /*
    double t0,t1,tmin,cycles,dt;
    CHRONO(print("ok"),cycles);
    printf("%f\n",cycles);
  */
  return 0;
}

