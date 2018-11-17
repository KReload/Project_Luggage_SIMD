#include "../include/testutil.h"
#include "../include/morpho.h"


void testErosion() {
  printf("\n Test erosion\n");
  int a = 255;
  int min = 200;
  int checkTest=1;

  min = MIN(a,min);

  printResultTest("it should return 255 if elem = 255 and min = 200", min == 200);
  min = MIN(200,min);

  printResultTest("it should return 255 if elem = 200 and min = 200", min == 200);
  min = MIN(255,min);

  printResultTest("it should return 255 if elem = 255 and min = 200", min == 200);

}

void testDilatation() {
  printf("\n Test dilatation\n");
  int a = 255;
  int max = 200;
  int checkTest=1;

  max = MAX(a,max);

  printResultTest("it should return 255 if elem = 255 and max = 200", max == 255);
  max = MAX(200,max);

  printResultTest("it should return 255 if elem = 200 and max = 255", max == 255);
  max = MAX(255,max);

  printResultTest("it should return 255 if elem = 255 and max = 255", max == 255);
}


int main(void) {
  testErosion();
  testDilatation();

  return 0;
}
