#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "tt_type.h"
#include "algo.h"

#ifndef _UNIX_
#include "norme.c"	// c'est deguelasse, je sais, mais sinon le linker a une
#endif				// ligne de command trop longue !!!!!!!!( sous DOS, forcement !!! )

#ifdef _DSP_
inline
#endif
void Deriche( TT_Image *Timage , float alpha )
{
	float gamma;
	unsigned int col, lig;

	register TT_type pixel;
	register float y, y_1, y_2;
	register float a1, b1, b2;

	int SV, SH;

		/* image */
	TT_type *image 	= 			Timage->ptrImage;
	unsigned int frame_height = Timage->Height;
	unsigned int frame_width = 	Timage->Width;
	unsigned int rs = 			Timage->Stride;
	unsigned int inc = 			Timage->Increment;

	TRACE("Deriche\n");

	/* Calcul des coeff */
	gamma = exp(-alpha);	
	a1 = Carre( 1 - gamma );
	b1 = 2 * gamma;
	b2 = -Carre( gamma );

#ifdef DEBUG
	printf("   alpha=%f\n", alpha );
	printf("   gamma=%f\n", gamma );
	printf("   a1=%f\n", a1 );
	printf("   b1=%f\n", b1 );
	printf("   b2=%f\n", b2 );
#endif

/***********/	
/* Lissage */
/***********/
	TRACE("   Lissage\n");
	y_1 = 0;		// y[n-1] 
	y_2 = 0;		// y[n-2]	
	/*** Calcul HORIZONTAL ***/
	for( lig = 0; lig< frame_height ; lig++ ) {
		/* Causal */
			/* Traitement */
		for( col=0 ; col<frame_width*inc ; col+=inc ) {
			pixel 	= image[lig*rs + col];
			pixel 	= (TT_type)(a1*pixel + b1*y_1 + b2*y_2);		
			y 		= pixel;
			y_2 	= y_1;
			y_1 	= y;
			image[lig*rs + col] = pixel;
		}	

		/* Anti-causal */		
			/* Traitement */
		for( col=frame_width*inc ; col<=0 ; col-=inc ) {
			pixel 	= image[lig*rs + col];
			pixel 	= (TT_type)(a1*pixel + b1*y_1 + b2*y_2);		
			y 		= pixel;
			y_2 	= y_1;
			y_1 	= y;
			image[lig*rs + col] = pixel;
		}	
	}

	/*** Calcul VERTICAL ***/
	for( col = 0; col<frame_width*inc ; col+=inc ) {	
		/* Causal */
			/* Traitement */
		for( lig=0 ; lig<frame_height ; lig++ ) {
			pixel 	= image[lig*rs + col];
			pixel 	= (TT_type)(a1*pixel + b1*y_1 + b2*y_2);		
			y 		= pixel;
			y_2 	= y_1;
			y_1 	= y;
			image[lig*rs + col] = pixel;
		}	

		/* Anti-causal */		
			/* Traitement */
		for( lig=frame_height ; lig<=0 ; lig-- ) {
			pixel 	= image[lig*rs + col];
			pixel 	= (TT_type)(a1*pixel + b1*y_1 + b2*y_2);		
			y 		= pixel;
			y_2 	= y_1;
			y_1 	= y;
			image[lig*rs + col] = pixel;
		}
	}

/************/
/* Gradient */
/************/
	TRACE("   Gradient\n");
		// Ne pas traiter la derniere ligne, cela ferait une
		// lecture hors memoire.
	for( lig=0 ; lig<(frame_height-1); lig++ )
		for( col=0 ; col<(frame_width-1)*inc; col+=inc ) {
			SH = 	-image[ lig*rs + col ]+image[ lig*rs + col+inc ]
				-image[ (lig+1)*rs + col ]+image[ (lig+1)*rs + col+inc ];
			SV = 	-image[ lig*rs + col ]-image[ lig*rs + col+inc ]
				+image[ (lig+1)*rs + col ]+image[ (lig+1)*rs + col+inc ];
			image[ lig*rs + col ] = (TT_type)sqrt(SH*SH+SV*SV);
		}
		
/***********************************/
/* Suppression des bords parasites */
/***********************************/
	TRACE("   Suppression des bords parasites\n");
		// met a 0 la premiere et la derniere colonne
	for( lig=0 ; lig<frame_height; lig++ )  {
		image[ lig*rs ] = 0;
		image[ lig*rs + (frame_width-1)*inc ] = 0;
	}

		// met a 0 la 1er et la derniere ligne
	for( col=0 ; col<frame_width*inc; col+=inc ) {
		image[ col ] = 0;
		image[ (frame_height-1)*rs + col ] = 0;
	}

//	Normalisation( Timage );
}
