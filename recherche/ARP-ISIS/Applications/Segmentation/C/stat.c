#include <stdio.h>
#include <timer40.h>
#include <compt40.h>
#include <intpt40.h>

#include "tdm435.h"
#include "graph.h"
#include "stat.h"

/*#define IT_CAPTURE*/
#define ENABLE_WAIT_VBLANK

/* 40MHz input clock */
/*extern float CLOCK_PER_SEC=20000000.0;*/

/* 50MHz input clock */
extern float CLOCK_PER_SEC=25000000.0;

unsigned int nb_frame_in;
unsigned int nb_frame_out;
unsigned int clock_1s;

struct stat Stat = { 0, 0, 0, 0, 0, 0 };

char msg_stat[100];

	/* fonction sous it, appelee a la fin de la capture video */
	/* incremente le nombre d'images capturee */
interrupt
void inc_nb_frame_in(void)
{
	nb_frame_in++;
}

inline
void print_stat(void)
{
	char msg[50];

#ifndef IT_CAPTURE
	Stat.nb_frame_in = 25;
#endif
	sprintf(msg, "Captured frames: %d", Stat.nb_frame_in );	
	print_right(overlay, font_gc, msg, 500);

	if( Stat.time_count ) { // SPF
		sprintf(msg, "Seconds per frame: %.4f", Stat.time );
	}
	else
		sprintf(msg, "Frames per second: %d", Stat.nb_frame_out );
	print_right(overlay, font_gc, msg, 1000);

	Stat.available = 0;
}

	/* fonction sous it */
	/* elle est appelee toutes les secondes pour afficher les stats */
	/* sur le nombre d'images capturees et le nombre d'images affichees */
interrupt
void frame_stat(void)
{
#ifdef IT_CAPTURE
	Stat.nb_frame_in = nb_frame_in;
	nb_frame_in = 0;
#endif
	Stat.nb_frame_out = nb_frame_out; 
	Stat.available = 1;
	nb_frame_out = 0;

//	clock_1s = !clock_1s;
}

void start_stat(void)
{
	TIMER_REG  *tim_ptr = TIMER_BASE;	/* TIMER 0 REGISTER POINTER     */

	INT_DISABLE();
	set_ivtp(DEFAULT);

#ifdef IT_CAPTURE
	/* initialise l'it de capture */
	display->regs->VCONT_CAPTURE[4] &= 0xFC;	/* it a chaque frame */
	display->regs->VCONT_CAPTURE[4] |= 0x01;
	while (d_load_VCAPTURE_reg(display->regs->VCONT_CAPTURE) == TERR_Failed);
	install_int_vector(inc_nb_frame_in, 4);	/* 0x4 : IIOF1 "it externe 1" */
	asm("	or 90h, IIF");			/* active l'it externe 1 */
#endif

	/* initialise l'it de timer a 1s */
	install_int_vector(frame_stat, 2);	/* 0x2 : Timer 0 interrupt */ 
	alarm(1);				/* interruption toutes les secs */
	set_iie(TIMER0);			/* ENABLE TINT0 BIT IN IIE	    */


	/* initialisation des compteurs */
	nb_frame_in = 0;
	nb_frame_out = 0;
	clock_1s = 0;
	Stat.started = 1;
	Stat.available = 0;
	Stat.time_count = FPS;
	Stat.time = 0;

        /* Zouuuuu, on active les interruptions */
	INT_ENABLE();

}

inline 
void stop_stat(void)
{

	INT_DISABLE();
	/* supprime l'it timer */
	reset_iie(TIMER0);
	deinstall_int_vector(2);

#ifdef IT_CAPTURE
	/* supprime l'it capture */
	asm("	and 6Fh,IIF");
	deinstall_int_vector(4);
#endif
	Stat.started = 0;
	INT_ENABLE();

}

	// type==FPS || type==SPF
inline 
set_stat( unsigned int type )
{
	Stat.time_count = type;
	Stat.available = 0;
}

inline 
void sync_stat(void)
{
	int nbcycles;

	if( Stat.time_count ) { //SPF
		nbcycles = time_stop(1);
		Stat.time = (nbcycles/CLOCK_PER_SEC)*2; // *2 => cf user manual section 9.?
		time_start(1);
		Stat.available = 1;
	}
	else nb_frame_out++;	
	if( Stat.available ) print_stat();

#ifdef	ENABLE_WAIT_VBLANK
	t_wait_capture_vblank();
#endif

}