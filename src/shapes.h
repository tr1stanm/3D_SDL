#include "gsl/gsl_matrix.h"
#include "SDL.h"
#pragma once

// for cube
gsl_matrix** initCube(gsl_matrix**, double);
gsl_matrix** renderCube(gsl_matrix**, gsl_matrix**, int);
void destroyCube(gsl_matrix**);
void destroyPointTrace(gsl_matrix**, int);

// for sierpinski
struct TracePoint;
void renderPoints(struct TracePoint**, double);
void createPointsTrace(struct TracePoint**, int, gsl_matrix*);
void renderPointsTraceBehind(struct TracePoint**, int, int);
void renderPointsTraceFront(struct TracePoint**, int, int);
void destroyPoints(struct TracePoint**, int); 
gsl_matrix* f1_2D(gsl_matrix*);
gsl_matrix* f2_2D(gsl_matrix*);
gsl_matrix* f3_2D(gsl_matrix*);

gsl_matrix* f1_3D(gsl_matrix*);
gsl_matrix* f2_3D(gsl_matrix*);
gsl_matrix* f3_3D(gsl_matrix*);
gsl_matrix* f4_3D(gsl_matrix*);
