#ifndef _XCIM_H_
#define _XCIM_H_

#define LICENSE AF00039

//#ifdef VPE1_CORE_NAME
//#define VPE1_CORE VPE1_CORE_NAME
//#endif
#define VPE1_CORE Pipe
#ifdef VPE2_CORE_NAME
#define VPE2_CORE VPE2_CORE_NAME
#endif

int FPGA_init( void );
int FPGA_stop( void );

#endif

