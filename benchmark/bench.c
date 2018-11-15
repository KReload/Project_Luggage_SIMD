#include "../include/bench.h"

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
  clock_t startSD, endSD, startTot, endTot, startMorpho, endMorpho;
  startTot = clock();
  double cpuTimeSD = 0.0;
  double cpuTimeTot = 0.0;
  double cpuTimeMorpho = 0.0;
  long nrl, nrh, ncl, nch;
  int dim = 3;
  char* filename0 = (char*) malloc(sizeof(char) * FILENAMESIZE);
  char* filename1 = (char*) malloc(sizeof(char) * FILENAMESIZE);
  char* filenameE = (char*) malloc(sizeof(char) * (FILENAMESIZE+8));
  char* filenameO = (char*) malloc(sizeof(char) * (FILENAMESIZE+8));
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

      startSD = clock();
      sigmaDelta(I0, I1, M0, M1, V0, V1, O1, E1, nrl, nrh, ncl, nch);
      endSD = clock();
      cpuTimeSD += ((double) (endSD-startSD))/ CLOCKS_PER_SEC * 1000;

      startMorpho = clock();
      // E1 = ouverture(E1, nrl, nrh, ncl, nch, dim);
      // E1 = fermeture(E1, nrl, nrh, ncl, nch, dim);
      // E1 = ouverture(E1, nrl, nrh, ncl, nch, 5);
      // E1 = fermeture(E1, nrl, nrh, ncl, nch, 5);
      endMorpho = clock();
      cpuTimeMorpho += ((double) (endMorpho-startMorpho))/CLOCKS_PER_SEC * 1000;
      
      sprintf(filenameO,"../exe/hallSDO/hall%06dO.pgm",i);
      sprintf(filenameE,"../exe/hallSDE/hall%06dE.pgm",i+1);
      
      SavePGM_ui8matrix(O1, nrl, nrh, ncl, nch, filenameO);
      SavePGM_ui8matrix(E1, nrl, nrh, ncl, nch, filenameE);

      copy_ui8matrix_ui8matrix (V1, nrl, nrh, ncl, nch, V0);
      copy_ui8matrix_ui8matrix (M1, nrl, nrh, ncl, nch, M0);
    }

  // free_ui8matrix(O1, nrl+2, nrh+2, ncl+2, nch+2);
  // free_ui8matrix(E1, nrl+2, nrh+2, ncl+2, nch+2);
  free_ui8matrix(I0, nrl, nrh, ncl, nch);
  free_ui8matrix(I1, nrl, nrh, ncl, nch);
  free_ui8matrix(V0, nrl, nrh, ncl, nch);
  free_ui8matrix(V1, nrl, nrh, ncl, nch);
  free_ui8matrix(M0, nrl, nrh, ncl, nch);
  free_ui8matrix(M1, nrl, nrh, ncl, nch);
  endTot = clock();
  cpuTimeTot = ((double) (endTot-startTot))/ CLOCKS_PER_SEC * 1000;
  printf("Temps passé dans l'algo SD : %f ms\n", cpuTimeSD);
  printf("Temps passé dans les algos de morphologies : %f ms\n", cpuTimeMorpho);
  printf("Temps total : %f ms\n", cpuTimeTot);
}

void benchDetectionMouvementFD(uint8 theta)
{
  clock_t startFD, endFD, startTot, endTot, startMorpho, endMorpho;
  double cpuTimeFD = 0.0;
  double cpuTimeTot = 0.0;
  double cpuTimeMorpho = 0.0;
  long nrl, nrh, ncl, nch;
  int dim = 3;
  char* filename0 = (char*) malloc(sizeof(char) * FILENAMESIZE);
  char* filename1 = (char*) malloc(sizeof(char) * FILENAMESIZE);
  char* filenameE = (char*) malloc(sizeof(char) * (FILENAMESIZE+8));
  char* filenameO = (char*) malloc(sizeof(char) * (FILENAMESIZE+8));
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

      startTot = clock();
      startFD = clock();
      frameDifference(I0, I1, O, E, nrl, nrh, ncl, nch, theta);
      endFD = clock();
      cpuTimeFD += ((double) (endFD-startFD))/ CLOCKS_PER_SEC * 1000;

      startMorpho = clock();
      // E = ouverture(E, nrl, nrh, ncl, nch, dim);
      // E = fermeture(E, nrl, nrh, ncl, nch, dim);
      endMorpho = clock();
      cpuTimeMorpho += ((double) (endMorpho-startMorpho))/ CLOCKS_PER_SEC * 1000;
      endTot = clock();
      cpuTimeTot += ((double) (endTot-startTot))/ CLOCKS_PER_SEC * 1000;
      
      sprintf(filenameO,"../exe/hallFDO/hall%06dO.pgm",i);
      sprintf(filenameE,"../exe/hallFDE/hall%06dE.pgm",i+1);
      
      SavePGM_ui8matrix(O, nrl, nrh, ncl, nch, filenameO);
      SavePGM_ui8matrix(E, nrl, nrh, ncl, nch, filenameE);
      
    }
  //free_ui8matrix(O, nrl, nrh, ncl, nch);
  //free_ui8matrix(E, nrl, nrh, ncl, nch);
  free_ui8matrix(I0, nrl, nrh, ncl, nch);
  free_ui8matrix(I1, nrl, nrh, ncl, nch);

  printf("Temps passé dans l'algo FD : %f ms\n", cpuTimeFD);
  printf("Temps passé dans les algos de morphologies : %f ms\n", cpuTimeMorpho);
  printf("Temps total : %f ms\n", cpuTimeTot);
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
  char* filename0 = (char*) malloc(sizeof(char) * 19);
  char* filename1 = (char*) malloc(sizeof(char) * 30);
  double mcc = 0.0;
  for(int i = 32; i < 92; i+=20)
    {
      sprintf(filename0,"./IVT/hall%d_VT.pgm",i);
      sprintf(filename1,"../exe/hallFDE/hall0000%dE.pgm",i);

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
  char* filename0 = (char*) malloc(sizeof(char) * 20);
  char* filename1 = (char*) malloc(sizeof(char) * 30);

  for(int i = 32; i < 92; i+=20)
    {
      sprintf(filename0,"./IVT/hall%d_VT.pgm",i);
      sprintf(filename1,"../exe/hallSDE/hall0000%dE.pgm",i);

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
  int theta = 10;
  printf("+==========================+\nFrame Difference :\n");
  benchDetectionMouvementFD(theta);
  //benchQualitatifFD();
  
  printf("+==========================+\nSigma Delta :\n");
  benchDetectionMouvementSD();
  //benchQualitatifSD();

  
  /*
    double t0,t1,tmin,cycles,dt;
    CHRONO(print("ok"),cycles);
    printf("%f\n",cycles);
  */
  return 0;
}

