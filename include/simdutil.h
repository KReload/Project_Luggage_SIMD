


void ReadPGMrowSIMD(FILE *file, long width, vuint8  *line);
void WritePGMrowSIMD(vuint8 *line, long width, FILE  *file);
vuint8** LoadPGM_vui8matrix(char *filename, long *nrl, long *nrh, long *ncl, long *nch);
void SavePGM_vui8matrix(vuint8 **m, long nrl, long nrh, long ncl, long nch, char *filename);