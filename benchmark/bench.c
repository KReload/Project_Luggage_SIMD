#include "../include/bench.h"



void print(char* msg) {
    printf("%s\n", msg);
}

int main(int argc, char const *argv[])
{

    double t0,t1,tmin,cycles,dt;
    CHRONO(print("ok"),cycles);
    printf("%f\n",cycles);
    return 0;
}

