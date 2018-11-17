
#include <stdio.h>

void printResultTest(char*desctest, int test) {
    printf("\t\t%s: ",desctest);
    if(test){
        printf("\u2714\n");
    } else {
        printf("\u274c\n");
    }
}