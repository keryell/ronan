#ifndef _STAT_H_
#define _STAT_H_

#define FPS	0
#define SPF	1

struct stat
{
	int available;
	int time_count;
	float time;
	unsigned int nb_frame_in;
	unsigned int nb_frame_out;
	int started;
};

void start_stat( );

#endif
