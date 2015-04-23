/* 
 * TRIG Image Processing Example
 *
 * @(#)ipdemo.h	1.4
 * 
 * Copyright (c) 1995 Transtech Parallel Systems 
 */ 
#ifndef _GRAPH_H_
#define _GRAPH_H_ 

#include "trig.h"
#include "tt_type.h"

#define RED 15
#define YELLOW 14
#define GREEN 9
#define BLUE 1

extern t_window	*frame;
extern t_window	*overlay;
extern t_gc	*blue, *red, *black, *green;
extern t_font	*font, *big_font;
extern t_gc	*font_gc, *big_font_gc;
extern t_display *display;

void draw_TDM435_banner(void);
void draw_demo_banner(char*);

void capture_single_frame_WORD( unsigned int *capt_addr );
void draw_frame_WORD( unsigned int *frame2draw );
void capture_single_frame_BYTE( unsigned char *capt_addr );
void draw_frame_BYTE( unsigned char *frame2draw );

void capture_single_frame( TT_Image *TImage );
void draw_frame( TT_Image *TImage, unsigned int *display_addr );

int half_left_screenshot_2_file( const char *FileName );
int write_frame_2_file( TT_Image *TImage, const char *FileName );
int write_result_2_file( const char *FileName );
int write_screen_2_file( const char *FileName );

#endif