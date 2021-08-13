#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


static const int SCREEN_WIDTH = 1000;
static const int SCREEN_HEIGHT = 700;

static const int LASER_WIDTH = 40;
static const int LASER_HEIGHT = 40;

static const int CHARACTER_SPEED = 10;

static const int LASER_SPEED = 3;

static const int MAX_LASER_QUANTITY = 10;









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

    int health;

    SDL_Texture *texture;

} Character;

typedef struct {

    int x;
    int y;
    int w;
    int h;

    SDL_Texture *texture;

} Laser;





typedef struct {

    int quantity;

    Laser *array;

} LaserArray;












void createWindow() {

    SDL_Init(SDL_INIT_VIDEO);

	game.window = SDL_CreateWindow("Wizard of Wor", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                    SCREEN_WIDTH, SCREEN_HEIGHT, 0);

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







void createLaser(Character *player, LaserArray *laserArray) {

    if (laserArray->quantity < MAX_LASER_QUANTITY) {

        Laser laser;
        laser.texture = IMG_LoadTexture(game.renderer,"Images/Laser.png");
        laser.x = player->x;
        laser.y = player->y;
        laser.w = LASER_WIDTH;
        laser.h = LASER_HEIGHT;

        laserArray->array[laserArray->quantity] = laser;

        laserArray->quantity++;

    }

    if (laserArray->quantity >= MAX_LASER_QUANTITY) {

        laserArray->quantity = 0;

    }

}







void inputAction(Character *player, LaserArray *laserArray) {

    SDL_Event event;

    while(SDL_PollEvent(&event)) {

        switch(event.type) {

            case SDL_QUIT:

                exit(0);
                break;

            case SDL_KEYDOWN:

                switch (event.key.keysym.sym) {

                    case SDLK_UP:

                        player->y -= CHARACTER_SPEED;
                        break;

                    case SDLK_DOWN:

                        player->y += CHARACTER_SPEED;
                        break;

                    case SDLK_LEFT:

                        player->x -= CHARACTER_SPEED;
                        break;

                    case SDLK_RIGHT:

                        player->x += CHARACTER_SPEED;
                        break;

                    case SDLK_SPACE:

                        createLaser(player, laserArray);
                        break;

                }

        }

    }

}














void laserAction(Character *player, LaserArray *laserArray) {

    for (int i = 0; i < laserArray->quantity; i++) {

        if (laserArray->array[i].x < SCREEN_WIDTH) {

            laserArray->array[i].x += player->x + LASER_SPEED;
            laserArray->array[i].y = player->y;

            setPosition(laserArray->array[i].texture, laserArray->array[i].x,
                        laserArray->array[i].y, laserArray->array[i].w,
                        laserArray->array[i].h);

        }

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





	LaserArray laserArray;
	laserArray.array = (Laser*)malloc(10 * sizeof(Laser));
	laserArray.quantity = 0;











	Character player2;
	player2.texture = IMG_LoadTexture(game.renderer,"Images/Wizard of Wor.png");
	player2.x = 300;
	player2.y = 300;
	player2.w = 100;
	player2.h = 100;


	while(1) {

        SDL_SetRenderDrawColor(game.renderer, 0, 255, 0, 255);
        SDL_RenderClear(game.renderer);

        inputAction(&player, &laserArray);

        laserAction(&player, &laserArray);

























        setPosition(player.texture, player.x, player.y, player.w, player.h);

        setPosition(player2.texture, player2.x, player2.y, player2.w, player2.h);




		SDL_RenderPresent(game.renderer);

		SDL_Delay(50);

	}

	SDL_DestroyTexture(player.texture);
	SDL_DestroyTexture(player2.texture);

	free(laserArray.array);

	IMG_Quit();
	SDL_DestroyRenderer(game.renderer);
	SDL_DestroyWindow(game.window);
	SDL_Quit();

	return 0;

}













