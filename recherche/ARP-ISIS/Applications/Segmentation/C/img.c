#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include "tt_type.h"

/*
// Sauvegarde et charge des images au format PGM
/////////////////////////////////////////////////
*/

TT_Image *read_image_file( const char *FileName )
{
	int handle;
	int nbel;		// number of end line
	int i, x, y, inc, rs;
	unsigned int width, height;
	unsigned char *ImageBuffer;
	unsigned char head[100];
	TT_Image *T_Image;
	TT_type *image;

#ifdef DEBUG
	printf("Lecture de l'image %s\n", FileName );
#endif

	if ( ((handle=open(FileName, O_RDONLY ,
						S_IREAD | S_IWRITE)))==-1) {
		printf("Erreur à l'ouverture du fichier.\n");
		exit(1);
	}
		// lecture de l'entete
	nbel=0;
	for( i=0 ; nbel<3; i++ ) {
		if( read(handle, (void*)(head+i), sizeof(unsigned char))!=sizeof(unsigned char) ) {
			printf("Erreur de lecture dans le fichier.\n");
			exit(1);
		}
		if( head[i]=='\n' ) nbel++;
	}
		// recupere la taille de l'image
	if( sscanf((const char*)head, "P5\n%d %d\n255\n", &width, &height)!=2 ){
		printf("Entete inconnue!\n");
		exit(1);
	}

	ImageBuffer = (unsigned char*)malloc( sizeof(unsigned char)*width*height );
	if( ImageBuffer==NULL ) {
		printf("Impossible d'allouer de la memoire\n");
		exit(1);
	}

		// lecture de l'image
	if( read(handle, (void*)ImageBuffer, width*height)!=(width*height) ) {
		printf("Erreur de lecture dans le fichier.\n");
		exit(1);
	}
	close(handle);	

	T_Image = (TT_Image*)malloc( sizeof(TT_Image) );
	if( T_Image==NULL ) {
		printf("Impossible d'allouer de la memoire\n");
		exit(1);
	}

	T_Image->Height 		= height;
	T_Image->Width  		= width;
		// Depend de la memoire utilisee (cf DSP et la VRAM d'acquisition)
	T_Image->Stride 		= width;
	T_Image->Increment 	= 1;
	inc = T_Image->Increment;
	rs  = T_Image->Stride;

	image = (TT_type*)malloc( sizeof(TT_type)*width*height );
	if( image==NULL ) {
		printf("Impossible d'allouer de la memoire\n");
		exit(1);
	}

	for( y=0, i=0 ; y<height ; y++)
		for( x=0 ; x<width*inc ; x+=inc, i++)
			image[ y*rs + x ]=(TT_type)ImageBuffer[i];

	T_Image->ptrImage = image;
	free( ImageBuffer );

	return T_Image;
}

void write_image_file( const char *FileName, TT_Image *TImage )
{
	char head[100];
	int len, i, x, y;
	int handle;
	unsigned char *ImageBuffer;

		/* image */
	TT_type *image			= TImage->ptrImage;
	unsigned int height	= TImage->Height;
	unsigned int width	= TImage->Width;	
	unsigned int rs		= TImage->Stride;
	unsigned int inc		= TImage->Increment;

#ifdef DEBUG
	printf("Ecriture de l'image %s\n", FileName );
#endif

	if ((handle = open(FileName, O_WRONLY | O_CREAT | O_TRUNC,
						S_IREAD | S_IWRITE)) == -1) {
		printf("Erreur à l'ouverture du fichier.\n");
		exit(1);
	}
		// ecriture de l'entete pour le format PGM
	sprintf(head,"P5\n%d %d\n255\n", height, width );
	len = strlen( head );
	if( write(handle, (void*)(head), len )!= len ) {
		printf("Erreur d'ecriture dans le fichier.\n");
		exit(1);
	}

		// Attention, les donnes de l'image en memoire sont du type
		// TT_type. La fonction write ecrit des octets, il faut donc passer
		// par un tampon intermediaire.
	ImageBuffer = (unsigned char*)malloc( sizeof(unsigned char)*height*width );
	if( ImageBuffer==NULL ){
		printf("Impossible d'allouer de la memoire\n");
		exit(1);
	}
	for( y=0, i=0 ; y<height ; y++)
		for( x=0 ; x<width*inc ; x+=inc, i++)
			ImageBuffer[i]=(unsigned char)image[ y*rs + x ];

		// ecriture de l'image
	if( write(handle, (void*)ImageBuffer, width*height)!=(width*height) ) {
		printf("Erreur d'ecriture dans le fichier.\n");
		exit(1);
	}
	close(handle);
	free( ImageBuffer );

}

void print_image( TT_Image *Timage )
{
	unsigned int lig, col;

		/* image */
	TT_type *image 	= 			Timage->ptrImage;
	unsigned int frame_height = Timage->Height;
	unsigned int frame_width = 	Timage->Width;
	unsigned int rs = 			Timage->Stride;
	unsigned int inc = 			Timage->Increment;

	for( lig = 0; lig< frame_height ; lig++ ) {
		for( col=0 ; col<frame_width*inc ; col+=inc ) {
			printf("%2u ", image[lig*rs + col ] );
			if(col==30) break;
		}
		//printf("\n###########\n");
		printf("\n");
		if(lig==20) exit(1);
	}
}
