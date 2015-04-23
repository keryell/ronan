/* 
 *      TDM435 driver library.
 *
 *      @(#)tdm435.h	1.7
 * 
 *      Copyright 1995 Transtech Parallel Systems 
 */

#ifndef _TRIG_H
#include "trig.h"
#endif

typedef struct _t_registers
{
	/* TDM435 registers storage.                            */
	int LM1882[20];        /* Table for LM1882 registers*/
	int disp_clock;        /* A.K.A. AV9110 */
	int capt_clock;        /* 24bit data for the capture clock */
	int VCONT_CAPTURE[8];  /* Table for VCONTROL registers */
	int VCONT_DISPLAY[8];  /* Table for VCONTROL registers */
	int BT477_commd;       /* BT477 command to set sync */
	int BT252[5];          /* table for BT252 registers */
	int BT261[32];         /* Table for BT261 registers */

	/* TDM435 default value storage. */
	int V_FRONT_P;
	int V_ACTIVE_LINE;
	int H_SYNC_START;
	int H_SYNC_STOP;

};


#define AV9110 disp_clock


/* 
 * Registers for TDM435. (pointer initialized in regaddrs.c)
 */

typedef volatile unsigned char vuchar;
typedef volatile unsigned int vuint;
typedef unsigned char uchar;
typedef unsigned int uint;

extern uchar		*TDM435_8_vram_adr_a;
extern uchar		*TDM435_8_vram_adr_b;

extern uchar		*TDM435_8_vram_adr;

extern uint		*TDM435_32_vram_adr_a;
extern uint		*TDM435_32_vram_adr_b;

extern uint		*TDM435_32_vram_mask_a;
extern uint		*TDM435_32_vram_mask_b;

extern uint		*TDM435_32_vram_col_a;
extern uint		*TDM435_32_vram_col_b;

extern uint		*TDM435_32_vram_adr;

extern uint		*TDM435_vram_mask_a;
extern uint		*TDM435_vram_mask_b;

extern uint		*TDM435_vram_col_a;
extern uint		*TDM435_vram_col_b;

extern uchar		*TDM435_8_overlay_adr;

extern uint		*TDM435_32_overlay_adr;

extern uint		*TDM435_overlay_mask;
extern uint		*TDM435_overlay_col;

extern vuint		*TDM435_BT252_adr;
extern vuint		*TDM435_BT252_d_pal;
extern vuint		*TDM435_BT252_commd;

extern vuint		*TDM435_BT261_adr;
extern vuint		*TDM435_BT261_d_pal;

extern vuint		*TDM435_BT477_r_pal;
extern vuint		*TDM435_BT477_w_pal;
extern vuint		*TDM435_BT477_d_pal;
extern vuint		*TDM435_BT477_r_msk;

extern vuint		*TDM435_BT477_r_ovl;
extern vuint		*TDM435_BT477_w_ovl;
extern vuint		*TDM435_BT477_d_ovl;
extern vuint		*TDM435_BT477_commd;

extern vuint		*TDM435_LM1882_A;
extern vuint		*TDM435_LM1882_B;

extern vuint		*TDM435_AV9110;

extern vuint		*TDM435_VCAPTURE_1;
extern vuint		*TDM435_VCAPTURE_2;

extern vuint		*TDM435_VDISPLAY_1;
extern vuint		*TDM435_VDISPLAY_2;

extern vuint		*TDM435_VCONTROL;
extern vuint		*TDM435_VCOMMAND;

#define TDM435_VCAPT_UPDATE	((vuint *)0xBD00000C)
#define TDM435_VDISP_UPDATE	((vuint *)0xBD000003)




int d_select_capture_video(t_display *disp, int video);
int d_select_capture_sync(t_display *disp, int video);
void d_init_ints(void);
int d_load_LM1882_reg(int *table);
int d_load_AV9110_reg(int disp, int capt);
int d_load_VCAPTURE_reg(int *table);
int d_load_VDISPLAY_reg(int *table);
int d_load_BT477_ovl(int *table);
int d_load_BT477_pal(int *table);
int d_load_BT252_pal(int *table);
int d_load_BT252_reg(int *table);
int d_load_BT261_reg(int *table);
int d_install_TDM435_ovl_clut(t_clut *clut);
int d_install_TDM435_frame_clut(t_clut *clut);
int d_install_TDM435_capture_clut(t_clut *clut);
int d_init_disp_TDM435(t_display *disp);
int d_set_disp_TDM435_frame(t_display *disp, int frame);
int d_clr_disp_TDM435_frame(t_display *disp, int frame, int col);
int d_clr_disp_TDM435_overlay(t_display *disp, int overlay, int col);
