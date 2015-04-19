all : essai_MPE onde_MPE

MPICC = mpicc

% : %.c
	$(MPICC) -o $@ $? -lmpe -L/usr/X11R6/lib -lX11 $(MPI_LIBS) -lm
