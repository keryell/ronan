#include <stdio.h>
#include "algo.h"
#include "tt_type.h"


/*****************/
/* Normalisation */
/*****************/
//inline
void Normalisation( TT_Image *Timage )
{
	unsigned int col, lig;
	int min, max;

		/* image */
	TT_type *image 	= 			Timage->ptrImage;
	unsigned int frame_height = Timage->Height;
	unsigned int frame_width = 	Timage->Width;
	unsigned int rs = 			Timage->Stride;
	unsigned int inc = 			Timage->Increment;


	TRACE("Normalisation\n");

/* Normalisation sur 256 niveaux de gris*/
  /* Recherche du min et du max */
	min = 100000;
	max = 0;
	for (lig=0; lig<frame_height; lig++)
		for (col=0; col<frame_width*inc; col+=inc) {
			if (image[ lig * rs + col ] < min) min = image[ lig * rs + col ];
    		if (image[ lig * rs + col ] > max) max = image[ lig * rs + col ];
	}
#ifdef DEBUG
	printf("Min=%d\tMax=%d\n", min, max);
#endif
  /* Normalisation de l'image */
  for (lig=0; lig<frame_height; lig++)
    for (col=0; col<frame_width*inc; col+=inc ) {
      	image[ lig * rs + col ] = (TT_type)((image[ lig * rs + col ]-min)*255)/(max-min);
	}
}


void BidouilleNico( TT_Image *Timage )
{
	unsigned int col, lig;
	int min, max;
	TT_type pixel;

		/* image */
	TT_type *image 	= 			Timage->ptrImage;
	unsigned int frame_height = Timage->Height;
	unsigned int frame_width = 	Timage->Width;
	unsigned int rs = 			Timage->Stride;
	unsigned int inc = 			Timage->Increment;

	TRACE("BidouilleNico\n");

/* Normalisation sur 256 niveaux de gris*/
  /* Recherche du min et du max */
	min = 100000;
	max = 0;
	for (lig=0; lig<frame_height; lig++)
		for (col=0; col<frame_width*inc; col+=inc) {
			if (image[ lig * rs + col ] < min) min = image[ lig * rs + col ];
    		if (image[ lig * rs + col ] > max) max = image[ lig * rs + col ];
	}
#ifdef DEBUG
	printf("Min=%d\tMax=%d\n", min, max);
#endif
  /* Normalisation de l'image */
  for (lig=0; lig<frame_height; lig++)
    for (col=0; col<frame_width*inc; col+=inc ) {
		pixel = image[ lig * rs + col ];
		if( pixel>1 ) {
			if( (((pixel-min)*255)/(max-min))>6 )
      			image[ lig * rs + col ] =(TT_type)((pixel-min)*255)/(max-min); 
			image[ lig * rs + col ]<<=5; 
		}
	}
}
