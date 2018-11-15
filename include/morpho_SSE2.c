#include "morpho_SSE2.h"

void copy_vui8matrix_vui8matrix(vuint8 **X, int i0, int i1, int j0, int j1, vuint8 **Y)
{
    int i, j;

    for(i=i0; i<=i1; i++) {
        for(j=j0; j<=j1; j++) {
            Y[i][j] = X[i][j];
        }
    }
}

vuint8** dilatation_SSE(vuint8** M, int nrl, int nrh, int ncl, int nch, int dim)
{
  int r = dim/2;
  vuint8 max = init_vuint8((uint8)0);
  vuint8 zero = init_vuint8((uint8)0);
  vuint8 cmp0, cmp1, cmp2, cmp3;
  vuint8 vk, vp, vnrl, vncl, vnrh, vnch;

  vuint8** tmp = vui8matrix(nrl-r, nrh+r, ncl-r, nch+r);
  vuint8** output = vui8matrix(nrl, nrh, ncl, nch);
  copy_vui8matrix_vui8matrix(M, nrl, nrh, ncl, nch, tmp);
  
  for(int i = nrl; i < nrh; i++)
    {
      for(int j = ncl; j < nch; j++)
	{
	  for(int k = i-r; k < i+(r+1); k++)
	    {
	      for(int p = j-r; p < j+(r+1); p++)
		{
		  max =  _mm_max_epu8(tmp[k][p],max);
		}
	    }
	  _mm_store_si128((__m128i*)&output[i][j],max);
	  max = zero;
	}
    }
	
  return output;
}


vuint8** erosion_SSE(vuint8** M, int nrl, int nrh, int ncl, int nch, int dim)
{
  int r = dim/2;
  vuint8 min = init_vuint8((uint8)255);
  vuint8 nzero = init_vuint8((uint8)255);
  vuint8 cmp0, cmp1, cmp2, cmp3;
  vuint8 vk, vp, vnrl, vncl, vnrh, vnch;
  
  vuint8** tmp = vui8matrix(nrl-r, nrh+r, ncl-r, nch+r);
  vuint8** output = vui8matrix(nrl, nrh, ncl, nch);
  copy_vui8matrix_vui8matrix(M, nrl, nrh, ncl, nch, tmp);
  
   for(int i = 0; i < nrh; i++)
    {
      for(int j = 0; j < nch; j++)
	{
	  for(int k = i-r; k < i+(r+1); k++)
	    {
	      for(int p = j-r; p < j+(r+1); p++)
		{
		  min = _mm_min_epu8(tmp[k][p],min);
		}
	    }
	  _mm_store_si128((__m128i*)&output[i][j],min);
	  min = nzero;
	}
    }
   
   return output;
}

//Effectue une dilatation SSE3x3 element vertical en O(r*c*s)
vuint8** dilatation_SSE3x3_elemVertical(vuint8** M, int nrl, int nrh, int ncl, int nch) {
	vuint8** output = vui8matrix(nrl, nrh, ncl, nch);

	vuint8 elemStructa; //Premier pixel de l'élément structurant
	vuint8 elemStructb; //Deuxième pixel de l'élément structurant
	vuint8 elemStructc; //Troisième pixel de l'élément structurant

	//Step 1: Element structurant vertical 
	//Step 1.1: Prologue
	for(int j=0;j<nch;j++) {
		elemStructb = _mm_load_si128(&M[0][j]);
		elemStructc = _mm_load_si128(&M[1][j]);

		

	//Sauvegarde de l'opération dans la sortie
		_mm_store_si128(&output[0][j],
			_mm_or_si128(elemStructb, elemStructc)
		);
	}

	
	//Step 1.2: Corps de boucle
	for(int i = 1; i < nrh-1; i++)
  {
    for(int j = 0; j < nch; j++) {

			elemStructa = _mm_load_si128(&M[i-1][j]);
			elemStructb = _mm_load_si128(&M[i+0][j]);
			elemStructc = _mm_load_si128(&M[i+1][j]);

			//Je sauvegarde dans output
			_mm_store_si128(&output[i][j],
				_mm_or_si128(elemStructa, _mm_or_si128(elemStructa, elemStructc))
			);
		}
	}

	//Step 1.3: Epilogue de boucle

	for(int j=0;j<nch;j++) {
		elemStructb = _mm_load_si128(&M[nrh-2][j]);
		elemStructc = _mm_load_si128(&M[nrh-1][j]);

	//Sauvegarde de l'opération dans la sortie
		_mm_store_si128(&output[nrh-1][j],
			_mm_or_si128(elemStructb, elemStructc)
		);
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
		resToStore = _mm_or_si128((__m128i)_mm_slli_si128((__m128i)elemStructb,1),resToStore);

	//Sauvegarde de l'opération dans la sortie
		_mm_store_si128(&output[i][0],
			resToStore
		);
	}

	
	//Step 1.2: Corps de boucle
	for(int i = 0; i < nrh; i++)
  {
    for(int j = 1; j < (nch-1); j++) {
			elemStructa = _mm_load_si128(&M[i][j-1]);
			elemStructb = _mm_load_si128(&M[i][j+0]);
			elemStructc = _mm_load_si128(&M[i][j+1]);

			// (M[i][j+1] >> 15) + (M[i][j] << 1) + (M[i][j] >> 1) + (M[i][j-1] << 15) 
			
			//resToStore = (__m128i)_mm_srli_si128((__m128i)elemStructc,15);
			//resToStore = _mm_or_si128((__m128i)_mm_slli_si128((__m128i)elemStructb,1),resToStore);

			resToStore = shift_left_add_next_si128(elemStructb,elemStructc,1);

			temp = shift_right_add_prec_si128(elemStructb, elemStructa,1);

			//resToStore = _mm_or_si128((__m128i)_mm_slli_si128((__m128i)elemStructa,15),resToStore);
			
			resToStore = _mm_or_si128(temp,resToStore);

			//Je sauvegarde dans output
			_mm_store_si128(&output[i][j],
				resToStore
			);
		}
	}

	//Step 1.3: Sortie de boucle

	for(int i=0;i<nrh;i++) {
		elemStructa = _mm_load_si128(&M[i][nch-2]);
		elemStructb = _mm_load_si128(&M[i][nch-1]);

		// (M[i][nch-2] << 15) + (M[i][nch-1] << 1) + (M[i][nch-1] >> 1)
		//resToStore = (__m128i)_mm_slli_si128((__m128i)elemStructa,15);
		//resToStore = _mm_or_si128((__m128i)_mm_slli_si128((__m128i)elemStructb,1),resToStore);

		resToStore = shift_right_add_prec_si128(elemStructb,elemStructa,1);
		resToStore = _mm_or_si128((__m128i)_mm_srli_si128((__m128i)elemStructb,1),resToStore);

	//Sauvegarde de l'opération dans la sortie
		_mm_store_si128(&output[i][nch-1],
			resToStore
		);
	}

	return output;

}


//Effectue une erosion SSE3x3 element vertical en O(r*c*s)
vuint8** erosion_SSE3x3_elemVertical(vuint8** M, int nrl, int nrh, int ncl, int nch) {
	vuint8** output = vui8matrix(nrl, nrh, ncl, nch);

	vuint8 elemStructa; //Premier pixel de l'élément structurant
	vuint8 elemStructb; //Deuxième pixel de l'élément structurant
	vuint8 elemStructc; //Troisième pixel de l'élément structurant

	//Step 1: Element structurant vertical 
	//Step 1.1: Prologue
	for(int j=0;j<nch;j++) {
		elemStructb = _mm_load_si128(&M[0][j]);
		elemStructc = _mm_load_si128(&M[1][j]);

	//Sauvegarde de l'opération dans la sortie
		_mm_store_si128(&output[0][j],
			_mm_and_si128(elemStructb, elemStructc)
		);
	}

	
	//Step 1.2: Corps de boucle
	for(int i = 1; i < nrh-1; i++)
  {
    for(int j = 0; j < nch; j++) {
			elemStructa = _mm_load_si128(&M[i-1][j]);
			elemStructb = _mm_load_si128(&M[i+0][j]);
			elemStructc = _mm_load_si128(&M[i+1][j]);

			//Je sauvegarde dans output
			_mm_store_si128(&output[i][j],
				_mm_and_si128(elemStructa, _mm_and_si128(elemStructa, elemStructc))
			);
		}
	}

	//Step 1.3: Sortie de boucle

	for(int j=0;j<nch;j++) {
		elemStructb = _mm_load_si128(&M[nrh-2][j]);
		elemStructc = _mm_load_si128(&M[nrh-1][j]);

	//Sauvegarde de l'opération dans la sortie
		_mm_store_si128(&output[nrh-1][j],
			_mm_and_si128(elemStructb, elemStructc)
		);
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

		// [(M[i][1] >> 15) + (M[i][0] << 1)] x (M[i][nch-1] >> 1)  
		//resToStore = (__m128i)_mm_srli_si128((__m128i)elemStructc,15);
		//resToStore = _mm_or_si128((__m128i)_mm_slli_si128((__m128i)elemStructb,1),resToStore);
		resToStore = shift_left_add_next_si128(elemStructb,elemStructc,1);
		resToStore = _mm_and_si128((__m128i)_mm_slli_si128((__m128i)elemStructb,1),resToStore);
	//Sauvegarde de l'opération dans la sortie
		_mm_store_si128(&output[i][0],
			resToStore
		);
	}

	
	//Step 1.2: Corps de boucle
	for(int i = 0; i < nrh; i++)
  {
    for(int j = 1; j < (nch-1); j++) {

			elemStructa = _mm_load_si128(&M[i][j-1]);
			elemStructb = _mm_load_si128(&M[i][j+0]);
			elemStructc = _mm_load_si128(&M[i][j+1]);

			// [(M[i][j+1] >> 15) + (M[i][j] << 1)] x [(M[i][j] >> 1) + (M[i][j-1] << 15)] 
			//resToStore = (__m128i)_mm_srli_si128((__m128i)elemStructc,15);
			//resToStore = _mm_or_si128((__m128i)_mm_slli_si128((__m128i)elemStructb,1),resToStore);
			resToStore = shift_left_add_next_si128(elemStructb,elemStructc,1);

			//temp = (__m128i)_mm_slli_si128((__m128i)elemStructa,15);
			//temp = _mm_or_si128((__m128i)_mm_srli_si128((__m128i)elemStructb,1),temp);
			temp = shift_right_add_prec_si128(elemStructb,elemStructa,1);

			resToStore = _mm_and_si128(resToStore,temp);

			//Je sauvegarde dans output
			_mm_store_si128(&output[i][j],
				resToStore
			);
		}
	}

	//Step 1.3: Sortie de boucle

	for(int i=0;i<nrh;i++) {
		elemStructa = _mm_load_si128(&M[i][nch-2]);
		elemStructb = _mm_load_si128(&M[i][nch-1]);

		// [(M[i][nch-2] << 15) + (M[i][nch-1] >> 1)] x (M[i][nch-1] << 1) 
		//resToStore = (__m128i)_mm_slli_si128((__m128i)elemStructa,15);
		//resToStore = _mm_or_si128((__m128i)_mm_srli_si128((__m128i)elemStructb,1),resToStore);
		resToStore = shift_right_add_prec_si128(elemStructb,elemStructa,1);
		resToStore = _mm_and_si128((__m128i)_mm_srli_si128((__m128i)elemStructb,1),resToStore);
		

	//Sauvegarde de l'opération dans la sortie
		_mm_store_si128(&output[i][nch-1],
			resToStore
		);
	}

	return output;

}

vuint8** fermeture_SSE(vuint8** M, int nrl, int nrh, int ncl, int nch, int dim)
{
  vuint8** output;
  //output = dilatation_SSE(M, nrl, nrh, ncl, nch, dim);
  //output = erosion_SSE(output, nrl, nrh, ncl, nch, dim);

	output = dilatation_SSE3x3_elemVertical(M, nrl, nrh, ncl, nch);
	output = dilatation_SSE3x3_elemHorizontal(output, nrl, nrh, ncl, nch);
	output = erosion_SSE3x3_elemVertical(output, nrl, nrh, ncl, nch);
	output = erosion_SSE3x3_elemHorizontal(output, nrl, nrh, ncl, nch);

  return output;
}

vuint8** ouverture_SSE(vuint8** M, int nrl, int nrh, int ncl, int nch, int dim)
{
  vuint8** output;
  //output = erosion_SSE(M, nrl, nrh, ncl, nch, dim);
  //output = dilatation_SSE(output, nrl, nrh, ncl, nch, dim);

	
	output = erosion_SSE3x3_elemVertical(M, nrl, nrh, ncl, nch);
	output = erosion_SSE3x3_elemHorizontal(output, nrl, nrh, ncl, nch);
	output = dilatation_SSE3x3_elemVertical(output, nrl, nrh, ncl, nch);
	output = dilatation_SSE3x3_elemHorizontal(output, nrl, nrh, ncl, nch);

  return output;
}
