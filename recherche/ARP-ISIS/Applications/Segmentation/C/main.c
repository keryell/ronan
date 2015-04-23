/*
 *	Image processing for multiple platform
 * UNIX, the TDM435 DSP board with or without FPGA
 *	
 *	Bertrand BAUDET & Nicolas VITEAU
 *	ESIEE 1999
 */ 

/* bibliothèques DSP */
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

/* prototypes */
#include "tt_type.h"
#ifdef _DSP_
#include "graph.h"
#include "videodev.h"
#ifdef _XCIM_
#include "xcim.h"
#endif //  XCIM
#endif //  DSP
#ifdef _UNIX_
#include "img.h"
#endif

#include "algo.h"
#include "benchmark.h"

// Les valeurs par defaut
unsigned int LowLevel=LOW_LEVEL;
unsigned int HighLevel=HIGH_LEVEL;
float Alpha=3;

char ImageIN[50]="muscle.pgm";

#ifdef _UNIX_
char ImageOUT[50]="out.pgm";
#else
char ImageOUT[50]="out";	
#endif

unsigned char TSave	 =0;
unsigned char TCapture=1;
unsigned char TDer    =1;
unsigned char TSeuil  =1;
unsigned char TCC     =1;
unsigned char TER     =1;
unsigned char Ttiming =1;

// Les externes
extern unsigned int frame_height;
extern unsigned int frame_width;
extern unsigned int stride;

int print_menu( void )
{
	char c, car;
	
	printf("Traitements video");

#ifdef _DSP_
	printf(" sur DSP");
#ifdef _XCIM_
	printf(" et FPGA\n");
#else
	printf("\n");
#endif // XCIM
#endif // DSP
#ifdef _UNIX_
	printf(" sur UNIX\n");
#endif
	printf(" 0 - Modification des valeurs par defaut\n");
	printf(" 1 - Chaine de segmentation\n");
#ifdef _DSP_
	printf(" 2 - Video inverse\n");
	printf(" 3 - Video miroir\n");
	printf(" 4 - Video commport\n");
	printf(" 5 - Video copy EDRAM\n");
	printf(" 6 - Sauvegarder le resultat\n");
#endif
#ifdef _UNIX_
	printf(" 8 - Visualisation de l'image de sortie\n");
#endif
	printf(" 9 - Fin\n");
	
	printf("Entrer votre choix: ");
	scanf("%c", &car );
	while( c!='\n' ) scanf("%c", &c );
	return ((int)(car-'0'));
}

void change_values( void )
{
	char buf[50], car, c;
	int fin=0;
	
	while( !fin ) {	
		printf("Modification des valeurs par defaut\n");
		printf(" 1 - Valeurs de seuillage\n");
		printf("      Seuil bas  = %d\n", LowLevel );
		printf("      Seuil haut = %d\n", HighLevel );		
		printf(" 2 - Filtre de Deriche\n");
		printf("      alpha = %f\n", Alpha );
		printf(" 3 - Chaine d'acquisition:\n");
		printf("      %-26s = %s\n", "Sauvegarde des traitements", (TSave) ? "oui":"non" );
#ifdef _DSP_
		printf("      %-26s = %s\n", "Image en entree", (TCapture) ? "capture":"fichier" );
#endif
		printf("      %-26s = %s\n", "Deriche", (TDer) ? "oui":"non" );
		printf("      %-26s = %s\n", "Seuillage", (TSeuil) ? "oui":"non" );		
		printf("      %-26s = %s\n", "Fermeture de contour", (TCC) ? "oui":"non" );		
		printf("      %-26s = %s\n", "Etiquetage des regions", (TER) ? "oui":"non" );		
		printf(" 4 - Fichier d'entree\n");
		printf("      Image a traiter = %s\n", ImageIN );
		printf(" 5 - Fichier de sortie\n");
		printf("      Image traitee = %s\n", ImageOUT );
		printf(" 6 - Affichage des timings\n");
		printf("      %s\n", (Ttiming) ? "oui":"non" );			
		printf(" 9 - Fin\n");
		printf("Entrer votre choix: ");
		scanf("%c", &car );
		while( c!='\n' ) scanf("%c", &c );		
		
		switch( car-'0' ) {
			case 1:
				printf("Valeurs de seuillage\n");
				printf("   Seuil bas[%u]=", LowLevel );
				scanf("%u", &LowLevel );				
				while( c!='\n' ) scanf("%c", &c );		
				printf("   Seuil haut[%u]=", HighLevel );		
				scanf("%u", &HighLevel );			
				while( c!='\n' ) scanf("%c", &c );						
				break;
				
			case 2:
				printf("Filtre de Deriche\n");
				printf("   alpha[%f]=", Alpha );
				scanf("%f", &Alpha );
				while( c!='\n' ) scanf("%c", &c );		
				break;
				
			case 3:
				printf("Traitements (o\\n)\n");
	
				printf("   Sauvegarde[%s]=", (TSave) ? "oui":"non" );
				scanf("%c", &c);
				if( c=='n' || c=='N' ) TSave=0;
				else TSave=1;
				while( c!='\n' ) scanf("%c", &c );										

				printf("   Image en entree[%s]=", (TCapture) ? "capture":"fichier" );
				scanf("%c", &c);
				if( c=='f' || c=='F' ) TCapture=0;
				else TCapture=1;
				while( c!='\n' ) scanf("%c", &c );										
				
				printf("   Deriche[%s]=", (TDer) ? "oui":"non" );
				scanf("%c", &c);
				if( c=='n' || c=='N' ) TDer=0;
				else TDer=1;
				while( c!='\n' ) scanf("%c", &c );										
				
				printf("   Seuillage[%s]=", (TSeuil) ? "oui":"non" );		
				scanf("%c", &c);
				if( c=='n' || c=='N' ) TSeuil=0;
				else TSeuil=1;
				while( c!='\n' ) scanf("%c", &c );										
				
				printf("   Fermeture de contour[%s]=", (TCC) ? "oui":"non" );		
				scanf("%c", &c);
				if( c=='n' || c=='N' ) TCC=0;
				else TCC=1;
				while( c!='\n' ) scanf("%c", &c );										
				
				printf("   Etiquetage des regions[%s]=", (TER) ? "oui":"non" );									
				scanf("%c", &c);
				if( c=='n' || c=='N' ) TER=0;
				else TER=1;
				while( c!='\n' ) scanf("%c", &c );										
				break;
				
			case 4:
				printf("Fichier d'entree\n");
				printf("   Image a traiter[%s]=", ImageIN );
				scanf("%s", ImageIN );			
				while( c!='\n' ) scanf("%c", &c );						
				break;

			case 5:
				printf("Fichier de sortie\n");
				printf("   Image de sortie[%s]=", ImageOUT );
				scanf("%s", ImageOUT );			
				while( c!='\n' ) scanf("%c", &c );						
				break;
				
			case 6:
				printf("Affichage des timings[%s]=", (Ttiming) ? "oui":"non" );
				scanf("%c", &c);
				if( c=='n' || c=='N' ) Ttiming=0;
				else Ttiming=1;
				while( c!='\n' ) scanf("%c", &c );										
				break;
				
			case 9:
				fin=1;
				break;
				
			default:
				printf("Mauvais choix\n");
		}
	}
}

int 
//main (int argc, char *argv[])
main ( )
{
	int fin;
	char command[50];
	TT_Image *ptrTImage;
#ifdef _DSP_
	char str[50];
	TT_Image TImage;
	unsigned int *display_addr = (unsigned int *)0xB0000000 + 1024*512 ;
#endif
	
	printf("Image processing by Bertrand BAUDET & Nicolas VITEAU - ESIEE 1999\n");

#ifdef _DSP_
#ifdef _XCIM_
	if( FPGA_init()<0 ) {
		printf("L'initialisation des FPGA a echoué\n");
		exit(1);
	}
#endif // _XCIM_
	init_graphic();
	draw_banner();
	start_stat();
#endif // _DSP_

	fin = 0;
	while( !fin ) {
		switch( print_menu( ) ) {
			case 0 :
				change_values( );
				break;
				
			case 1 :
#ifdef _UNIX_
				ptrTImage = read_image_file( ImageIN );
				if( TDer==1 ) {
					bench_start();
				   Deriche( ptrTImage, Alpha );
					bench_print("Deriche");
				}
				if( TSeuil==1 ) {
					bench_start();
					Double_Threshold( ptrTImage );
					bench_print("Double seuillage");
				}
				create_border( ptrTImage );
 				if( TCC==1 ) {
					bench_start();
					contour_closing( ptrTImage );
					bench_print("Fermeture de contour");
				}
				if( TER==1 ) {			
					bench_start();
					region_labelling( ptrTImage );
					bench_print("Etiquetage des regions");
				}
				bench_start();
//		 		Normalisation( ptrTImage );
				BidouilleNico( ptrTImage );
				bench_print("Normalisation");
				write_image_file( ImageOUT, ptrTImage );
#endif
#ifdef _DSP_
				do_segmentation(  );
#endif
				break;
#ifdef _DSP_
			case 2 :
				do_video_reverse();
				break;
				
			case 3 :
				do_video_mirror();
				break;

			case 4 :
				do_video_commport();
				break;

			case 5 :
				do_video_copy_EDRAM();
				break;

			case 6:
				printf("Entrer le nom de fichier: ");
				scanf("%s", str );

//				capture_single_frame( &TImage );
				printf("Image capturee\n");
//				half_left_screenshot_2_file( str );		
//				write_frame_2_file( &TImage, str );
				write_result_2_file( str );
//				write_screen_2_file( str );

				printf("Image sauvegardee\n");
				break;
#endif

#ifdef _UNIX_
			case 8:
				sprintf( command, "xv %s &", ImageOUT );
				system( command );
				break;
#endif
			case 9:
				fin=1;
				break;
				
			default:
				printf("Mauvais choix\n");
		}
	
	}

#ifdef _DSP_	
	stop_stat();
#ifdef _XCIM_
	if( FPGA_stop( )<0 ) {
		printf("Impossible d'effacer les configurations des FPGA\n");
		exit(1);
	}
#endif // _XCIM_
#endif // _DSP_
		
	exit(0);
}

