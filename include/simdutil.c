
#include <emmintrin.h>
#include "vnrutil.h"
#include <stdio.h>
#include <string.h>


#include "nrutil.h"



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
    nrerror("ouverture du fichier impossible\n");
    //nrerror("ouverture du fichier %s impossible\n", filename);

  /* lecture de l'entete du fichier pgm */
  readitem(file, buffer);
  /*fscanf(fichier, "%s", buffer);*/
  if(strcmp(buffer, "P5") != 0)
    nrerror("entete du fichier %s invalide\n");
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
    nrerror("ouverture du fichier %s impossible dans SavePGM_vui8matrix\n");

  /* enregistrement de l'image au format rpgm */
  
  sprintf(buffer,"P5\n%ld %ld\n255\n",ncol, nrow);
  fwrite(buffer,strlen(buffer),1,file);
  for(i=nrl; i<=nrh; i++)
    WritePGMrowSIMD(m[i], ncol, file);

  /* fermeture du fichier */
  fclose(file);
}
/*
int main(void)
{
  long nrl, nrh, ncl,nch;
  vuint8**img = LoadPGM_vui8matrix("./hall/hall000000.pgm",&nrl,&nrh,&ncl,&nch);
  SavePGM_vui8matrix(img, nrl, nrh, ncl, nch, "hall0SIMDtest.pgm");
}
*/
