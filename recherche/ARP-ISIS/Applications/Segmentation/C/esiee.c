#include "graph.h"
#include "esiee.h"

extern unsigned int *frame_addr;

void draw_logo_esiee( )
{
	unsigned int *LOGO_addr = (unsigned int *) frame_addr+693+10*1024;
	register unsigned int x, y, pos_y, pos_x, word, pos_logo;

	for( y=0, pos_logo=0 ; y<SIZE_LOGO ; y++){
		pos_y = y<<10;				/* y*stride (1024) */
		for( x=0 ; x<SIZE_LOGO ; x+=4, pos_logo+=4 ) {
			pos_x = pos_y + x;
			word = 	  logo_esiee[pos_logo+3]<<24
				| logo_esiee[pos_logo+2]<<16
 				| logo_esiee[pos_logo+1]<<8
				| logo_esiee[pos_logo];

			LOGO_addr[ pos_x ] = word;

		}
	}
}