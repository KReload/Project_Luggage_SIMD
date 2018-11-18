#include "morpho_OMPxSSE2.h"
#include "mouvement_OMPxSSE2.h"
#include "nrutil.h"
#include <math.h>

#include <x86intrin.h>
#include <stdio.h>
#include <time.h>
#define NITER 2
#define NRUN 5
#define NIMAGES 300

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
