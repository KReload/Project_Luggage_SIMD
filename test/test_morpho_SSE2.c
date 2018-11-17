#include "../include/morpho_SSE2.h"
#include "../include/testutil.h"

void test_dilatation_SSE()
{
  
  int checktest = 1;
  vuint8**img = vui8matrix(0, 10, 0, 10);

  for(int i = 0; i < 10; i++)
    {
      for(int j = 0; j < 10; j++)
	{
	  if(i == 5 && j == 5)
	    img[i][j] = init_vuint8((uint8)1);
	  else
	    img[i][j] = init_vuint8((uint8)0);
	}
    }

  //display_vui8matrix (img,  0, 10, 0, 10, "%d ", "IMG :");
  printf("===================================\n");
  vuint8**M = dilatation_SSE3x3_elemVertical(img, 0, 10, 0, 10);
  M = dilatation_SSE3x3_elemHorizontal(M, 0, 10, 0, 10);
  //display_vui8matrix (M,  0, 10, 0, 10, "%d ", "IMG DILATE :");

  printf("\nTesting dilatation\n");
  vuint8 res[1];
  uint8* test = (uint8*)res; 
  for(int i = 0; i < 10; i++)
  {
    for(int j = 0; j < 10; j++)
	  {
      if(i > 4 && i < 6 && j == 5) {

        res[0] = _mm_cmpeq_epi8(M[i][j],init_vuint8(1));
        
        for(int k=0;k<N_OCTET;k++){
          if(test[k]!=255){
            checktest = 0;
          }
        }
        
      }
    }
  }

  printResultTest("it should return the right result: 1 inside the dilatation", checktest);
  checktest = 1;
  

  for(int i = 4; i < 7; i++) {
    res[0] = _mm_cmpeq_epi8(M[i][4],_mm_set_epi8(1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0));
    for(int k=0;k<N_OCTET;k++){
      if(test[k]!=255){
        checktest = 0;
      }
    }
  }
  printResultTest("it should return the right result: 1 on the left side of the dilatation", checktest);
  checktest = 1;

  for(int i = 4; i < 7; i++) {
    res[0] = _mm_cmpeq_epi8(M[i][6],_mm_set_epi8(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1));

    for(int k=0;k<N_OCTET;k++){
      if(test[k]!=255){
        checktest = 0;
      }
    }
  }
  printResultTest("it should return the right result: 1 on the right side of the dilatation", checktest);
  checktest = 1;

  
  //printResultTest("0 < 255 should return 11111111*16", checkTest);

}


void test_erosion_SSE()
{
  
  int checkTest = 1;   
  vuint8**img = vui8matrix(0, 10, 0, 10);

  for(int i = 0; i < 10; i++)
    {
      for(int j = 0; j < 10; j++)
	{
	  if(i == 5 && j == 5)
	    img[i][j] = init_vuint8((uint8)1);
	  else if(i == 4 && j == 5)
	    img[i][j] = init_vuint8((uint8)1);
	  else if(i == 6 && j == 5)
	    img[i][j] = init_vuint8((uint8)1);
	  else if(i == 7 && j == 5)
	    img[i][j] = init_vuint8((uint8)1);
	  else
	    img[i][j] = init_vuint8((uint8)0);
	}
    }

  //display_vui8matrix (img, 0, 10, 0, 10, "%d ", "IMG :");
  printf("===================================\n");
  printf("\nTesting erosion\n");
  vuint8**M = erosion_SSE3x3_elemVertical(img, 0, 10, 0, 10);
  M = erosion_SSE3x3_elemHorizontal(M, 0, 10, 0, 10);
  //display_vui8matrix (M, 0, 10, 0, 10, "%d ", "IMG ERODE :");
  vuint8 res[1];
  uint8* test = (uint8*)res; 

  for(int i = 5; i < 7; i++) {
    res[0] = _mm_cmpeq_epi8(M[i][5],_mm_set_epi8(0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0));

    for(int k=0;k<N_OCTET;k++){
      if(test[k]!=255){
        checkTest = 0;
      }
    }
  }

  printResultTest("it should return the right erosion (two lines of 0111111111111110)", checkTest);
}

int main(){
  test_dilatation_SSE();
  test_erosion_SSE();
  return 0;
}
