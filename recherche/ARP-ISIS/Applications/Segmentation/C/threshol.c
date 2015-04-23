/*************************************
 *
 *	Double seuillage
 *
 ************************************/
#include <stdio.h>
#include "tt_type.h"
#include "algo.h"

extern unsigned int LowLevel;
extern unsigned int HighLevel;
/***********************/
/* Creation d'un cadre */
/***********************/
//inline
void create_border( TT_Image *Timage )
{
	register unsigned int lig, col;
	
		/* image */
	TT_type *image 	= 			Timage->ptrImage;
	unsigned int frame_height = Timage->Height;
	unsigned int frame_width = 	Timage->Width;
	unsigned int rs = 			Timage->Stride;
	unsigned int inc = 			Timage->Increment;

	TRACE("Creation du bord\n");

	// Creation d'un cadre noir
		// met a CONTOUR la 2eme et avant dernier colonne
	for( lig=0 ; lig<frame_height; lig++ )  {
			// 2 et avant derniere colonne
		image[ lig*rs + inc ] = CONTOUR;
		image[ lig*rs + (frame_width-2)*inc ] = CONTOUR;
	}
			// met a CONTOUR la 2eme et avant derniere ligne		
	for( col=0 ; col<frame_width*inc; col+=inc ) {
			// 2 et avant derniere ligne
		image[ rs + col ] = CONTOUR;
		image[ (frame_height-2)*rs + col ] = CONTOUR;
	}

	// Met les bords de l'image a BACK_GND		
		// met a BACK_GND la premiere et la derniere colonne
	for( lig=0 ; lig<frame_height; lig++ )  {
			// 1 et derniere colonne
		image[ lig*rs ] = BACK_GND;
		image[ lig*rs + (frame_width-1)*inc ] = BACK_GND;
	}
		// met a BACK_GND la 1er et la derniere ligne
	for( col=0 ; col<frame_width*inc; col+=inc ) {
			// 1 et derniere ligne
		image[ col ] = BACK_GND;
		image[ (frame_height-1)*rs + col ] = BACK_GND;
	}
}
 
//inline
void Double_Threshold( TT_Image *Timage )
{
	register unsigned int lig, col; 
	register TT_type pixel;
	
		/* image */
	TT_type *image 	= 			Timage->ptrImage;
	unsigned int frame_height = Timage->Height;
	unsigned int frame_width = 	Timage->Width;
	unsigned int rs = 			Timage->Stride;
	unsigned int inc = 			Timage->Increment;

	TRACE("Double seuillage\n");
	
 	for( lig = 0; lig< frame_height ; lig++ ) {
 		for( col=0 ; col<frame_width*inc ; col+=inc ) {
 			pixel 	= image[lig*rs + col];
			if( pixel<LowLevel ) image[lig*rs + col]=LOW_VAL ;
			else if( pixel>HighLevel ) image[lig*rs + col]=HIGH_VAL ;
 			else image[lig*rs + col]=MIDDLE_VAL;
 		}	
	}
}
