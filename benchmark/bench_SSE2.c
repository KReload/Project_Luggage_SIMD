#include "../include/mouvement_SSE2.h"
#include "../include/morpho_SSE2.h"
#include <time.h>

#define FILENAMESIZE 22
#define NIMAGES 300

void detectionMouvement()
{
  clock_t startSD, endSD, startTot, endTot, startMorpho, endMorpho;
  double cpuTimeSD = 0.0;
  double cpuTimeTot = 0.0;
  double cpuTimeMorpho = 0.0;

  long nrl, nrh, ncl, nch;
  int dim = 3;
  char* filename0 = (char*) malloc(sizeof(char) * FILENAMESIZE);
  char* filename1 = (char*) malloc(sizeof(char) * FILENAMESIZE);
  char* filenameE = (char*) malloc(sizeof(char) * (FILENAMESIZE+2));
  char* filenameO = (char*) malloc(sizeof(char) * (FILENAMESIZE+2));
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
      startSD = clock();
      sprintf(filename0,"../hall/hall%06d.pgm",i);
      sprintf(filename1,"../hall/hall%06d.pgm",i+1);
      I0 = LoadPGM_vui8matrix(filename0, &nrl, &nrh, &ncl, &nch);
      I1 = LoadPGM_vui8matrix(filename1, &nrl, &nrh, &ncl, &nch);

      sigmaDelta(I0, I1, M0, M1, V0, V1, O1, E1, nrl, nrh, ncl, nch);
      
      //E1 = ouverture_SSE(E1, nrl, nrh, ncl, nch, dim);
      //E1 = fermeture_SSE(E1, nrl, nrh, ncl, nch, dim);
      /*E1 = ouverture_SSE(E1, nrl, nrh, ncl, nch, 5);
      E1 = fermeture_SSE(E1, nrl, nrh, ncl, nch, 5);*/

      E1 = erosion_SSE3x3_elemVertical(E1, nrl, nrh, ncl, nch);
      E1 = erosion_SSE3x3_elemHorizontal(E1, nrl, nrh, ncl, nch);
      E1 = erosion_SSE3x3_elemVertical(E1, nrl, nrh, ncl, nch);
      E1 = erosion_SSE3x3_elemHorizontal(E1, nrl, nrh, ncl, nch);
      E1 = erosion_SSE3x3_elemVertical(E1, nrl, nrh, ncl, nch);
      E1 = erosion_SSE3x3_elemHorizontal(E1, nrl, nrh, ncl, nch);
      E1 = erosion_SSE3x3_elemVertical(E1, nrl, nrh, ncl, nch);
      E1 = erosion_SSE3x3_elemHorizontal(E1, nrl, nrh, ncl, nch);

      
      endSD = clock();
      cpuTimeSD += ((double) (endSD-startSD))/ CLOCKS_PER_SEC * 1000;
      //E1 = ouverture(E1, nrl, nrh, ncl, nch, 5);
      //E1 = fermeture(E1, nrl, nrh, ncl, nch, 5);
      
      sprintf(filenameO,"../hallSSESDO/hall%06dO.pgm",i);
      sprintf(filenameE,"../hallSSESDE/hall%06dE.pgm",i+1);
      
      SavePGM_vui8matrix(O1, nrl, nrh, ncl, nch, filenameO);
      SavePGM_vui8matrix(E1, nrl, nrh, ncl, nch, filenameE);
    
    }
  endTot = clock();
  cpuTimeTot = ((double) (endTot-startTot))/ CLOCKS_PER_SEC * 1000;
  printf("Temps passé dans l'algo SD : %f ms\n", cpuTimeSD);
  printf("Temps total : %f ms\n", cpuTimeTot);

  // free_ui8matrix(O1, nrl+2, nrh+2, ncl+2, nch+2);
  // free_ui8matrix(E1, nrl+2, nrh+2, ncl+2, nch+2);
  free_vui8matrix(I0, nrl, nrh, ncl, nch);
  free_vui8matrix(I1, nrl, nrh, ncl, nch);
  free_vui8matrix(V0, nrl, nrh, ncl, nch);
  free_vui8matrix(V1, nrl, nrh, ncl, nch);
  free_vui8matrix(M0, nrl, nrh, ncl, nch);
  free_vui8matrix(M1, nrl, nrh, ncl, nch);

}

int main(int argc, char* argv[])
{
  detectionMouvement();
  return 0;
}
