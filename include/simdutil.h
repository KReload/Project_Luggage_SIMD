
#define sub_abs_epi8(a,b) _mm_or_si128(_mm_subs_epu8(a,b),_mm_subs_epu8(b,a))
#define sel_si128(vcontrole,a,b) _mm_or_si128(_mm_and_si128(vcontrole,a),_mm_andnot_si128(vcontrole,b))


void ReadPGMrowSIMD(FILE *file, long width, vuint8  *line);
void WritePGMrowSIMD(vuint8 *line, long width, FILE  *file);
vuint8** LoadPGM_vui8matrix(char *filename, long *nrl, long *nrh, long *ncl, long *nch);
void SavePGM_vui8matrix(vuint8 **m, long nrl, long nrh, long ncl, long nch, char *filename);
void mullo_epi8(vuint8 left, vuint8 right, vuint16* lo, vuint16* hi);

