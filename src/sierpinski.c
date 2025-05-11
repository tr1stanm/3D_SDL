#include "3DR.h"
#include "shapes.h"
#include <SDL2/SDL_render.h>

const int CANVASSIZE = 600;
const double QUADSIZE = CANVASSIZE / 2.0;
double x = 0;
double y = 0;

int main() {
	const int FPS = 60;
	const double frameDelay = 1000.0/FPS;
	Uint32 frameStart;
	int frameTime;
	int angle = 0;

	init("sierpinski", CANVASSIZE, CANVASSIZE, 0);

	while(isRunning) {

		frameStart = SDL_GetTicks();
		renderPoints(angle++);
		frameTime = SDL_GetTicks() - frameStart;
		if(frameTime < frameDelay) SDL_Delay(frameDelay - frameTime);
	}
}

void renderPoints(double angle) {
	gsl_matrix *points = gsl_matrix_alloc(4, 1);
	gsl_matrix *toDraw = gsl_matrix_alloc(3, 1);
	//for(int i = 0; i < 4; ++i) gsl_matrix_set(points, i, 0, 1);
	gsl_matrix_set_zero(points);
	gsl_matrix_set(points, 3, 0, 1);
	double r;

	handleEvents();
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	for(int i = 0; i < 20000; ++i) {
		r = drand48();
		if(r < .2) points = f1_3D(points);
		else if(r < .4) points = f2_3D(points);
		else if(r < .6) points = f3_3D(points);
		else if(r < .8) points = f4_3D(points);
		else points = f5_3D(points);

		gsl_matrix_set(toDraw, 0, 0, gsl_matrix_get(points, 0, 0));
		gsl_matrix_set(toDraw, 1, 0, gsl_matrix_get(points, 1, 0));
		gsl_matrix_set(toDraw, 2, 0, gsl_matrix_get(points, 2, 0));
		//gsl_matrix_scale(toDraw, QUADSIZE / 1.25);

		toDraw = rotateX(rotateY(rotateZ(toDraw, angle), angle), angle);
		//gsl_matrix *proj = orthoProject(toDraw);
		gsl_matrix *proj = project(toDraw, 2);
		gsl_matrix_scale(proj, QUADSIZE / 1.25);

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		plotPoint(proj);
		gsl_matrix_free(proj);
	}
	gsl_matrix_free(points);
	gsl_matrix_free(toDraw);
	SDL_RenderPresent(renderer);
}

gsl_matrix* f1_2D(gsl_matrix *m) {
	gsl_matrix *f1 = gsl_matrix_alloc(4, 4);
	gsl_matrix_set_zero(f1);
	gsl_matrix_set(f1, 0, 0, 0.5);
	gsl_matrix_set(f1, 1, 1, 0.5);
	gsl_matrix_set(f1, 3, 3, 1);

	gsl_matrix* returnval = matrixMul(f1, m);
	gsl_matrix_free(f1);
	gsl_matrix_free(m);
	return returnval;
}

gsl_matrix* f2_2D(gsl_matrix *m) {
	gsl_matrix *f2 = gsl_matrix_alloc(4, 4);
	gsl_matrix_set_zero(f2);
	gsl_matrix_set(f2, 0, 0, 0.5);
	gsl_matrix_set(f2, 1, 1, 0.5);
	gsl_matrix_set(f2, 3, 3, 1);
	gsl_matrix_set(f2, 0, 3, 0.5);
	
	gsl_matrix* returnval = matrixMul(f2, m);
	gsl_matrix_free(f2);
	gsl_matrix_free(m);
	return returnval;
}

gsl_matrix* f3_2D(gsl_matrix *m) {
	gsl_matrix *f3 = gsl_matrix_alloc(4, 4);
	gsl_matrix_set_zero(f3);
	gsl_matrix_set(f3, 0, 0, 0.5);
	gsl_matrix_set(f3, 1, 1, 0.5);
	gsl_matrix_set(f3, 3, 3, 1);
	gsl_matrix_set(f3, 0, 3, 0.25);
	gsl_matrix_set(f3, 1, 3, 0.5);
	
	gsl_matrix* returnval = matrixMul(f3, m);
	gsl_matrix_free(f3);
	gsl_matrix_free(m);
	return returnval;
}

gsl_matrix* f1_3D(gsl_matrix *m) {
	gsl_matrix *f1 = gsl_matrix_alloc(4, 4);
	gsl_matrix_set_zero(f1);
	gsl_matrix_set(f1, 0, 0, 0.5);
	gsl_matrix_set(f1, 1, 1, 0.5);
	gsl_matrix_set(f1, 2, 2, 0.5);
	gsl_matrix_set(f1, 3, 3, 1);
	
	gsl_matrix* returnval = matrixMul(f1, m);
	gsl_matrix_free(f1);
	gsl_matrix_free(m);
	return returnval;
}

gsl_matrix* f2_3D(gsl_matrix *m) {
	gsl_matrix *f2 = gsl_matrix_alloc(4, 4);
	gsl_matrix_set_zero(f2);
	gsl_matrix_set(f2, 0, 0, 0.5);
	gsl_matrix_set(f2, 1, 1, 0.5);
	gsl_matrix_set(f2, 2, 2, 0.5);
	gsl_matrix_set(f2, 2, 3, 0.5);
	gsl_matrix_set(f2, 3, 3, 1);

	gsl_matrix* returnval = matrixMul(f2, m);
	gsl_matrix_free(f2);
	gsl_matrix_free(m);
	return returnval;
}

gsl_matrix* f3_3D(gsl_matrix *m) {
	gsl_matrix *f3 = gsl_matrix_alloc(4, 4);
	gsl_matrix_set_zero(f3);
	gsl_matrix_set(f3, 0, 0, 0.5);
	gsl_matrix_set(f3, 0, 3, 0.5);
	gsl_matrix_set(f3, 1, 1, 0.5);
	gsl_matrix_set(f3, 2, 2, 0.5);
	gsl_matrix_set(f3, 2, 3, 0.5);
	gsl_matrix_set(f3, 3, 3, 1);

	gsl_matrix* returnval = matrixMul(f3, m);
	gsl_matrix_free(f3);
	gsl_matrix_free(m);
	return returnval;
}

gsl_matrix* f4_3D(gsl_matrix *m) {
	gsl_matrix *f4 = gsl_matrix_alloc(4, 4);
	gsl_matrix_set_zero(f4);
	gsl_matrix_set(f4, 0, 0, 0.5);
	gsl_matrix_set(f4, 1, 1, 0.5);
	gsl_matrix_set(f4, 2, 2, 0.5);
	gsl_matrix_set(f4, 2, 3, 0.5);
	gsl_matrix_set(f4, 3, 3, 1);

	gsl_matrix* returnval = matrixMul(f4, m);
	gsl_matrix_free(f4);
	gsl_matrix_free(m);
	return returnval;
}

gsl_matrix* f5_3D(gsl_matrix *m) {
	gsl_matrix *f5 = gsl_matrix_alloc(4, 4);
	gsl_matrix_set_zero(f5);
	gsl_matrix_set(f5, 0, 0, 0.5);
	gsl_matrix_set(f5, 0, 3, 0.25);
	gsl_matrix_set(f5, 1, 1, 0.5);
	gsl_matrix_set(f5, 1, 3, 0.5);
	gsl_matrix_set(f5, 2, 2, 0.5);
	gsl_matrix_set(f5, 2, 3, 0.25);
	gsl_matrix_set(f5, 3, 3, 1);

	gsl_matrix* returnval = matrixMul(f5, m);
	gsl_matrix_free(f5);
	gsl_matrix_free(m);
	return returnval;
}

gsl_matrix* createPoint(gsl_matrix *point) {
	double r = drand48();

	if(r < .2) point = f1_3D(point);
	else if(r < .4) point = f2_3D(point);
	else if(r < .6) point = f3_3D(point);
	else if(r < .8) point = f4_3D(point);
	else point = f5_3D(point);

	return point;
}

gsl_matrix* drawPoint(gsl_matrix *point, double angle) {
	gsl_matrix *toDraw = gsl_matrix_alloc(3, 1);
	
	gsl_matrix_set(toDraw, 0, 0, gsl_matrix_get(point, 0, 0));
	gsl_matrix_set(toDraw, 1, 0, gsl_matrix_get(point, 1, 0));
	gsl_matrix_set(toDraw, 2, 0, gsl_matrix_get(point, 2, 0));

	

	toDraw = rotateX(rotateY(rotateZ(toDraw, angle), angle), angle);
	gsl_matrix *proj = orthoProject(point);
	gsl_matrix_scale(proj, QUADSIZE / 1.25);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	plotPoint(proj);
	gsl_matrix_free(proj);

	gsl_matrix_free(toDraw);
	SDL_RenderPresent(renderer);

	return point;
}

void destroyPoints(gsl_matrix **points, int numPoints) {
	for(int i = 0; i < numPoints; ++i) gsl_matrix_free(points[i]);
}

