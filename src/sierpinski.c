#include "3DR.h"
#include "shapes.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include "SDL2_image/SDL_image.h"

const int CANVASSIZE = 700;
const double QUADSIZE = CANVASSIZE / 2.0;
const int NUM_POINTSTOTRACE = 60 * 60 * 10;	// 10 minutes of frames to trace points for

typedef struct TracePoint {
	gsl_matrix* point;
	short isBehind;
} TracePoint;

int main() {
	const int FPS = 120;
	const double frameDelay = 1000.0/FPS;
	Uint32 frameStart;
	double frameTime;
	double increment = 0;	// for the angle and the points index for the pointsTrace
	TracePoint **tracePoint = (TracePoint**)malloc(sizeof(TracePoint*) * 3);
	char fname[200];

	for(int i = 0; i < 3; ++i) tracePoint[i] = (TracePoint*)malloc(sizeof(TracePoint) * NUM_POINTSTOTRACE);
	for(int i = 0; i < 3; ++i) tracePoint[i][0].point = gsl_matrix_alloc(3,1);
	
	init("sierpinski", CANVASSIZE, CANVASSIZE, 0);

	//system("mkdir ../anim");
	while(isRunning) {
		frameStart = SDL_GetTicks();
		renderPoints(tracePoint, increment++);
		SDL_RenderPresent(renderer);

		/*
		sprintf(fname, "../anim/frame_%04d.png", (int)increment);
		SDL_Surface *surface = SDL_CreateRGBSurface(0, CANVASSIZE, CANVASSIZE, 
				     32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
		SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ARGB8888, surface->pixels, surface->pitch);

		IMG_SavePNG(surface, fname);
		*/

		frameTime = SDL_GetTicks() - frameStart;
		if(frameTime < frameDelay) SDL_Delay(frameDelay - frameTime);
	}

	destroyPoints(tracePoint, increment);
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
}

void createPointsTrace(struct TracePoint **tracePoint, int newPIndex, gsl_matrix *rMatrix) {


	if(!newPIndex) {	// sets trace.pointsTrace's initial points

		gsl_matrix_set(tracePoint[0][0].point, 0, 0, 1);
		gsl_matrix_set(tracePoint[0][0].point, 1, 0, 1);
		gsl_matrix_set(tracePoint[0][0].point, 2, 0, 0);
		gsl_matrix_set(tracePoint[1][0].point, 0, 0, 0);
		gsl_matrix_set(tracePoint[1][0].point, 1, 0, 1);
		gsl_matrix_set(tracePoint[1][0].point, 2, 0, 1);
		gsl_matrix_set(tracePoint[2][0].point, 0, 0, 1);
		gsl_matrix_set(tracePoint[2][0].point, 1, 0, 0);
		gsl_matrix_set(tracePoint[2][0].point, 2, 0, 1);
	}
	else {
		for(int i = 0; i < 3; ++i) {
			tracePoint[i][newPIndex].point = matrixMul(rMatrix, tracePoint[i][0].point);
			if(gsl_matrix_get(tracePoint[i][newPIndex].point, 2, 0) < 0) {
				tracePoint[i][newPIndex].isBehind = 1;
			}
			else tracePoint[i][newPIndex].isBehind = 0;
		}
	
	}
}

void renderPointsTraceBehind(struct TracePoint **tracePoint, int pIndex, int pointToTrace) {
	for(int i = 1; i <= pIndex; ++i) {
		if(tracePoint[pointToTrace][i].isBehind) {
			gsl_matrix* proj1 = project(tracePoint[pointToTrace][i].point, 3);
			gsl_matrix* proj2 = project(tracePoint[pointToTrace][i-1].point, 3);
			gsl_matrix_scale(proj1, QUADSIZE / 0.75);
			gsl_matrix_scale(proj2, QUADSIZE / 0.75);

			SDL_SetRenderDrawColor(renderer, 127*pointToTrace, 50*pointToTrace, 255 - (60*pointToTrace), 255);
			plotVector(proj1, proj2);
			gsl_matrix_free(proj1);
			gsl_matrix_free(proj2);
		}
	}
}

void renderPointsTraceFront(struct TracePoint **tracePoint, int pIndex, int pointToTrace) {
	for(int i = 1; i <= pIndex; ++i) {
		if(!tracePoint[pointToTrace][i].isBehind) {
			gsl_matrix* proj1 = project(tracePoint[pointToTrace][i].point, 3);
			gsl_matrix* proj2 = project(tracePoint[pointToTrace][i-1].point, 3);
			gsl_matrix_scale(proj1, QUADSIZE / 0.75);
			gsl_matrix_scale(proj2, QUADSIZE / 0.75);

			SDL_SetRenderDrawColor(renderer, 127*pointToTrace, 50*pointToTrace, 255 - (60*pointToTrace), 255);
			plotVector(proj1, proj2);
			gsl_matrix_free(proj1);
			gsl_matrix_free(proj2);
		}
	}
}

void renderPoints(struct TracePoint **tracePoint, double inc) {
	gsl_matrix *points = gsl_matrix_alloc(4, 1);
	gsl_matrix *toDraw = gsl_matrix_alloc(3, 1);
	gsl_matrix_set_zero(points);
	gsl_matrix_set(points, 3, 0, 1);
	double r;

	handleEvents();
	SDL_SetRenderDrawColor(renderer, 55, 55, 55, 255);
	SDL_RenderClear(renderer);

	gsl_matrix* rotationMatrix = gsl_matrix_alloc(3, 3);
	gsl_matrix_set_identity(rotationMatrix);
	
	rotationMatrix = rotateX(rotateY(rotateZ(rotationMatrix, inc * 1.4), inc * 1.12), inc * 1.33);

	createPointsTrace(tracePoint, (int)inc, rotationMatrix);

	for(int i = 0; i < 3; ++i) renderPointsTraceBehind(tracePoint, (int)inc, i);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	for(int i = 0; i < 25000; ++i) {
		r = drand48();
		if(r < .25) points = f1_3D(points);
		else if(r < .5) points = f2_3D(points);
		else if(r < .75) points = f3_3D(points);
		else points = f4_3D(points);

		gsl_matrix_set(toDraw, 0, 0, gsl_matrix_get(points, 0, 0));
		gsl_matrix_set(toDraw, 1, 0, gsl_matrix_get(points, 1, 0));
		gsl_matrix_set(toDraw, 2, 0, gsl_matrix_get(points, 2, 0));
		//gsl_matrix_scale(toDraw, QUADSIZE / 1.25);

		gsl_matrix* newDraw = matrixMul(rotationMatrix, toDraw);
		gsl_matrix_free(toDraw);
		toDraw = newDraw;

		//gsl_matrix *proj = orthoProject(toDraw);
		gsl_matrix *proj = project(toDraw, 3);
		gsl_matrix_scale(proj, QUADSIZE / .75);

		plotPoint(proj);
		gsl_matrix_free(proj);
	}

	for(int i = 0; i < 3; ++i) renderPointsTraceFront(tracePoint, (int)inc, i);
	
	gsl_matrix_free(points);
	gsl_matrix_free(toDraw);
	gsl_matrix_free(rotationMatrix);
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
	gsl_matrix_set(f2, 1, 3, 0.5);
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
	gsl_matrix_set(f3, 1, 3, 0.5);
	gsl_matrix_set(f3, 2, 2, 0.5);
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
	gsl_matrix_set(f4, 0, 3, 0.5);
	gsl_matrix_set(f4, 1, 1, 0.5);
	gsl_matrix_set(f4, 2, 2, 0.5);
	gsl_matrix_set(f4, 2, 3, 0.5);
	gsl_matrix_set(f4, 3, 3, 1);

	gsl_matrix* returnval = matrixMul(f4, m);
	gsl_matrix_free(f4);
	gsl_matrix_free(m);
	return returnval;
}

void destroyPoints(struct TracePoint **tracePoint, int numPoints) {
	for(int i = 0; i < 3; ++i) {
		for(int j = 0; j < NUM_POINTSTOTRACE; ++j) {
			gsl_matrix_free(tracePoint[i][j].point);
		}
		free(tracePoint[i]);
	}
	free(tracePoint);
}

