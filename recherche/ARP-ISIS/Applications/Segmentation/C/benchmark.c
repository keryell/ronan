#include <stdio.h>

extern unsigned char Ttiming;

#ifdef _UNIX_
#include <sys/time.h>
static struct timeval ref_time;
#endif

#ifdef _DSP_
#include "stat.h"
extern struct stat Stat;
extern float CLOCK_PER_SEC;
static int ref_time = 0;	
#endif

#ifdef _DSP_
inline
#endif
void bench_start( void )
{
#ifdef _UNIX_
	struct timezone tzp;
	
	 gettimeofday (&ref_time, &tzp);	
#endif
#ifdef _DSP_
	if( !Stat.started ) start_stat( );
	ref_time = time_read( 1 );	
#endif

}

	// retourne le temps en millisecondes entre
	// bench_start et bench_read
#ifdef _DSP_
inline
#endif
float bench_read( void )
{
#ifdef _UNIX_
	struct timezone tzp;	
	struct timeval get_time, lapsed;
	
	 gettimeofday (&get_time, &tzp);

	if (ref_time.tv_usec > get_time.tv_usec) {
		get_time.tv_usec += 1000000;
		get_time.tv_sec--;
	}

	lapsed.tv_usec = get_time.tv_usec - ref_time.tv_usec;
	lapsed.tv_sec  = get_time.tv_sec  - ref_time.tv_sec;
	
	return lapsed.tv_usec/1000 + lapsed.tv_sec*1000;
#endif

#ifdef _DSP_	
	unsigned int get_time;
	get_time = time_read( 1 );
	return ((get_time-ref_time)/CLOCK_PER_SEC)*2000 ;
#endif
	
}

#ifdef _DPS_
inline
#endif
void bench_print( const char *Msg )
{
	if(Ttiming) printf("%-25s : %8.2f ms\n", Msg, bench_read() );

}
