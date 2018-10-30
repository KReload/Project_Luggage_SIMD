void dilatation(char *filename1) {
  long nrl,nrh,ncl,nch;

  //Image à dilater
  uint8** img = LoadPGM_ui8matrix(filename1,&nrl,&nrh,&ncl,&nch);

  //Element structurant
  uint8 elemStruct = ui8matrix(-1,+1,-1,+1);

  //Allocation sortie de taille (r+2)*(c+2) pour avoir une bordure
  uint8** output = ui8matrix(nrl-1,nrh+1,ncl-1,nch+1);

  for(int i = 0;i<nrh;i++) {
    for(int j = 0;j<nch;j++) {
      output[i][j] = 0;
    }
  }

}

//A completer
