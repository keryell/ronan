/*
 * Traitements video
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <timer40.h>
#include <compt40.h>
#include <intpt40.h>
#include <string.h>

#include "trig.h"
#include "tdm435.h"
#include "graph.h"
#include "stat.h"
#include "algo.h"
#include "comm.h"
#include "tt_type.h"

#include "benchmark.c" // c'est crade, mais DOS oblige !!!

unsigned int *frame_addr = (unsigned int *) 0xB0000000;
unsigned int *EDRAM_addr = (unsigned int *) 0x003B0000;
unsigned int frame_height = 512;
unsigned int frame_width  = 512;
unsigned int stride = 1024;

extern unsigned int clock_1s;

extern unsigned char TSave;
extern unsigned char TCapture;
extern unsigned char TDer;
extern unsigned char TSeuil;
extern unsigned char TCC;
extern unsigned char TER;
extern float Alpha;
extern char ImageOUT[];
extern char ImageIN[];

void do_video_copy_EDRAM()
{
	register unsigned int x, y, offset, pos_y, pos_x, pos_EDRAM;
	int count;

	draw_presentation("Video Copy EDRAM");
	set_stat( FPS );	// Frames per second

	offset = stride * frame_height ;

	for(count=0; count< 750 ; count++)
	{
		sync_stat();

                	/* recopie de l'image en VRAM vers l'EDRAM */
		for( y=0, pos_EDRAM=0; y < frame_height ; y++ )
		{
			pos_y = y<<10;
			for( x=0 ; x<frame_width ; x+=4, pos_EDRAM++ )
			{
				pos_x = pos_y + x;
				EDRAM_addr[ pos_EDRAM ] = frame_addr[ pos_x ];
                        }
		}
                	/* recopie de l'image en EDRAM vers la VRAM */
		for( y=0, pos_EDRAM=0; y < frame_height ; y++)
		{
			pos_y = y<<10;
			for( x=0 ; x<frame_width ; x+=4, pos_EDRAM++ )
			{
				pos_x = pos_y + x;
				frame_addr[ pos_x + offset ] = EDRAM_addr[ pos_EDRAM ];
                        }
		}
	}

	clr_target( offset );

}


void do_video_dma_comport()
{
	register unsigned int x, y, pos_y, offset;
	int count;

	draw_presentation("Recopie en mode DMA via 1 comport"); 
	set_stat( FPS );	// Frames per second	

	offset = stride * frame_height;

	for(count=0; count< 750 ; count++)
	{
		sync_stat();
		for( y=0; y < frame_height ; y++ )
		{
			pos_y = y<<10;
			send_msg( COMPORT_OUT, frame_addr+pos_y, frame_width/4, 4 );
			receive_msg( COMPORT_IN, frame_addr+pos_y+offset, 4 );		
/*			while(chk_dma(COMPORT_OUT));
			while(chk_dma(COMPORT_IN));
*/		}

	}

	clr_target( offset );
}

void do_video_commport()
{
	register unsigned int x, y, offset, pos_y, pos_x ;
	register unsigned int TTword;
	int count;
	COMPORT_REG *cp_ptr_in= COMPORT_ADDR( COMPORT_IN );
	COMPORT_REG *cp_ptr_out= COMPORT_ADDR( COMPORT_OUT );

	draw_presentation("Video Comm Port");
	set_stat( FPS );	// Frames per second
	
	offset = stride * frame_height ;

	for(count=0; count< 750 ; count++) {
		sync_stat();
		for( y=0; y < frame_height ; y++)
		{
			pos_y = y<<10;
			for( x=0 ; x<frame_width ; x+=4 ) {
				pos_x = pos_y + x;

				TTword = frame_addr[ pos_y+x ];
//printf("0x%08x-->",TTword);				
				cp_ptr_out->out_port = TTword;
//				cp_ptr_out->out_port = frame_addr[ pos_y+x ];
//				frame_addr[ pos_x + offset ] = cp_ptr_in->in_port;
				TTword = cp_ptr_in->in_port;				
				frame_addr[ pos_x + offset ] = TTword;
//printf("0x%08x\n",TTword);
            }
		}
	}

	clr_target( offset );
}



void do_video_reverse()
{
	register unsigned int x, y, offset, pos_y, pos_x;
	int count;

	draw_presentation("Video Inverse");
	set_stat( FPS );	// Frames per second
	
	offset = stride * frame_height ;

	for(count=0; count< 300 ; count++)
	{
		sync_stat();
		for( y=0; y < frame_height ; y++)
		{
			pos_y = y<<10;
			for( x=0 ; x<frame_width ; x+=4 )
			{
				pos_x = pos_y + x;
				frame_addr[ pos_x + offset ] = 0xFFFFFFFF - frame_addr[ pos_x ];
			}
		}
	}

//	clr_target( offset );
}


void do_video_mirror()
{
	register unsigned int x, y, offset, pos_y, pos_x, word ;
	int count;

	draw_presentation("Video Mirror");
	set_stat( FPS );	// Frames per second
	
	offset = stride * frame_height ;

	for(count=0; count< 750 ; count++)
	{
		sync_stat();
		for( y=0; y < frame_height ; y++)
		{
			pos_y = y<<10;
			for( x=0 ; x<=frame_width ; x+=4 )
			{
				pos_x = pos_y + x;

				word = frame_addr[ pos_y+frame_width-x ];
				word = ((word&0x000000FF)<<24) |
				       ((word&0x0000FF00)<<8) |
				       ((word&0x00FF0000)>>8) |
					(word&0xFF000000)>>24;
				frame_addr[ pos_x + offset-4 ] = word;
                        }
		}
	}

	clr_target( offset );
}

void do_segmentation( void )
{
	char filename[50];
	int count;
	unsigned int *display_addr = (unsigned int *)0xB0000000 + stride * frame_height ;
	TT_Image Timage;

/*
		// Image en memoire VRAM B
	Timage.ptrImage = (TT_type*) 0xB0100000; // VRAM B Word
	Timage.Height 	= frame_height;
	Timage.Width 	= frame_width;
	Timage.Stride	= stride*2;
	Timage.Increment= 4;
*/
		// Image en memoire EDRAM
	Timage.ptrImage = (TT_type*) 0x003B0000; // EDRAM Word
	Timage.Height 	= frame_height;
	Timage.Width 	= frame_width;
	Timage.Stride	= frame_width;
	Timage.Increment= 1;

	draw_presentation("Images segmentation");
	set_stat( SPF ); // Seconds per frame

	for(count=0; count< 1 ; count++)
	{
		sync_stat();
		if( TCapture ) capture_single_frame( &Timage );		
		else read_frame_from_file(ImageIN, &Timage);
		if( TSave==1 ) {
			strcpy( filename, ImageOUT );
			strcat( filename, "ORG.raw" );
			write_frame_2_file( &Timage, filename );
		}
		if( TDer==1 ) {
			bench_start();
			Deriche( &Timage, Alpha );
			bench_print("Deriche");
			if( TSave==1 ) {
				strcpy( filename, ImageOUT );
				strcat( filename, "DER.raw" );
				write_frame_2_file( &Timage, filename );
			}
		}
		if( TSeuil==1 ) {
			bench_start();
			Double_Threshold( &Timage );
			bench_print("Double seuillage");
			if( TSave==1 ) {
				strcpy( filename, ImageOUT );
				strcat( filename, "LEV.raw" );
				write_frame_2_file( &Timage, filename );
			}
		}
		create_border( &Timage );
		if( TCC==1 ) {
			bench_start();
			contour_closing( &Timage );
			bench_print("Fermeture de contour");
			if( TSave==1 ) {
				strcpy( filename, ImageOUT );
				strcat( filename, "FCO.raw" );
				write_frame_2_file( &Timage, filename );
			}
		}
		if( TER==1 ) {
			bench_start();
			region_labelling( &Timage );
			bench_print("Etiquetage des regions");
			if( TSave==1 ) {
				strcpy( filename, ImageOUT );
				strcat( filename, "ETR.raw" );
				write_frame_2_file( &Timage, filename );
			}
		}
		bench_start();
//		Normalisation( &Timage );
		BidouilleNico( &Timage );
		bench_print("Normalisation");
		if( TSave==1 ) {
			strcpy( filename, ImageOUT );
			strcat( filename, "NOR.raw" );
			write_frame_2_file( &Timage, filename );
		}
		draw_frame( &Timage, display_addr );
	}
	sync_stat();
}
