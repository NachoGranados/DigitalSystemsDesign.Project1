#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

static const int WIDTH = 1000;
static const int HEIGHT = 700;

typedef struct {

    SDL_Renderer *renderer;
    SDL_Window *window;

} Game;

static Game game;

/*
typedef struct {

    int up;
    int down;
    int left;
    int right;

} Action;

static Action action;

typedef struct {

    int life;
    int score;
    int direction;

    SDL_Texture *image_texture;
    SDL_Rect texture_destination;

} Player;
*/

typedef struct {

    int x;
    int y;
    int w;
    int h;

    //int dx;
    //int dy;
    int health;

    SDL_Texture *texture;

} Character;

typedef struct {

    float x;
    float y;
    int w;
    int h;
    int active;

    SDL_Texture *texture;

} Laser;















void createWindow() {

    SDL_Init(SDL_INIT_VIDEO);

	game.window = SDL_CreateWindow("Wizard of Wor", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                    WIDTH, HEIGHT, 0);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

	game.renderer = SDL_CreateRenderer(game.window, -1, SDL_RENDERER_ACCELERATED);

}

void setPosition(SDL_Texture *texture, int x, int y, int w, int h) {

    SDL_Rect destination;

    destination.x = x;
    destination.y = y;
    destination.w = w;
    destination.h = h;

    SDL_RenderCopy(game.renderer, texture, NULL, &destination);

}

void inputAction(Character *player, Laser *laser) {

    SDL_Event event;

    while(SDL_PollEvent(&event)) {

        switch(event.type) {

            case SDL_QUIT:

                exit(0);
                break;

            case SDL_KEYDOWN:

                switch (event.key.keysym.sym) {

                    case SDLK_UP:

                        player->y -= 4;
                        break;

                    case SDLK_DOWN:

                        player->y += 4;
                        break;

                    case SDLK_LEFT:

                        player->x -= 4;
                        break;

                    case SDLK_RIGHT:

                        player->x += 4;
                        break;

                    case SDLK_SPACE:

                        laser->active = 1;

                }

        }

    }

}


void laserAction(Character *player, Laser *laser) {

    if (laser->active == 1 && laser->x < WIDTH) {

        laser->x += player->x + 1;
        laser->y = player->y;

        setPosition(laser->texture, laser->x, laser->y, laser->w, laser->h);

    }

}

















int main (int argc, char **argv) {

    createWindow();

	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);


    Character player;
	player.texture = IMG_LoadTexture(game.renderer,"Images/Worrior.png");
	player.x = 100;
	player.y = 100;
	player.w = 100;
	player.h = 100;


	Laser laser;
	laser.texture = IMG_LoadTexture(game.renderer,"Images/Laser.png");
	laser.x = 100;
	laser.y = 100;
	laser.w = 40;
	laser.h = 40;



    /*

	Player player2;
	SDL_Surface *image = IMG_Load("Wizard of Wor.png");
	player2.image_texture = SDL_CreateTextureFromSurface(game.renderer, image);
	SDL_FreeSurface(image);

	player2.texture_destination.x = 300;
	player2.texture_destination.y = 300;
	player2.texture_destination.w = 100;
	player2.texture_destination.h = 100;
	*/

	while(1) {

        SDL_SetRenderDrawColor(game.renderer, 0, 255, 0, 255);
        SDL_RenderClear(game.renderer);

        inputAction(&player, &laser);

        laserAction(&player, &laser);

























        setPosition(player.texture, player.x, player.y, player.w, player.h);

		//SDL_RenderCopy(game.renderer, player2.image_texture, NULL, &player2.texture_destination);

		SDL_RenderPresent(game.renderer);

		SDL_Delay(45);

	}

	SDL_DestroyTexture(player.texture);
	//SDL_DestroyTexture(player2.image_texture);

	IMG_Quit();
	SDL_DestroyRenderer(game.renderer);
	SDL_DestroyWindow(game.window);
	SDL_Quit();

	return 0;

}













