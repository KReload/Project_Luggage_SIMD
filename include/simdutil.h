#include "vnrutil.h"
#include <stdio.h>
#include <stddef.h>
#include <ctype.h> 
#include <emmintrin.h>

#define sub_abs_epi8(a,b) _mm_or_si128(_mm_subs_epu8(a,b),_mm_subs_epu8(b,a))
#define sel_si128(vcontrole,a,b) _mm_or_si128(_mm_and_si128(vcontrole,a),_mm_andnot_si128(vcontrole,b))
#define _mm_cmplt_epu8(a,b) _mm_cmplt_epi8(_mm_sub_epi8(a, init_vuint8(128)), _mm_sub_epi8(b, init_vuint8(128)))
#define MAX_SSE(a,b) (sel_si128((vuint8)_mm_cmplt_epi8((__m128i)a,(__m128i)b),b,a))
#define MIN_SSE(a,b) (sel_si128((vuint8)_mm_cmplt_epi8((__m128i)a,(__m128i)b),a,b))

#define shift_right_add_prec_si128(a,b,imm) _mm_or_si128(_mm_slli_si128(a,imm),_mm_srli_si128(b,16-imm))
#define shift_left_add_next_si128(a,b,imm) _mm_or_si128(_mm_srli_si128(a,imm),_mm_slli_si128(b,16-imm))

void ReadPGMrowSIMD(FILE *file, long width, vuint8  *line);
void WritePGMrowSIMD(vuint8 *line, long width, FILE  *file);
vuint8** LoadPGM_vui8matrix(char *filename, long *nrl, long *nrh, long *ncl, long *nch);
void SavePGM_vui8matrix(vuint8 **m, long nrl, long nrh, long ncl, long nch, char *filename);
void mullo_epi8(vuint8 left, vuint8 right, vuint16* lo, vuint16* hi);

