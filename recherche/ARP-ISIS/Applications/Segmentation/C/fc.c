/*************************************
 *
 *	Fermeture des contours
 *
 ************************************/
#include <stdio.h>
#include <stdlib.h>
#include "tt_type.h"
#include "algo.h"

static unsigned int EqC[4096];
 
// Resolution des equivalences
void connect( TT_Image *Timage )
{
	unsigned int lig, col;
	unsigned int b1, b2;	
	unsigned int pixel;

		/* image */
	TT_type *image 	= 			Timage->ptrImage;
	unsigned int frame_height = Timage->Height;
	unsigned int frame_width = 	Timage->Width;
	unsigned int rs = 			Timage->Stride;
	unsigned int inc = 			Timage->Increment;

	TRACE("Resolution des equivalences\n");

	for( lig = 0; lig<frame_height ; lig++ )
		for( col=0 ; col<frame_width*inc ; col+=inc ) {
			pixel = image[lig*rs + col];
			if( pixel>2 ) {
				b1=EqC[pixel];
				if( b1==pixel ) image[lig*rs + col]=BACK_GND;
				else {
					b2=EqC[b1];
					if( b2>2 ) image[lig*rs + col]=BACK_GND;
					else image[lig*rs + col]=CONTOUR;
				}
			}
			else if( image[lig*rs + col]==EXTREM ) image[lig*rs + col]=CONTOUR;
		}
}


/** Detection des extremites  **/
/** Utilisation des 8 voisins **/
/** Permet de resoudre le pb de contour epais **/
void contour_closing( TT_Image *Timage )
{
	unsigned int lig, col;
	unsigned int a, b, c, d, e;
	unsigned int a0, e0, c0, d0;
	unsigned int a1, a2, e1, e2;
	unsigned int all8;
	unsigned int li1, li2, li3, co1, co2, co3;
	unsigned int cas3;
	unsigned int RefC, Wc;
	unsigned int b1, b2;		
	unsigned int pixel;
	int k;

		/* image */
	TT_type *image 	= 			Timage->ptrImage;
	unsigned int frame_height = Timage->Height;
	unsigned int frame_width = 	Timage->Width;
	unsigned int rs = 			Timage->Stride;
	unsigned int inc = 			Timage->Increment;

	TRACE("Fermeture des contours\n");

	RefC = 3;	// Etiquette courante
	Wc   = 0;	// delai = 0
	cas3 = 0;

	// 8-voisins
	// 		a1	  a		a2
	//		c	  b		d
	//		e1	  e		e2
	// il faut commencer a 1 et finir taille-1 pour eviter de lire en dehors de
	// la memoire.
	for( lig = 1; lig< frame_height-1 ; lig++ )
		for( col=1 ; col<(frame_width-1)*inc ; col+=inc ) {
			b  = image[lig*rs + col];
			a  = image[(lig-1)*rs + col     ];
			e  = image[(lig+1)*rs + col     ];
			c  = image[lig*rs     + col-inc ];
			d  = image[lig*rs     + col+inc ];
			a1 = image[(lig-1)*rs + col-inc ];
			a2 = image[(lig+1)*rs + col+inc ];
			e1 = image[(lig-1)*rs + col-inc ];
			e2 = image[(lig+1)*rs + col+inc ];
			
			all8= a1 | a | a2 | c | d | e1 | e | e2;
			li3 = e1 | e | e2;
			co3 = a2 | d | e2;

				// Les pixels contour et extremite deviennent des pixels contour
				// Les pixels fond et crete deviennet des pixels fond
			a0 = (a&1)^(a>>1);
			e0 = (e&1)^(e>>1);
			c0 = (c&1)^(c>>1);
			d0 = (d&1)^(d>>1);

			a1 = (a1&1)^(a1>>1);
			a2 = (a2&1)^(a2>>1);
			e1 = (e1&1)^(e1>>1);
			e2 = (e2&1)^(e2>>1);

			li1= a1 | a0 | a2;
			li2= e1 | e0 | e2;
			co1= a1 | c0 | e1;
			co2= a2 | d0 | e2;

			// Detection des extremités
			if( b==CONTOUR || b==LINE ) {
					// extremite si seulement un de a0, c0, d0, e0 == 1 autour du contour
				if( b==CONTOUR )
					if( ((a0^e0)^(c0^d0)) ) b=EXTREM;
						// les cas etendus
					else if( (!li2&!co2&a1&!(a^c)) | (!li2&!co1&a2&!(a^d)) |
							 (!li1&!co1&e2&!(d^e)) | (!li1&!co2&e2&!(c^e)) ) b=EXTREM;
						// seulement des lines et du fond autour
					else if( !(a0|c0|d0|e0|a1|a2|e1|e2) ) b=LINE;
					else b=CONTOUR;
						// fond si seulement 1 de a, c, d, e == CONTOUR autour d'1 crete
				else if( (a==CONTOUR && (c|d|li3)==0) || (c==1 && (a|e|co3)==0) ) {
					b=BACK_GND;
					cas3=1;
				}
					// supprime les points isoles				
				if(!all8) b=BACK_GND;
			}
			///////////////////////////////////
			//	Fermeture de contour 1er passe
			//
			// Version amelioree
			// fenetre de 4 pixels:
			//		a1	a
			//		c	b
			///////////////////////////////////
				// Cas sans delai: Wc=0
				////////////////////////
			if( Wc==0 ) {
				if( b<2 ) {			// equivalent a: if( b==BACK_GND || b==CONTOUR )
					if( a>=3 && c>=3 ) EqC[c]=EqC[a];			// cas 1N
					else if( a>=3 && c==EXTREM ) EqC[a]=EXTREM;	// cas 3N
					else if( a==EXTREM && c>=3 ) EqC[c]=EXTREM;	// cas 5N
					image[lig*rs + col]=b;						// cas 1 | 1N | 3N | 5N
				}
				else if( b==EXTREM ) {
					if( a<2 && c<2 && a1>=3 ) EqC[a1]=EXTREM;	// cas 4N
					if( a>=3 ) EqC[a]=EXTREM;					// cas 2
					if( c>=3 ) EqC[c]=EXTREM;					// cas 2
					image[lig*rs + col]=EXTREM;					// cas 2 | 4N
				}
				else {
					if( a<2 && c<2 && !cas3 ){
						if( a1==EXTREM ) image[lig*rs + col]=EXTREM;	// cas 6N
						else if( a1>=3 ) image[lig*rs + col]=EqC[a1];	// cas 2N
						else {		// cas 4
							image[lig*rs + col]=RefC;
							EqC[RefC]=RefC;
							Wc=1;
						}	
					}
					else if( a==EXTREM && c==EXTREM )			// cas 5
						image[lig*rs + col]=EXTREM;
					else if( c>=3 && a<2 )						// cas 6
						image[lig*rs + col]=c;
					else if( c<2 && a>=3 )						// cas 7
						image[lig*rs + col]=EqC[a];
					else {										// cas 8
						if( a==EXTREM ) {
							image[lig*rs + col]=EXTREM;
							if( c>=3 ) EqC[c]=EXTREM;
						}
						else if( c==EXTREM ) {
							image[lig*rs + col]=EXTREM;
							if( a>=3 ) EqC[a]=2;
						}
						else if( EqC[a]<c ) {
							image[lig*rs + col]=EqC[a];
							EqC[c]=EqC[a];
						}
						else {
							image[lig*rs + col]=c;
							EqC[a]=c;
						}
					}
				}
			} // if Wc==0
			else {	// delai courant != 0: Wc!=0 CAS AVEC DELAI
				if( b<2 ) {									// cas 11
					if( a==EXTREM ) {						// cas 5N R
						for( k=Wc ; k>=0 ; k-- ) {
							image[lig*rs + (col-k*inc)]=EXTREM;
							
						}
						Wc=0;
					}
					else if( a>=3 ) {						// cas 1N R
						for( k=Wc ; k>=0 ; k-- ) {
							image[lig*rs + (col-k*inc)]=EqC[a];
						}
						Wc=0;
					}
					else {
						image[lig*rs + col]=b;
						RefC++;
						Wc=0;
					}
				} // if b<2
				else if( b==EXTREM ) {						// cas 12
					for( k=Wc ; k>=0 ; k-- ) {
						image[lig*rs + (col-k*inc)]=EXTREM;
					}
					if( a>=3 ) EqC[a]=EXTREM;
					Wc=0;
				}
				else if( a<2 ) {							// cas 13, 14
					image[lig*rs + col]=RefC;
					if( Wc<WC_MAX ) Wc++;
					else {
						Wc=0;
						RefC++;
					}
				}
				else {										// cas 15
					for( k=Wc ; k>=0 ; k-- ) {
						image[lig*rs + (col-k*inc)]=EqC[a];
					}
					Wc=0;				
				}
			} // else Wc != 0 
		} // for

// Resolution des equivalences
	TRACE("Resolution des equivalences\n");
	for( lig = 0; lig<frame_height ; lig++ )
		for( col=0 ; col<frame_width*inc ; col+=inc ) {
			pixel = image[lig*rs + col];
			if( pixel>2 ) {
				b1=EqC[pixel];
				if( b1==pixel ) image[lig*rs + col]=BACK_GND;
				else {
					b2=EqC[b1];
					if( b2>2 ) image[lig*rs + col]=BACK_GND;
					else image[lig*rs + col]=CONTOUR;
				}
			}
			else if( image[lig*rs + col]==EXTREM ) image[lig*rs + col]=CONTOUR;
		}

}
