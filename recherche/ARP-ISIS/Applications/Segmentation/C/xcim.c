/* bibliothèques XCIM */
#include <xclib.h>

#include "xcim.h"
#include "comm.h"

extern XCIM_LICENSE LICENSE;

#ifdef VPE1_CORE
extern XCIM_CORE VPE1_CORE[1];
#endif
#ifdef VPE2_CORE
extern XCIM_CORE VPE2_CORE[1];
#endif

XCIM_MODULE xcim;

XCIM_CONNECT cnx=
					{{{ 3, COMPORT_OUT, TRUE},
					  { 1, COMPORT_IN,  TRUE},
					  {-1, -1, TRUE},
					  {-1, -1, TRUE}}};

int FPGA_init( void )
{
	/* declarations pour le module XCIM */

	int status;
	XCIM_VPE *vpe1, *vpe2;

	/* Activation des interruptions */

	asm("	LDHI	0002Fh,R0	");
	asm("	OR	0F800h,R0	");
	asm("	LDPE	R0,IVTP	");
	asm("	LDPE	R0,TVTP	");
	asm("	AND	0E3FFh,ST	");
	asm("	OR	03800h,ST	");

	/* Initialisation du module XCIM */
	
	status = xcim_init(&xcim, CLK_50MHZ, &cnx, &LICENSE );
	if (status != XCIM_OK) {
		printf("xcim_init retourne %d\n", status );
		return -1;
	}
	
	/* Assignation et Configuration des coeurs dans les VPE's */

#ifdef VPE1_CORE	
	vpe1 = xcim_assign(&xcim, 1, VPE1_CORE );
	if( vpe1==NULL ) {
		printf("xcim_assign retourne NULL pour vpe1\n");
		return -1;
	}
#endif
#ifdef VPE2_CORE
	vpe2 = xcim_assign(&xcim, 2, VPE2_CORE );
	if( vpe2==NULL ) {
		printf("xcim_assign retourne NULL pour vpe2\n");
		return -1;
	}
#endif

	status = xcim_configure(&xcim);
	if (status != XCIM_OK) {
		printf("xcim_configure retourne %d\n", status );
		return -1;
	}
	
	return 0;

}


int FPGA_stop( void )
{
	int status;
	
	/* Arret des VPE's */
	status = xcim_shutdown(&xcim);
	if (status != XCIM_OK) {
		printf("xcim_shutdown retourne %d\n", status );
		return -1;
	}
	
	/* Restitution des ressources */
	xcim_free(&xcim);

}