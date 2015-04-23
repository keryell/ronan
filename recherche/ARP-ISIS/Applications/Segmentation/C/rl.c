/**********************************************
 *
 * Etiquetage des regions
 *
 **********************************************/
#include <stdio.h>

#include "algo.h"
#include "tt_type.h" 

static unsigned int EqR[4096];
 
void region_labelling( TT_Image *Timage )
{
	register unsigned int lig, col;
	unsigned int a, b, c;
	unsigned int RefR, Wr;
	int k;

		/* image */
	TT_type *image 	= 			Timage->ptrImage;
	unsigned int frame_height = Timage->Height;
	unsigned int frame_width = 	Timage->Width;
	unsigned int rs = 			Timage->Stride;
	unsigned int inc = 			Timage->Increment;

	TRACE("Etiquetage des regions\n");

	Wr = 0;
	RefR = 3;

	// 		a
	//		c	b
	for( lig = 1; lig< frame_height ; lig++ )
		for( col=1 ; col<frame_width*inc ; col+=inc ) {
			b = image[lig*rs + col ];
			a = image[(lig-1)*rs + col     ];
			c = image[lig*rs     + col-inc ];
		
			if( Wr==0 ) {
				if( b==CONTOUR ) {							// cas 1
					image[lig*rs + col ] = b;
				}
				else if( a==CONTOUR && c==CONTOUR )	{	// cas 2
					Wr = 1;
					image[lig*rs + col ] = RefR;
					EqR[RefR] = RefR;
				}
				else if( a>2 && c==CONTOUR ) {	// cas 3
					image[lig*rs + col ] = EqR[a];
				}
				else if( a==CONTOUR && c>2 ) {	// cas 4
					image[lig*rs + col ] = c;
				}
				else {											// cas 5
					if( EqR[a]<c ) {
						image[lig*rs + col ] = EqR[a];
						EqR[c]=EqR[a];
					}
					else {
						image[lig*rs + col ] = c;
						EqR[a] = c;
					}
				}
			}  // if Wr=0
			else {
				if( b==CONTOUR ) {						// cas 6
					image[lig*rs + col ] = b;
					RefR++;
					Wr = 0;
				}
				else if( a>2 ) {				// cas 7
					for( k=Wr ; k>=0; k-- ) image[lig*rs + (col-k*inc) ] = EqR[a];
					Wr = 0;
				}
				else {										// cas 8
					image[lig*rs + col ] = RefR;
					if( Wr<WR_MAX ) {
						Wr++;
					}
					else {
						RefR++;
						Wr = 0;
					}
				}
			} // else Wr=0				
		} // for col		

#ifdef _DEBUG_
	printf("RefR=%u\n", RefR);
#endif

	// Resolution des equivalences
	TRACE("Resolution des equivalences\n");
	for( lig = 0; lig< frame_height ; lig++ )
		for( col=0 ; col<frame_width*inc ; col+=inc ) {
			b = image[lig*rs + col ];
			if( b>1 ) {
				image[lig*rs + col ] = EqR[ EqR[b] ];
			}
		} // for col
}
