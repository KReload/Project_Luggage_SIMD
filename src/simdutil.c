

#include "../include/vnrutil.h"
#include <stdio.h>
#include <string.h>


#include "../include/nrutil.h"

void copy_vui8matrix_vui8matrix(vuint8 **X, int i0, int i1, int j0, int j1, vuint8 **Y)
{
    int i, j;
	vuint8 tmp;

    for(i=i0; i<=i1; i++) {
        for(j=j0; j<=j1; j++) {
			tmp = _mm_load_si128(&X[i][j]);
			_mm_store_si128(&Y[i][j], tmp);
        }
    }
}

void ReadPGMrowSIMD(FILE *file, long width, vuint8  *line)
/* ---------------------------------------------- */
{
    /* Le fichier est ouvert (en lecture) et ne sera pas ferme a la fin */
     fread(&(line[0]), sizeof(vuint8), width/16, file);
}

void WritePGMrowSIMD(vuint8 *line, long width, FILE  *file)
/* ----------------------------------------------- */
{
/* Le fichier est deja ouvert et ne sera pas ferme a la fin */

   fwrite(&(line[0]), sizeof(vuint8), width/16, file);

}

vuint8** LoadPGM_vui8matrix(char *filename, long *nrl, long *nrh, long *ncl, long *nch)
/* ------------------------------------------------------------------------------ */
{
  /* cette version ne lit plus que le type P5 */

  long height, width, gris;
  vuint8 **m;
  FILE *file;
  /*int   format;*/

  char *buffer;
  /*char  c;*/
  int i;
  
  buffer = (char*) calloc(80, sizeof(char));
  /* ouverture du fichier */
  file = fopen(filename,"rb");
  if (file==NULL)
    fprintf(stderr,"ouverture du fichier impossible\n");
    //nrerror("ouverture du fichier %s impossible\n", filename);

  /* lecture de l'entete du fichier pgm */
  readitem(file, buffer);
  /*fscanf(fichier, "%s", buffer);*/
  if(strcmp(buffer, "P5") != 0)
    fprintf(stderr,"entete du fichier %s invalide\n");
    //nrerror("entete du fichier %s invalide\n", filename);

  width  = atoi(readitem(file, buffer));
  height = atoi(readitem(file, buffer));
  gris   = atoi(readitem(file, buffer));

  *nrl = 0;
  *ncl = 0;
  *nrh = height - 1;
  *nch = width/16;
  m = vui8matrix(*nrl, *nrh, *ncl, *nch);
  
  for(i=0; i<height; i++) {
    ReadPGMrowSIMD(file, width, m[i]);
  }

  fclose(file);
  free(buffer);

  return m;
}

void SavePGM_vui8matrix(vuint8 **m, long nrl, long nrh, long ncl, long nch, char *filename)
/* ----------------------------------------------------------------------------------- */
{
  long nrow = nrh-nrl+1;
  long ncol = nch*16-ncl;

  char buffer[80];
  
  FILE *file;
  int  i;

  file = fopen(filename, "wb");
  if (file == NULL)
    //nrerror("ouverture du fichier %s impossible dans SavePGM_bmatrix\n", filename);
    fprintf(stderr,"ouverture du fichier %s impossible dans SavePGM_vui8matrix\n");

  /* enregistrement de l'image au format rpgm */
  
  sprintf(buffer,"P5\n%ld %ld\n255\n",ncol, nrow);
  fwrite(buffer,strlen(buffer),1,file);
  for(i=nrl; i<=nrh; i++)
    WritePGMrowSIMD(m[i], ncol, file);

  /* fermeture du fichier */
  fclose(file);
}


void mullo_epi8(vuint8 left, vuint8 right, vuint16* lo, vuint16* hi) {

  vuint8 zeros= init_vuint8(0);
  vuint16 left_lo= _mm_unpacklo_epi8(left,zeros);
  vuint16 left_hi= _mm_unpackhi_epi8(left,zeros);
  vuint16 right_lo= _mm_unpacklo_epi8(right,zeros);
  vuint16 right_hi= _mm_unpackhi_epi8(right,zeros);

  //vuint8 a = _mm_set_epi16(1,2,3,4,5,6,7,65535);
  //vuint8 b = _mm_set_epi16(1,2,3,4,5,6,7,3);
  
  *lo = _mm_mullo_epi16(left_lo,right_lo);
  *hi = _mm_mullo_epi16(left_hi,right_hi);


}
/*
int main(void)
{
  long nrl, nrh, ncl,nch;
  vuint8**img = LoadPGM_vui8matrix("./hall/hall000000.pgm",&nrl,&nrh,&ncl,&nch);
  SavePGM_vui8matrix(img, nrl, nrh, ncl, nch, "hall0SIMDtest.pgm");
}
*/
