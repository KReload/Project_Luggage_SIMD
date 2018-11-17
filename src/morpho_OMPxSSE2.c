#include "../include/morpho_SSE2.h"

//Effectue une dilatation SSE3x3 element vertical en O(r*c*s)
vuint8** dilatation_SSE3x3_elemVertical(vuint8** M, int nrl, int nrh, int ncl, int nch) {
  vuint8** output = vui8matrix(nrl, nrh, ncl, nch);

  vuint8 elemStructa; //Premier pixel de l'élément structurant
  vuint8 elemStructb; //Deuxième pixel de l'élément structurant
  vuint8 elemStructc; //Troisième pixel de l'élément structurant

  vuint8 temp;

  //Step 1: Element structurant vertical 
  //Step 1.1: Prologue
  for(int j=0;j<nch;j++) {
    elemStructb = _mm_load_si128(&M[0][j]);
    elemStructc = _mm_load_si128(&M[1][j]);
	
	  temp = _mm_or_si128(elemStructb, elemStructc);

    //Sauvegarde de l'opération dans la sortie
    _mm_store_si128(&output[0][j],temp);
  }

	
  //Step 1.2: Corps de boucle
  for(int i = 1; i < nrh-1; i++)
    {
      for(int j = 0; j < nch; j++) {

        elemStructa = _mm_load_si128(&M[i-1][j]);
        elemStructb = _mm_load_si128(&M[i+0][j]);
        elemStructc = _mm_load_si128(&M[i+1][j]);

        temp = _mm_or_si128(elemStructa, _mm_or_si128(elemStructb, elemStructc));
        //Je sauvegarde dans output
        _mm_store_si128(&output[i][j],temp);
      }
    }

  //Step 1.3: Epilogue de boucle

  for(int j=0;j<nch;j++) {
    elemStructa = _mm_load_si128(&M[nrh-2][j]);
    elemStructb = _mm_load_si128(&M[nrh-1][j]);

	  temp = _mm_or_si128(elemStructa, elemStructb);

    //Sauvegarde de l'opération dans la sortie
    _mm_store_si128(&output[nrh-1][j],temp);
  }

  return output;
}

//Effectue une dilatation SSE3x3 element horizontal en O(r*c*s)
vuint8** dilatation_SSE3x3_elemHorizontal(vuint8** M, int nrl, int nrh, int ncl, int nch) {
  vuint8** output = vui8matrix(nrl, nrh, ncl, nch);

  vuint8 elemStructa; //Premier pixel de l'élément structurant
  vuint8 elemStructb; //Deuxième pixel de l'élément structurant
  vuint8 elemStructc; //Troisième pixel de l'élément structurant

  vuint8 temp;

  vuint8 resToStore;
  //Step 1: Element structurant vertical 
  //Step 1.1: Prologue
  for(int i=0;i<nrh;i++) {
    elemStructb = _mm_load_si128(&M[i][0]);
    elemStructc = _mm_load_si128(&M[i][1]);

    // (M[i][1] >> 15) + (M[i][0] << 1) + (M[i][0] >> 1)  
    //resToStore = (__m128i)_mm_srli_si128((__m128i)elemStructc,15);
    //resToStore = _mm_or_si128((__m128i)_mm_slli_si128((__m128i),1),resToStore);
    resToStore = shift_left_add_next_si128(elemStructb,elemStructc,1);
    resToStore = _mm_or_si128(_mm_slli_si128(elemStructb,1),resToStore);
	  resToStore = _mm_or_si128(resToStore,elemStructb);

    //Sauvegarde de l'opération dans la sortie
    _mm_store_si128(&output[i][0],resToStore);
  }

	
  //Step 1.2: Corps de boucle
  for(int i = 0; i < nrh; i++)
    {
      for(int j = 1; j < (nch-1); j++) {
        elemStructa = _mm_load_si128(&M[i][j-1]);
        elemStructb = _mm_load_si128(&M[i][j+0]);
        elemStructc = _mm_load_si128(&M[i][j+1]);

        // (M[i][j+1] >> 15) + (M[i][j] << 1) + (M[i][j] >> 1) + (M[i][j-1] << 15) + M[i][j]
            
        //resToStore = (__m128i)_mm_srli_si128((__m128i)elemStructc,15);
        //resToStore = _mm_or_si128((__m128i)_mm_slli_si128((__m128i)elemStructb,1),resToStore);

        resToStore = shift_left_add_next_si128(elemStructb,elemStructc,1);

        temp = shift_right_add_prec_si128(elemStructb, elemStructa,1);

        //resToStore = _mm_or_si128((__m128i)_mm_slli_si128((__m128i)elemStructa,15),resToStore);
            
        resToStore = _mm_or_si128(temp,resToStore);
        resToStore = _mm_or_si128(resToStore, elemStructb);


        //Je sauvegarde dans output
        _mm_store_si128(&output[i][j],resToStore);
      }
    }

  //Step 1.3: Sortie de boucle

  for(int i=0;i<nrh;i++) {
    elemStructa = _mm_load_si128(&M[i][nch-2]);
    elemStructb = _mm_load_si128(&M[i][nch-1]);

    // (M[i][nch-2] << 15) + (M[i][nch-1] << 1) + (M[i][nch-1] >> 1) + M[i][j]

    resToStore = shift_right_add_prec_si128(elemStructb,elemStructa,1);
    resToStore = _mm_or_si128(_mm_srli_si128(elemStructb,1),resToStore);
	  resToStore = _mm_or_si128(resToStore,elemStructb);

    //Sauvegarde de l'opération dans la sortie
    _mm_store_si128(&output[i][nch-1],resToStore);
  }

  return output;

}


//Effectue une erosion SSE3x3 element vertical en O(r*c*s)
vuint8** erosion_SSE3x3_elemVertical(vuint8** M, int nrl, int nrh, int ncl, int nch) {
  vuint8** output = vui8matrix(nrl, nrh, ncl, nch);

  vuint8 elemStructa; //Premier pixel de l'élément structurant
  vuint8 elemStructb; //Deuxième pixel de l'élément structurant
  vuint8 elemStructc; //Troisième pixel de l'élément structurant

  vuint8 temp;

  //Step 1: Element structurant vertical 
  //Step 1.1: Prologue
  for(int j=0;j<nch;j++) {
    elemStructb = _mm_load_si128(&M[0][j]);
    elemStructc = _mm_load_si128(&M[1][j]);

	  temp = _mm_and_si128(elemStructb, elemStructc);
    //Sauvegarde de l'opération dans la sortie
    _mm_store_si128(&output[0][j],temp);
  }

	
  //Step 1.2: Corps de boucle
  for(int i = 1; i < nrh-1; i++)
    {
      for(int j = 0; j < nch; j++) {
      elemStructa = _mm_load_si128(&M[i-1][j]);
      elemStructb = _mm_load_si128(&M[i+0][j]);
      elemStructc = _mm_load_si128(&M[i+1][j]);

      temp = _mm_and_si128(elemStructa, _mm_and_si128(elemStructb, elemStructc));

      //Je sauvegarde dans output
      _mm_store_si128(&output[i][j],temp);
      }
    }

  //Step 1.3: Sortie de boucle

  for(int j=0;j<nch;j++) {
    elemStructb = _mm_load_si128(&M[nrh-2][j]);
    elemStructc = _mm_load_si128(&M[nrh-1][j]);

	  temp = _mm_and_si128(elemStructb, elemStructc);
    //Sauvegarde de l'opération dans la sortie
    _mm_store_si128(&output[nrh-1][j],temp);
  }

  return output;
}

//Effectue une erosion SSE3x3 element horizontal en O(r*c*s)
vuint8** erosion_SSE3x3_elemHorizontal(vuint8** M, int nrl, int nrh, int ncl, int nch) {
  vuint8** output = vui8matrix(nrl, nrh, ncl, nch);


  vuint8 elemStructa; //Premier pixel de l'élément structurant
  vuint8 elemStructb; //Deuxième pixel de l'élément structurant
  vuint8 elemStructc; //Troisième pixel de l'élément structurant

  vuint8 temp;

  vuint8 resToStore;
  //Step 1: Element structurant vertical 
  //Step 1.1: Prologue
  for(int i=0;i<nrh;i++) {
		
    elemStructb = _mm_load_si128(&M[i][0]);
    elemStructc = _mm_load_si128(&M[i][1]);

    // [(M[i][1] >> 15) + (M[i][0] << 1)] x (M[i][nch-1] >> 1) x M[i][j] 
	
    resToStore = shift_left_add_next_si128(elemStructb,elemStructc,1);
    resToStore = _mm_and_si128(_mm_slli_si128(elemStructb,1),resToStore);
	  resToStore = _mm_and_si128(resToStore,elemStructb);
    //Sauvegarde de l'opération dans la sortie
    _mm_store_si128(&output[i][0],resToStore);
  }

	
  //Step 1.2: Corps de boucle
  for(int i = 0; i < nrh; i++)
    {
      for(int j = 1; j < (nch-1); j++) {

        elemStructa = _mm_load_si128(&M[i][j-1]);
        elemStructb = _mm_load_si128(&M[i][j+0]);
        elemStructc = _mm_load_si128(&M[i][j+1]);

        // [(M[i][j+1] >> 15) + (M[i][j] << 1)] x [(M[i][j] >> 1) + (M[i][j-1] << 15)] x M[i][j]
        
        resToStore = shift_left_add_next_si128(elemStructb,elemStructc,1);

        temp = shift_right_add_prec_si128(elemStructb,elemStructa,1);

        resToStore = _mm_and_si128(resToStore,temp);
        resToStore = _mm_and_si128(resToStore, elemStructb);

        //Je sauvegarde dans output
        _mm_store_si128(&output[i][j],resToStore);
      }
    }

  //Step 1.3: Sortie de boucle

  for(int i=0;i<nrh;i++) {
    elemStructa = _mm_load_si128(&M[i][nch-2]);
    elemStructb = _mm_load_si128(&M[i][nch-1]);
	

    // [(M[i][nch-2] << 15) + (M[i][nch-1] >> 1)] x (M[i][nch-1] << 1)  x M[i][j]
    //resToStore = (__m128i)_mm_slli_si128((__m128i)elemStructa,15);
    //resToStore = _mm_or_si128((__m128i)_mm_srli_si128((__m128i)elemStructb,1),resToStore);

    resToStore = shift_right_add_prec_si128(elemStructb,elemStructa,1);
    resToStore = _mm_and_si128(_mm_srli_si128(elemStructb,1),resToStore);
	  resToStore = _mm_and_si128(resToStore,elemStructb);
	
		

    //Sauvegarde de l'opération dans la sortie
    _mm_store_si128(&output[i][nch-1],resToStore);
  }

  return output;

}

vuint8** fermeture_SSE(vuint8** M, int nrl, int nrh, int ncl, int nch, int dim)
{
  vuint8** output;


  
  
  //Elem structurant 5x5
  if(dim == 5) {
    output = dilatation_SSE3x3_elemVertical(M, nrl, nrh, ncl, nch);
    output = dilatation_SSE3x3_elemHorizontal(output, nrl, nrh, ncl, nch);
    output = dilatation_SSE3x3_elemVertical(output, nrl, nrh, ncl, nch);
    output = dilatation_SSE3x3_elemHorizontal(output, nrl, nrh, ncl, nch);
    output = erosion_SSE3x3_elemVertical(output, nrl, nrh, ncl, nch);
    output = erosion_SSE3x3_elemHorizontal(output, nrl, nrh, ncl, nch);
    output = erosion_SSE3x3_elemVertical(output, nrl, nrh, ncl, nch);
    output = erosion_SSE3x3_elemHorizontal(output, nrl, nrh, ncl, nch);
  } else {
    output = dilatation_SSE3x3_elemVertical(M, nrl, nrh, ncl, nch);
    output = dilatation_SSE3x3_elemHorizontal(output, nrl, nrh, ncl, nch);
    output = erosion_SSE3x3_elemVertical(output, nrl, nrh, ncl, nch);
    output = erosion_SSE3x3_elemHorizontal(output, nrl, nrh, ncl, nch);
  }

  return output;
}

vuint8** ouverture_SSE(vuint8** M, int nrl, int nrh, int ncl, int nch, int dim)
{
  vuint8** output;
	
  //Elem structurant 5x5
  if(dim == 5) {
    output = erosion_SSE3x3_elemVertical(M, nrl, nrh, ncl, nch);
    output = erosion_SSE3x3_elemHorizontal(output, nrl, nrh, ncl, nch);
    output = erosion_SSE3x3_elemVertical(output, nrl, nrh, ncl, nch);
    output = erosion_SSE3x3_elemHorizontal(output, nrl, nrh, ncl, nch);
    output = dilatation_SSE3x3_elemVertical(output, nrl, nrh, ncl, nch);
    output = dilatation_SSE3x3_elemHorizontal(output, nrl, nrh, ncl, nch);
    output = dilatation_SSE3x3_elemVertical(output, nrl, nrh, ncl, nch);
    output = dilatation_SSE3x3_elemHorizontal(output, nrl, nrh, ncl, nch);
  } else {
    output = erosion_SSE3x3_elemVertical(M, nrl, nrh, ncl, nch);
    output = erosion_SSE3x3_elemHorizontal(output, nrl, nrh, ncl, nch);
    output = dilatation_SSE3x3_elemVertical(output, nrl, nrh, ncl, nch);
    output = dilatation_SSE3x3_elemHorizontal(output, nrl, nrh, ncl, nch);
  }

  return output;
}
