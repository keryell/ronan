#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <timer.h>
#include <intpt40.h>
#include <fcntl.h>

#include "trig.h"
#include "tdm435.h"
#include "graph.h"
#include "tt_type.h"
#include "algo.h"

t_window	*frame;
t_window	*overlay;
t_gc		*blue, *red, *black, *green;
t_font		*font, *big_font;
t_gc		*font_gc, *big_font_gc;
t_display	*display;

int			scr_width=1024, scr_height=1024;

extern unsigned int *frame_addr;

void print_centered(t_window *win, t_gc *gc, char *msg, int y)
{
	int *addr, width, height, stride;
	int box[4];
	int rv;
	int x;
	t_point rect[2];

	rv = t_get_window_addr(win, &addr, &width, &height, &stride); t_check_rv(rv);
	rv = t_get_gc_font_txt_box(gc, msg, 0, 0, box); t_check_rv(rv);

	x = (width - (box[0] + box[1]))/2;

	rect[0].x = x - (box[0]+10);
	rect[0].y = y - (box[2]+10);
	rect[1].x = x + (box[1]+10);
	rect[1].y = y + (box[3]+10);
	t_fill_rect(win, blue, rect);

	rv = t_set_window_txt_cursor(win, x, y); t_check_rv(rv);
	rv = t_draw_str(win, gc, msg); t_check_rv(rv);
}

void print_right(t_window *win, t_gc *gc, char *msg, int y)
{
	int *addr, width, height, stride;
	int box[4];
	int rv;
	int x;
	t_point rect[2];

	rv = t_get_window_addr(win, &addr, &width, &height, &stride); t_check_rv(rv);
	rv = t_get_gc_font_txt_box(gc, msg, 0, 0, box); t_check_rv(rv);

	x = (width - (box[0] + box[1]))/2 + width/4;

	rect[0].x = x - (box[0]+10);
	rect[0].y = y - (box[2]+10);
	rect[1].x = x + (box[1]+10);
	rect[1].y = y + (box[3]+10);
	t_fill_rect(win, blue, rect);

	rv = t_set_window_txt_cursor(win, x, y); t_check_rv(rv);
	rv = t_draw_str(win, gc, msg); t_check_rv(rv);
}


void draw_banner( void )
{
	static char small_banner[] = "Bertrand & Nicolas";

	print_right(overlay, font_gc, small_banner, 240 );

}

void draw_title(char *msg)
{
	print_right(overlay, font_gc, msg, 550);
}

void draw_presentation( char *msg )
{
	t_clr_display_overlay(display, 0, 0);
	draw_banner();
	draw_title(msg);
}

void update(void)
{
	int rv; 

	t_set_display(display, 0, 0, scr_width, scr_height, 8, 0);

	rv = t_init_display(display); t_check_rv(rv);
	rv = t_init_overlay(display, overlay, 0); t_check_rv(rv);
	rv = t_init_frame(display, frame, 0); t_check_rv(rv);
	t_clr_display_frame(display, 0, 0xb0);
	t_clr_display_frame(display, 1, 0x40);

}

void load(t_display * disp)
{
	char filename[]="good.dat";
	FILE *fp;
	int i;
	int rv;
	fp = fopen(filename, "rt");
	if (fp==NULL)
	{
		printf("Failed to open \"%s\": ", filename);
		perror(NULL);
		return;
	}

	fscanf(fp, "clock\n");
	rv = fscanf(fp, "0x%x 0x%x\n", &disp->regs->disp_clock, &disp->regs->capt_clock);
	if (rv != 2) goto err;
	
	fscanf(fp, "LM1882\n");
	for (i=0; i<(sizeof(disp->regs->LM1882)/sizeof(int)); i++)
	{
		rv = fscanf(fp, "0x%x\n", &disp->regs->LM1882[i]);
		if (rv != 1) goto err;
	}


	fscanf(fp, "VCONT_DISPLAY\n");
	for (i=0; i<(sizeof(disp->regs->VCONT_DISPLAY)/sizeof(int)); i++)
	{
		rv = fscanf(fp, "0x%x\n", &disp->regs->VCONT_DISPLAY[i]);
		if (rv != 1) goto err;
	}

	fscanf(fp, "VCONT_CAPTURE\n");
	for (i=0; i<(sizeof(disp->regs->VCONT_CAPTURE)/sizeof(int)); i++)
	{
		rv = fscanf(fp, "0x%x\n", &disp->regs->VCONT_CAPTURE[i]);
		if (rv != 1) goto err;
	}

	
	fscanf(fp, "BT252\n");
	for (i=0; i<(sizeof(disp->regs->BT252)/sizeof(int)); i++)
	{
		rv = fscanf(fp, "0x%x\n", &disp->regs->BT252[i]);
		if (rv != 1) goto err;
	}

	fscanf(fp, "BT261\n");
	for (i=0; i<(sizeof(disp->regs->BT261)/sizeof(int)); i++)
	{
		rv = fscanf(fp, "0x%x\n", &disp->regs->BT261[i]);
		if (rv != 1) goto err;
	}

	fscanf(fp, "BT477\n");
	rv = fscanf(fp, "0x%x\n", &disp->regs->BT477_commd);
	if (rv != 1) goto err;

        rv = fscanf(fp, "size %d %d\n", &scr_width, &scr_height);
        if (rv != 2) goto err;

	fclose(fp);

	update();
	return;
err:

	printf("Error: bad file format\n");
	fclose(fp);
}

#define FRAME 0

void init_graphic()
{
	t_clut		*grey_lut;
	t_clut		*colour_lut;
	int 		rv;
	unsigned int pos;

	display = t_new_display();

	/* load the file obtened with tweek with the good value for the monitor */
	load(display);

	rv = t_init_display(display); t_check_rv(rv);

	overlay = t_new_window();
	frame = t_new_window();
	rv = t_init_overlay(display, overlay, 0); t_check_rv(rv);

	rv = t_set_display_frame(display, FRAME); t_check_rv(rv);
	rv = t_set_capture_frame(display, FRAME); t_check_rv(rv);
	rv = t_init_frame(display, frame, FRAME); t_check_rv(rv);

	colour_lut = t_new_clut();
	rv = t_init_clut_color(colour_lut, 0); t_check_rv(rv);
	rv = t_set_clut_rgb(colour_lut, RED, 255, 0, 0); t_check_rv(rv);
	rv = t_set_clut_rgb(colour_lut, YELLOW, 255, 178, 0); t_check_rv(rv);
	rv = t_set_clut_rgb(colour_lut, GREEN, 100, 255, 100); t_check_rv(rv);
	rv = t_set_clut_rgb(colour_lut, BLUE, 0, 43, 153); t_check_rv(rv);

	grey_lut = t_new_clut();
	rv = t_init_grey_clut(grey_lut); t_check_rv(rv);

	rv = t_install_clut(display, grey_lut); t_check_rv(rv);
	rv = t_install_capture_clut(display, grey_lut); t_check_rv(rv);
	rv = t_install_ovl_clut(display, colour_lut); t_check_rv(rv);

	blue = t_new_gc();
	rv = t_init_gc(blue); t_check_rv(rv);
	rv = t_set_gc_draw_color(blue, BLUE, 0x00); t_check_rv(rv);

	red = t_new_gc();
	rv = t_init_gc(red); t_check_rv(rv);
	rv = t_set_gc_draw_color(red, RED, 0x00); t_check_rv(rv);

	black = t_new_gc();
	rv = t_init_gc(black); t_check_rv(rv);
	rv = t_set_gc_draw_color(black, 0x00, 0x00); t_check_rv(rv);

	font_gc = t_new_gc();
	rv = t_init_gc(font_gc); t_check_rv(rv);
	rv = t_set_gc_draw_color(font_gc, GREEN, 0x00); t_check_rv(rv);

	big_font_gc = t_new_gc();
	rv = t_init_gc(big_font_gc); t_check_rv(rv);
	rv = t_set_gc_draw_color(big_font_gc, GREEN, 0x00); t_check_rv(rv);


	/* load fonts */

	font = t_new_font();
	rv = t_load_font(font, "helvr24"); t_check_rv(rv);
	rv = t_set_gc_font(font_gc, font); t_check_rv(rv);
	rv = t_set_gc_txt_color(font_gc, GREEN, 0); t_check_rv(rv);
	rv = t_set_gc_txt_mode(font_gc, TFONT_FG); t_check_rv(rv);

	big_font = t_new_font();
	rv = t_load_font(big_font, "helvr24"); t_check_rv(rv);
	rv = t_set_gc_font(big_font_gc, big_font); t_check_rv(rv);
	rv = t_set_gc_txt_color(big_font_gc, YELLOW, 0); t_check_rv(rv);
	rv = t_set_gc_txt_mode(big_font_gc, TFONT_FG); t_check_rv(rv);
	rv = t_set_gc_font_scale(big_font_gc, 2, 2);

		/* remplit l'ecran en blanc */
	for( pos=0; pos < 1024*1024 ; pos+=4 )
	{
		frame_addr[ pos  ] = 0xFFFFFFFF;
	}

	draw_logo_esiee();

}

/**************************************************************************/
/*
 *	Utility for capture 
 */
#define COMMAND_REG	4

extern unsigned int *EDRAM_addr;
extern unsigned int frame_height;
extern unsigned int frame_width;
extern unsigned int stride;

inline
void clr_target( unsigned int offset )
{
	unsigned int y, x, pos_y;
        /* Efface l'image */
	for( y=0; y < frame_height ; y++ )
	{
		pos_y = y<<10;
		for( x=0 ; x<frame_width ; x+=4 )
			frame_addr[ pos_y + x + offset ] = 0xFFFFFFFF;
	}

}

void start_capture( void )
{
	display->regs->VCONT_CAPTURE[COMMAND_REG] |= 0x80;
	while (d_load_VCAPTURE_reg(display->regs->VCONT_CAPTURE) == TERR_Failed);
}


void stop_capture( void )
{
	display->regs->VCONT_CAPTURE[COMMAND_REG] &= 0x7f;
	while (d_load_VCAPTURE_reg(display->regs->VCONT_CAPTURE) == TERR_Failed);
}

void display_VRAM_B( void )
{
	display->regs->VCONT_DISPLAY[5] &= 0xbf;
	while (d_load_VDISPLAY_reg(display->regs->VCONT_DISPLAY) == TERR_Failed);
}

inline
void capture_single_frame( TT_Image *TImage )
{
	register unsigned int x, y, pos_y, pos_capt, pos_frame, word;

		/* image */
	TT_type *image		= TImage->ptrImage;
	unsigned int inc	= TImage->Increment;	
	TImage->Height		= frame_height;
	TImage->Width		= frame_width;	
	
	for( pos_capt=0, y=0; y<frame_height ; y++ ) {
		pos_y = y*stride;
		for( x=0 ; x<frame_width ; x+=4, pos_capt+=4*inc ) {
			pos_frame = pos_y + x;
			word = frame_addr[ pos_frame ];
				/* on joue avec les grands et petits indiens !!! */
			image[ pos_capt+3*inc ] = (TT_type)(word&0xff000000)>>24;
			image[ pos_capt+2*inc ] = (TT_type)(word&0x00ff0000)>>16;
			image[ pos_capt+1*inc ] = (TT_type)(word&0x0000ff00)>>8;
			image[ pos_capt   	  ] = (TT_type) word&0x000000ff;
		}
	}
}

inline
void draw_frame( TT_Image *TImage, unsigned int *display_addr )
{
	register unsigned int x, y, pos_y, pos_capt, pos_frame, word;

		/* image */
	TT_type *image		= TImage->ptrImage;
	unsigned int height	= TImage->Height;
	unsigned int width	= TImage->Width;	
	unsigned int inc	= TImage->Increment;
	
	for( pos_capt=0, y=0; y<height ; y++ ) {
		pos_y = y*stride;
		for( x=0 ; x<width ; x+=4, pos_capt+=4*inc ) {
			pos_frame = pos_y + x;
				/* ils font chier les grands et petits indiens !!! */
			word = (unsigned int) (
					 image[ pos_capt+3*inc ]<<24 
					|image[ pos_capt+2*inc ]<<16 			
					|image[ pos_capt+1*inc ]<<8
					|image[ pos_capt ] );
			display_addr[ pos_frame ] = word;			
		}
	}
}

int half_left_screenshot_2_file( const char *FileName )
{
	unsigned char *src_frame  = (unsigned char*)0xB0200000;
	unsigned char *capt_frame = (unsigned char*)0xB0300000;
	register unsigned int x, y, pos_y;
	int handle;

	for( y=0; y<frame_height*2 ; y++ ) {
		pos_y = y*stride;
		for( x=0 ; x<frame_width ; x++ )
			capt_frame[ pos_y+x ] = src_frame[ pos_y+x ];
	}

	printf("Image capturee\n");
display_VRAM_B();

	if ((handle = open(FileName, O_WRONLY | O_CREAT | O_TRUNC,
						S_IREAD | S_IWRITE)) == -1) {
		printf("Erreur à l'ouverture du fichier.\n");
		return -1;
	}

	for( y=0; y<frame_height*2 ; y++ ) {
		pos_y = y*stride;
		if( write(handle, (void*)(capt_frame+(pos_y)), frame_width)!=frame_width ) {
			printf("Erreur d'ecriture dans le fichier.\n");
			return -1;
		}
	}
	close(handle);
	return 0;
}

int write_frame_2_file( TT_Image *TImage, const char *FileName )
{
	int handle;
	register unsigned int x, y;

		/* image */
	TT_type *image		= TImage->ptrImage;
	unsigned int height	= TImage->Height;
	unsigned int width	= TImage->Width;	
	unsigned int rs		= TImage->Stride;
	unsigned int inc	= TImage->Increment;

	TRACE("Sauvegarde de l'image traitee\n");

	if ((handle = open(FileName, O_WRONLY | O_CREAT | O_TRUNC,
						S_IREAD | S_IWRITE)) == -1) {
		printf("Erreur à l'ouverture du fichier.\n");
		return -1;
	}

	if( inc==1 && rs==width ) {
		if( write(handle, (void*)((uc)image), height*width )!=height*width ) {
			printf("Erreur d'ecriture dans le fichier.\n");
			return -1;
		}
	}
	else {
		for( y=0; y<height ; y++ ) {
			for( x=0; x<width*inc; x+=inc ){
				if( write(handle, (void*)((uc)(image+(y*rs+x))), sizeof(uc))!=sizeof(uc) ) {
					printf("Erreur d'ecriture dans le fichier.\n");
					return -1;
				}
			}
		}
	}
	close(handle);
	return 0;

}

int write_result_2_file( const char *FileName )
{
	unsigned int *result_frameWords = (unsigned int*)0xB0000000+1024*512;
	unsigned char *capt_frameBytes = (unsigned char*)0xB0300000;
	unsigned int *capt_frameWords = (unsigned int*)0xB0100000;	
	register unsigned int x, y, pos_y;
	int handle;
	
	unsigned int FrameHeight = 512;
	unsigned int FrameWidth  = 512;
	unsigned int Stride = 1024;
	unsigned int inc = 4;

	for( y=0; y<FrameHeight ; y++ ) {
		pos_y = y*Stride;
		for( x=0; x<FrameWidth; x+=inc ){
			capt_frameWords[ pos_y + x ] = result_frameWords[ pos_y + x ];
		}
	}

	if ((handle = open(FileName, O_WRONLY | O_CREAT | O_TRUNC,
						S_IREAD | S_IWRITE)) == -1) {
		printf("Erreur à l'ouverture du fichier.\n");
		return -1;
	}

	for( y=0; y<FrameHeight ; y++ ) {
		pos_y = y*Stride;
		if( write(handle, (void*)((uc)(capt_frameBytes+(pos_y))), FrameWidth)!=FrameWidth ) {
			printf("Erreur d'ecriture dans le fichier.\n");
			return -1;
		}
	}
	close(handle);
	return 0;
}

int write_screen_2_file( const char *FileName )
{
	unsigned char *capt_frame = (unsigned char*)0xB0200000;
	register unsigned int x, y, pos_y;
	int handle;
	
	unsigned int FrameHeight = 1024;
	unsigned int FrameWidth  = 1024;
	unsigned int Stride = 1024;
	
	if ((handle = open(FileName, O_WRONLY | O_CREAT | O_TRUNC,
						S_IREAD | S_IWRITE)) == -1) {
		printf("Erreur à l'ouverture du fichier.\n");
		return -1;
	}

	if( write(handle, (void*)((uc)capt_frame), FrameWidth*FrameHeight)!=(FrameWidth*FrameHeight) ) {
		printf("Erreur d'ecriture dans le fichier.\n");
		return -1;
	}
	close(handle);
	return 0;
}


	// Il faut passer un pointeur valide pour T_Image. La zone doit etre
	// reserve pour recevoir l'image.
	// NE MARCHE PAS !!! A DEBUGUER
void read_frame_from_file( const char *FileName, TT_Image *T_Image )
{

	int handle;
	int nbel;		// number of end line
	int i, x, y, inc, rs;
	unsigned int width, height;
	unsigned char head[100];

	TT_type *image = T_Image->ptrImage;


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

		// lecture de l'image
	if( read(handle, (void*)image, width*height)!=(width*height) ) {
		printf("Erreur de lecture dans le fichier.\n");
		exit(1);
	}
	close(handle);	


	T_Image->Height 		= height;
	T_Image->Width  		= width;
		// Depend de la memoire utilisee (cf DSP et la VRAM d'acquisition)
		// ici EDRAM
	T_Image->Stride 		= width;
	T_Image->Increment 	= 1;

}
