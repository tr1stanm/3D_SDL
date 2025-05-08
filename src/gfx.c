#include "SDL.h"
#include "3DR.h"
#include <SDL2/SDL_video.h>

SDL_Renderer *renderer;
SDL_Window *window;
short isRunning = 0;

void init(const char *title, int w, int h, short isFullscreen) {
	window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, isFullscreen);
	if(window) renderer = SDL_CreateRenderer(window, -1, 0);
	if(renderer) isRunning = 1;
}

void handleEvents() {
		SDL_Event event;
		SDL_PollEvent(&event);
		switch(event.type) {
			case SDL_QUIT:
				isRunning = 0;
				break;
			default:
				break;
		}
}

// set render draw color before using
void plotPoint(gsl_matrix *toPlot) {
	if(toPlot->size1 != 2 || toPlot->size2 != 1) {
		printf("error: cannot plot a point from a non-2x1 matrix.\n");
		return;
	}

	SDL_RenderDrawPoint(renderer, gsl_matrix_get(toPlot, 0, 0) + QUADSIZE, gsl_matrix_get(toPlot, 1, 0) + QUADSIZE);
}

void plotVector(gsl_matrix *toPlot) {
	if(toPlot->size1 != 2 || toPlot->size2 != 2) {
		printf("error: cannot plot a vector from a non-2x2 matrix.\n");
		return;
	}

	SDL_RenderDrawLine(renderer, gsl_matrix_get(toPlot, 0, 0) + QUADSIZE, gsl_matrix_get(toPlot, 1, 0) + QUADSIZE,
	       		             gsl_matrix_get(toPlot, 0, 1) + QUADSIZE, gsl_matrix_get(toPlot, 1, 1) + QUADSIZE);
}
