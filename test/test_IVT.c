#include "../include/nrdef.h"
#include "../include/nrutil.h"

void checkIVT()
{
  long nrl, nrh, ncl, nch;

  uint8** IVT;
  char filename0[255];
  int valid = 1;
  for(int i = 12; i < 300; i+=20)
    {
      sprintf(filename0,"../IVT/hall%d_VT.pgm",i);
      IVT = LoadPGM_ui8matrix(filename0,&nrl,&nrh,&ncl,&nch);
      for(int j = nrl; j < nrh; j++)
	{
	  for(int k = ncl; k < nch; k++)
	    {
	      if(IVT[j][k] != 0 && IVT[j][k] != 255)
		{
		  printf("ERROR IVT[%d][%d] = %d\n", j,k,IVT[j][k]);;
		  valid = 0;
		}
	    }
	}
      if(valid == 1)
	printf("Image %d est valide.\n",i);
      else
	printf("Image %d est non valide.\n",i);
      valid = 1;
   }
  free_ui8matrix(IVT, nrl, nrh, ncl, nch);
}

int main(void)
{
  checkIVT();
}
