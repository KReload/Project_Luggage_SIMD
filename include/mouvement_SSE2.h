#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <stddef.h>
#include <ctype.h> 
#include <math.h> /* fabs */


#include "vnrutil.h"
#include "simdutil.h"

void** frameDifference(vuint8** I0, vuint8** I1, vuint8**O, vuint8**E, long nrl,long nrh,long ncl, long nch);
vuint8** sigmaDelta(vuint8** I0, vuint8** I1, vuint8** M0, vuint8** M1, vuint8** V0, vuint8** V1, vuint8** O1, vuint8** E1, long nrl,long nrh, long ncl, long nch);
