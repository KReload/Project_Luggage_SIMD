void sigmaDelta(char* filename1, char* filename2)
{
  long nrl,nrh,ncl,nch;
  uint8** I0 = LoadPGM_ui8matrix(char *filename,&nrl,&nrh,&ncl,&nch);
  uint8** I1 = LoadPGM_ui8matrix(char *filename,&nrl,&nrh,&ncl,&nch);
  uint8** M0 = I0;
  uint8** M1;
  uint8** V = Vmin;
  uint8** O1;

  //Step 1 : M1 estimation
  for(int i = 0; i < nrh; i++)
    {
      for(int j = 0; j < nch; j++)
	{
	  if(M0[i][j] < I1[i][j])
	    M1[i][j] = M0[i][j] + 1;
	  else if(M0[i][j] > I1[i][j])
	    M1[i][j] = M0[i][j] - 1;
	  else
	    M1[i][j] = M0[i][j];
	}
    }

  //Step 2 : O1 Computation
  for(int i = 0; i < nrh; i++)
    {
      for(int j = 0; j < nch; j++)
	O1[i][j] = M1[i][j] - I1[i][j]; 
    }

  //Step 3 : V1 Update and Clamping
  for(int i = 0; i < nrh; i++)
    {
      for(int j = 0; j < nch; j++)
	{
	  if(M0[i][j] < I1[i][j])
	    M1[i][j] = M0[i][j] + 1;
	  else if(M0[i][j] > I1[i][j])
	    M1[i][j] = M0[i][j] - 1;
	  else
	    M1[i][j] = M0[i][j];
	}
    }
  
}


int main(void)
{
  return 0;
}
