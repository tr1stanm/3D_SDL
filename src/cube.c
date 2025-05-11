#include "3DR.h"
#include "shapes.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>

const int CANVASSIZE = 700;
const double QUADSIZE = CANVASSIZE / 2.0;

int main() {
	const int FPS = 60;
	const double frameDelay = 1000.0/FPS;
	Uint32 frameStart;
	int frameTime;

	init("cube", CANVASSIZE, CANVASSIZE, 0);
	const double CUBESIZE = 1;
	gsl_matrix **cube = (gsl_matrix**)malloc(sizeof(gsl_matrix*) * 8);
	cube = initCube(cube, CUBESIZE);

	while(isRunning) {
		frameStart = SDL_GetTicks();
		cube = renderCube(cube);

		frameTime = SDL_GetTicks() - frameStart;
		if(frameTime < frameDelay) SDL_Delay(frameDelay - frameTime);
	}
	destroyCube(cube);
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
}

gsl_matrix** initCube(gsl_matrix **cube, double cubeSize) {
	double p1[3] = {-cubeSize, -cubeSize, -cubeSize};
	double p2[3] = {cubeSize, -cubeSize, -cubeSize};
	double p3[3] = {cubeSize, cubeSize, -cubeSize};
	double p4[3] = {-cubeSize, cubeSize, -cubeSize};
	double p5[3] = {-cubeSize, -cubeSize, cubeSize};
	double p6[3] = {cubeSize, -cubeSize, cubeSize};
	double p7[3] = {cubeSize, cubeSize, cubeSize};
	double p8[3] = {-cubeSize, cubeSize, cubeSize};

	cube[0] = set3DPoint(p1);
	cube[1] = set3DPoint(p2);
	cube[2] = set3DPoint(p3);
	cube[3] = set3DPoint(p4);
	cube[4] = set3DPoint(p5);
	cube[5] = set3DPoint(p6);
	cube[6] = set3DPoint(p7);
	cube[7] = set3DPoint(p8);

	return cube;
}

gsl_matrix** renderCube(gsl_matrix **cube) {
	handleEvents();
	gsl_matrix **proj = (gsl_matrix**)malloc(sizeof(gsl_matrix*) * 8);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	for(int i = 0; i < 8; ++i) {
		cube[i] = rotateX(rotateY(rotateZ(cube[i], 1.3), .75), 1.24);
		proj[i] = project(cube[i], 3);
		//proj[i] = orthoProject(cube[i]);

		gsl_matrix_scale(proj[i], QUADSIZE);
	}

	plotVector(proj[0], proj[1]);
	plotVector(proj[1], proj[2]);
	plotVector(proj[2], proj[3]);
	plotVector(proj[3], proj[0]);
	plotVector(proj[4], proj[5]);
	plotVector(proj[5], proj[6]);
	plotVector(proj[6], proj[7]);
	plotVector(proj[7], proj[4]);
	plotVector(proj[0], proj[4]);
	plotVector(proj[1], proj[5]);
	plotVector(proj[2], proj[6]);
	plotVector(proj[3], proj[7]);

	for(int i = 0; i < 8; ++i) gsl_matrix_free(proj[i]);
	free(proj);
	SDL_RenderPresent(renderer);
	return cube;
}

void destroyCube(gsl_matrix **cube) {
	for(int i = 0; i < 8; ++i) gsl_matrix_free(cube[i]);
	free(cube);
}
