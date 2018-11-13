#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <stddef.h>
#include <ctype.h> 
#include <math.h> /* fabs */


#include "vnrutil.h"
#include "simdutil.h"

vuint8** frameDifference(vuint8** I0, vuint8** I1, long* nrl,long* nrh,long* ncl, long* nch);
vuint8** sigmaDelta(vuint8** I0, vuint8** I1, long nrl,long nrh, long ncl, long nch);