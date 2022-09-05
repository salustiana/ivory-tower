#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

static inline void exit_err_sdl(const char *ctxt)
{
	fprintf(stderr, "%s: %s\n", ctxt, SDL_GetError());
	exit(1);
}

int main()
{
	SDL_Window *win;
	SDL_Renderer *rend;
	SDL_Surface *surf;
	SDL_Texture *tex;

	atexit(SDL_Quit);

	if (SDL_Init(SDL_INIT_VIDEO) != 0)
		exit_err_sdl("sdl init");

	win = SDL_CreateWindow("some win", SDL_WINDOWPOS_UNDEFINED,
			       SDL_WINDOWPOS_UNDEFINED, 834, 768,
			       SDL_WINDOW_OPENGL);
	if (win == NULL)
		exit_err_sdl("create window");

	rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	if (rend == NULL) {
		SDL_DestroyWindow(win);
		exit_err_sdl("create renderer");
	}

	surf = IMG_Load("./salustianaranja.jpg");
	if (surf == NULL) {
		SDL_DestroyRenderer(rend);
		SDL_DestroyWindow(win);
		exit_err_sdl("load image");
	}

	tex = SDL_CreateTextureFromSurface(rend, surf);
	SDL_FreeSurface(surf);
	if (tex == NULL) {
		SDL_DestroyRenderer(rend);
		SDL_DestroyWindow(win);
		exit_err_sdl("create texture");
	}

	SDL_RenderClear(rend);
	SDL_RenderCopy(rend, tex, NULL, NULL);
	SDL_RenderPresent(rend);

	SDL_Delay(3000);
	SDL_DestroyTexture(tex);
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(win);
}
