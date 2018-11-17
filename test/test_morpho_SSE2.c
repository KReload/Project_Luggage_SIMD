#include "../include/morpho_SSE2.h"
#include "../include/testutil.h"

void test_dilatation_SSE()
{
  printf("\nTesting dilatation\n");
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

  display_vui8matrix (img,  0, 10, 0, 10, "%d ", "IMG :");
  printf("===================================\n");
  vuint8**M = dilatation_SSE3x3_elemVertical(img, 0, 10, 0, 10);
  M = dilatation_SSE3x3_elemHorizontal(M, 0, 10, 0, 10);
  display_vui8matrix (M,  0, 10, 0, 10, "%d ", "IMG DILATE :");

  for(int i = 4; i < 6; i++)
    {
      for(int j = 4; j < 6; j++)
	{
	  checktest = 0;
	}
    }

  
  //printResultTest("0 < 255 should return 11111111*16", checkTest);

}


void test_erosion_SSE()
{
  printf("\nTesting erosion\n");   
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

  display_vui8matrix (img, 0, 10, 0, 10, "%d ", "IMG :");
  printf("===================================\n");
  vuint8**M = erosion_SSE3x3_elemVertical(img, 0, 10, 0, 10);
  M = erosion_SSE3x3_elemHorizontal(M, 0, 10, 0, 10);
  display_vui8matrix (M, 0, 10, 0, 10, "%d ", "IMG ERODE :");
}

int main(){
  test_dilatation_SSE();
  test_erosion_SSE();
  return 0;
}
