#include "gsl/gsl_matrix.h"
#include "SDL.h"
#pragma once

// for cube
gsl_matrix** initCube(gsl_matrix**, double);
gsl_matrix** renderCube(gsl_matrix**, gsl_matrix**, int);
void destroyCube(gsl_matrix**);
void destroyPointTrace(gsl_matrix**, int);

// for sierpinski
void renderPoints(double);
gsl_matrix* createPoint(gsl_matrix*);
gsl_matrix* drawPoint(gsl_matrix*, double);
void destroyPoints(gsl_matrix**, int); 
gsl_matrix* f1_2D(gsl_matrix*);
gsl_matrix* f2_2D(gsl_matrix*);
gsl_matrix* f3_2D(gsl_matrix*);

gsl_matrix* f1_3D(gsl_matrix*);
gsl_matrix* f2_3D(gsl_matrix*);
gsl_matrix* f3_3D(gsl_matrix*);
gsl_matrix* f4_3D(gsl_matrix*);
//gsl_matrix* f5_3D(gsl_matrix*);
