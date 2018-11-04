#include "validation.h"

void calculMatriceROC(uint32** ROC, uint8** img, uint8** img_vt, long nrl, long nrh, long ncl, long nch)
{
  int vp = 0, vn = 0, fp = 0, fn = 0;
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

  ROC[0][0] = vp;
  ROC[0][1] = fn;
  ROC[1][0] = fp;
  ROC[1][1] = vn;
}

double calculMCC(uint32** ROC)
{
  double mcc;
  double vp = ROC[0][0];
  double fn = ROC[0][1];
  double fp = ROC[1][0];
  double vn = ROC[1][1];
  
  mcc = (vp*vn - fp*fn) / sqrt((vp+fp)*(vp+fn)*(vn+fp)*(vn+fn));
  
  return mcc;
}
