#ifndef _ALGO_H_
#define _ALGO_H_
#include "tt_type.h"

	// Definition pour le debuguage
#ifdef DEBUG
#define TRACE(string) printf(string)
#else	
#define TRACE(string) {}
#endif

	// Valeur du retard max pour l'etiquetage des cretes
#define WC_MAX		3
#define WR_MAX		3

	// Numerotation  pour la fermeture de contour
	// Les valeurs de numerotation sont IMPORTANTES, 
	// certain calculs se font sur les bits de ces valeurs.
	// Il est donc fortement conseillé de NE PAS les changer.
#define BACK_GND	0
#define CONTOUR		1
#define EXTREM		2
#define LINE		3

	// Valeurs de seuils pour le double seuillage
#define LOW_LEVEL	8
#define HIGH_LEVEL	16

	// Valeur pour les pixels lors du seuillage
#define LOW_VAL		BACK_GND
#define MIDDLE_VAL	LINE	
#define HIGH_VAL	CONTOUR	

#define Carre(a) ((a)*(a))

void Deriche( TT_Image *Timage, float gamma );
void Double_Threshold( TT_Image *Timage );
void create_border( TT_Image *TImage );
void find_extrems_neighbour_4( TT_Image *TImage );
void find_extrems_neighbour_8( TT_Image *TImage );
void contour_closing( TT_Image *Timage );
void connect( TT_Image *Timage );
void region_labelling( TT_Image *Timage );
void out_labels( TT_Image *Timage );
void Normalisation( TT_Image *Timage );
void BidouilleNico( TT_Image *Timage );
#endif
