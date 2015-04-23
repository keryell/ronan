/*
 *      TRIG Example Linker File 
 *
 *      @(#)ipdemo.cmd	1.6
 *
 *      Copyright (c) 1994-1995 Transtech Parallel Systems
 */


-L\tps\C40\pacelib

-l pace.lib
-l comport.lib
-l tops.lib
-l trig.lib
-l tdm435.lib
-l threel.lib 
-l prts40.lib

-c
-x
-m dsp.map

-stack 0x4000
-heap 0x40000

MEMORY
{
	RAM0:		org = 0x002ff800 len = 0x400
	RAM1:		org = 0x002ffc00 len = 0x400
	LocalRAM:	org = 0x00300000 len = 0x100000
}

SECTIONS
{
	.vector:	> LocalRAM, align 512
	.cinit:		> LocalRAM
	.const:		> LocalRAM
	.stack:		> LocalRAM
	.data:		> LocalRAM
	.bss:		> LocalRAM, block 0x10000
	.text:		> LocalRAM
	.sysmem:	> LocalRAM
}

