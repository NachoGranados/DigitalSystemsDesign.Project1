#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

static const int width = 800;
static const int height = 600;


int main (int argc, char **argv) {

	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window *Window = SDL_CreateWindow("Hello, SDL2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			                              width, height, SDL_WINDOW_OPENGL);

	SDL_Renderer *renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

	SDL_Surface *image = IMG_Load("Garwor.png");
	SDL_Texture *image_texture = SDL_CreateTextureFromSurface(renderer, image);
	SDL_FreeSurface(image);

	SDL_Rect texture_destination;
	texture_destination.x = 100;
	texture_destination.y = 50;
	texture_destination.w = 255;
	texture_destination.h = 255;

	bool running = true;

	SDL_Event event;

	while(running) {

		while(SDL_PollEvent(&event)) {

			if(event.type == SDL_QUIT) {

				running = false;

			}

		}

		SDL_RenderClear(renderer);

		SDL_RenderCopy(renderer, image_texture, NULL, &texture_destination);

		SDL_RenderPresent(renderer);

	}

	SDL_DestroyTexture(image_texture);
	IMG_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(Window);
	SDL_Quit();

	return 0;

}
