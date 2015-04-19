#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include "mpi.h"
#include "mpe.h"
#include "mpe_graphics.h"

enum { NB_COLORS = 256,
       X_SIZE = 256,
       Y_SIZE = 256,
       X_ZOOM = 2,
       Y_ZOOM = 2};

#define K (1/300.0)
#define g 9.81

double alpha = K*g;

typedef double espace[X_SIZE][Y_SIZE];

void
initialise(espace u, espace v,
	   espace w,
	   espace side,
	   espace p) {
  int i, j;

  for(i = 0; i < X_SIZE; i++)
    for(j = 0; j < Y_SIZE; j++) {
      u[i][j] = 0;
      v[i][j] = 0;
      w[i][j] = 0;
      side[i][j] = K;
      p[i][j] = 2600.0;
    }
}


void
afficheOnde(MPE_XGraph graph,
	    MPE_Color colorArray[NB_COLORS],
	    espace w) {
  int i, j;

  for(i = 0; i < X_SIZE; i++)
    for(j = 0; j < Y_SIZE; j++) {
      unsigned char couleur = w[i][j] + NB_COLORS/2;
      //      printf("%f ", couleur);
      MPE_Fill_rectangle(graph, i*X_ZOOM, j*Y_ZOOM, X_ZOOM, Y_ZOOM, colorArray[couleur]);
    }
  /* Affiche réellement à l'écran : */
  MPE_Update(graph);
}


void
ajouteGoutte(espace w,
	     int x0, int y0) {
  int i, j, xmin, xmax, ymin, ymax;
  enum {
    DIAMETRE_GOUTTE = 5,
    HAUTEUR_GOUTTE = 20
  };
  xmin = x0 - DIAMETRE_GOUTTE;
  if (xmin < 0)
    xmin = 0;
  xmax = x0 + DIAMETRE_GOUTTE;
  if (xmax > X_SIZE)
    xmax = X_SIZE - 1;
  ymin = y0 - DIAMETRE_GOUTTE;
  if (ymin < 0)
    ymin = 0;
  ymax = y0 + DIAMETRE_GOUTTE;
  if (ymax > Y_SIZE)
    ymax = Y_SIZE - 1;

  for(i = xmin; i <= xmax; i++)
    for(j = ymin; j <= ymax; j++) {
      int dx = i - x0;
      int dy = j - y0;
      int d2 = dx * dx + dy * dy;
      if (d2 <= DIAMETRE_GOUTTE*DIAMETRE_GOUTTE) 
	w[i][j] += HAUTEUR_GOUTTE*(DIAMETRE_GOUTTE*DIAMETRE_GOUTTE - d2);
    }
}


void
calculeOnde(espace u, espace v,
	    espace w,
	    espace side,
	    espace p) {
  int i, j;

  for(i = 1; i < X_SIZE - 1; i++)
    for(j = 1; j < Y_SIZE - 1; j++) {
      double up, vp;
      /* grad w */
      up = w[i + 1][j] - w[i][j];
      vp = w[i][j + 1] - w[i][j];
      /* Intègre la vitesse : */
      u[i][j]  += up*alpha;
      v[i][j]  += vp*alpha;
    }
  for(i = 1; i < X_SIZE - 1; i++)
    for(j = 1; j < Y_SIZE - 1; j++) {
      double wp;
      /* div v */
      wp = (u[i][j] - u[i - 1][j]) + (v[i][j] - v[i][j - 1]);
      wp *= side[i][j]*(p[i][j] + w[i][j]);
      /* Intègre la hauteur d'eau : */
      w[i][j] += wp;
      /* Rajoute un terme de dissipation pour l'amortissement : */
      w[i][j] *= 0.99;
    }
}


int main(int argc, char *argv[])
{
  int t, np, myid, i, myWindowOpened, button, x, y, wasPressed;
  /* Profondeur moyenne de l'eau : */
  espace p;
  /* Profondeur relative de l'eau par rapport à p : */
  espace w;
  /* Vitesse en x et y : */
  espace u, v;
  /* Contient 0 si c'est un obstacle ou bord, le coefficient de
     transmission sinon :*/
  espace side;

  MPE_Color colorArray[NB_COLORS];
  MPE_XGraph graph;
  MPI_Init( &argc, &argv );
  MPI_Comm_rank( MPI_COMM_WORLD, &myid );
  MPI_Comm_size( MPI_COMM_WORLD, &np );

  /* Les mans de MPE sont dans http://www-unix.mcs.anl.gov/mpi/www/www4/ */

  myWindowOpened = (MPE_Open_graphics(&graph, MPI_COMM_WORLD, (char *)0,
				      100, 200,
				      X_SIZE*X_ZOOM, Y_SIZE*Y_ZOOM, 0)
		    == MPE_SUCCESS);

  /* Crée une palette graphique : */
  MPE_Make_color_array(graph, NB_COLORS, colorArray);

  initialise(u, v, w, side, p);
    ajouteGoutte(w, 100, 50);
  for (t = 0; t < 1000; t++) {
    afficheOnde(graph, colorArray, w);
    calculeOnde(u, v, w, side, p);
    /* Bug possible si exécuté par plusieurs processus... */
    if (myid == 0) {
      MPE_Iget_mouse_press(graph, &x, &y, &button, &wasPressed);
      if (wasPressed)
	ajouteGoutte(w, x/X_ZOOM, y/Y_ZOOM);
    }
  }
  
  /* Seul le processeur 0 lit la souris : */
  if (myid == 0) {
    printf("Pour sortir, cliquer sur un bouton :\n");
    MPE_Get_mouse_press(graph, &x, &y, &button );
    printf("bouton %d, x = %d, y = %d\n", button, x, y);
  }

  /* Attend que tout le monde soit prêt à finir : */
  MPI_Barrier(MPI_COMM_WORLD);

  MPE_Close_graphics(&graph);
  MPI_Finalize();
}
