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
	const double CUBESIZE = QUADSIZE / 3;
	gsl_matrix **cube;
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

	cube[0] = set3DVector(p1, p2);
	cube[1] = set3DVector(p2, p3);
	cube[2] = set3DVector(p3, p4);
	cube[3] = set3DVector(p4, p1);
	cube[4] = set3DVector(p5, p6);
	cube[5] = set3DVector(p6, p7);
	cube[6] = set3DVector(p7, p8);
	cube[7] = set3DVector(p8, p5);
	cube[8] = set3DVector(p1, p5);
	cube[9] = set3DVector(p2, p6);
	cube[10] = set3DVector(p3, p7);
	cube[11] = set3DVector(p4, p8);

	return cube;
}

gsl_matrix** renderCube(gsl_matrix **cube) {
	handleEvents();

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	for(int i = 0; i < 12; ++i) cube[i] = rotateX(rotateY(rotateZ(cube[i], 1), 1), 1);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	for(int i = 0; i < 12; ++i) plotVector(orthoProject(cube[i]));
	SDL_RenderPresent(renderer);

	return cube;
}

void destroyCube(gsl_matrix **cube) {
	for(int i = 0; i < 12; ++i) gsl_matrix_free(cube[i]);
}
