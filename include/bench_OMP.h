#include "morpho_OMP.h"
#include "mouvement_OMP.h"
#include <math.h>

#include <x86intrin.h>
#include <stdio.h>
#include <time.h>
#define NITER 2
#define NRUN 5

/*
#define CHRONO(X,t)                         \                    
    tmin = 2147483647;                      \
    for(int run=0; run < NRUN; run++) {     \
        t0 = (double)_rdtsc();              \
        for(int iter=0; iter<NITER;iter++) {\
            X;                              \
        }                                   \
        t1 = (double)_rdtsc();              \
        dt = t1 - t0;                       \
        if(dt<tmin) {                       \
            tmin = dt;                      \
        }                                   \
    }                                       \
    t = tmin/2
*/
