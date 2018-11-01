#include "morpho.h"

/*
void dilatation(char *filename1) {
  long nrl,nrh,ncl,nch;

  //Image à dilater
  uint8** img = LoadPGM_ui8matrix(filename1,&nrl,&nrh,&ncl,&nch);

  //Element structurant
  uint8 elemStruct = ui8matrix(-1,+1,-1,+1);

  //Allocation sortie de taille (r+2)*(c+2) pour avoir une bordure
  uint8** output = ui8matrix(nrl-1,nrh+1,ncl-1,nch+1);

  for(int i = 0;i<nrh;i++) {
    for(int j = 0;j<nch;j++) {
      output[i][j] = 0;
    }
  }

}
*/

uint8** dilatation(uint8** M, int nrl, int nrh, int ncl, int nch, int dim, int seuil)
{
  int r = dim/2;
  
  //Allocation sortie de taille (r+dim)*(c+dim) pour avoir une bordure
  uint8** output = ui8matrix(nrl-r, nrh+r, ncl-r, nch+r);
  
  for(int i = 0; i < nrh; i++)
    {
      for(int j = 0; j < nch; j++)
	{
	  if(M[i][j] > seuil)
	    {
	      for(int k = i-r; k < i+(r+1); k++)
		for(int p = j-r; p < j+(r+1); p++)
		  output[k][p] = 0;
	    }
	  
	  else
	    output[i][j] = M[i][j];

	}
    }

  return output;
}

uint8** erosion(uint8** M, int nrl, int nrh, int ncl, int nch, int dim, int seuil)
{
  int r = dim/2;
  //Allocation sortie de taille (r+dim)*(c+dim) pour avoir une bordure
  uint8** output = ui8matrix(nrl-r, nrh+r, ncl-r, nch+r);
  
   
  for(int i = 0; i < nrh; i++)
    {
      for(int j = 0; j < nch; j++)
	{
	  if(M[i][j] < seuil)
	    {
	      for(int k = i-r; k < i+(r+1); k++)
		for(int p = j-r; p < j+(r+1); p++)
		  output[k][p] = 255;
	    }
	  
	  else
	    output[i][j] = M[i][j];
	}
    }

  return output;
}

uint8** fermeture(uint8** M, int nrl, int nrh, int ncl, int nch, int dim, int seuil)
{
  uint8** output;
  output = dilatation(M, nrl, nrh, ncl, nch, dim, seuil);
  output = erosion(output, nrl, nrh, ncl, nch, dim, seuil);

  return output;
}

uint8** ouverture(uint8** M, int nrl, int nrh, int ncl, int nch, int dim, int seuil)
{
  uint8** output;
  output = erosion(M, nrl, nrh, ncl, nch, dim, seuil);
  output = dilatation(output, nrl, nrh, ncl, nch, dim, seuil);
  
  return output;
}
