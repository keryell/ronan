#########################################
# definitions pour le traitement sur DSP
#########################################

# DSP / compilation sous DOS ( beurk... )
#########################################
CC  = cl30
LNK = lnk30
RM = del
CFLAGS = -q -g -v40 -mr 
LNKFLAGS = -c
DEFINE = -dPAL -dTRIGERR -d_DSP_ 
#-dDEBUG
LIBS = dsp.cmd
SRC_DSP = stat.c graph.c esiee.c videodev.c
PROG = videodev.out

########################################
# source pour la chaine de segmentation 
########################################
SRC_ALGO = deriche.c threshol.c fc.c rl.c 
#norme.c

###############################
# definitions generiques 
###############################
SRC = $(SRC_DSP) $(SRC_UNIX) $(SRC_ALGO) main.c 
OBJ = $(SRC:.c=.obj)

###########
# Cibles 
###########

.SUFFIXES: c obj

.c.obj:
	$(CC) $(CFLAGS) $(DEFINE) -c $< $*.obj

all: $(OBJ)
	$(LNK) $(LNKFLAGS) $(OBJ) $(LIBS) -o $(PROG)
 
clean:
	@$(RM) *.obj
	@$(RM) *.out


