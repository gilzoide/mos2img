#include "mos2img.h"

int InitSDL () {
	if (SDL_Init (SDL_INIT_VIDEO) != 0) {
		fprintf(stderr, "\nUnable to initialize SDL:  %s\n", SDL_GetError());
		return 1;
	}
	if (TTF_Init() != 0) {
		fprintf(stderr, "TTF_Init: %s\n", TTF_GetError());
		return 2;
	}

	return 0;
}


void QuitSDL () {
	TTF_Quit ();
	SDL_Quit ();
}
