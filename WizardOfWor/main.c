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

    int up;
    int down;
    int left;
    int right;

} Game;


static Game game;


typedef struct {

    int up;
    int down;
    int left;
    int right;

} Action;


static Action action;




typedef struct {

    int x;
    int y;
    int w; // Puede ser constante
    int h; // Puede ser constante

    SDL_Texture *texture;
    //SDL_Rect destination;

} Character;









typedef struct {

    int life;
    int score;
    int direction;

    SDL_Texture *image_texture;
    SDL_Rect texture_destination;

} Player;



void createWindow() {

    SDL_Init(SDL_INIT_VIDEO);

	game.window = SDL_CreateWindow("Wizard of Wor", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                    WIDTH, HEIGHT, 0);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

	game.renderer = SDL_CreateRenderer(game.window, -1, SDL_RENDERER_ACCELERATED);

}
























//void setPosition(SDL_Texture *texture, int x, int y, int w, int h) {

void setPosition(SDL_Texture *texture, int x, int y, int w, int h) {

    SDL_Rect destination;

    destination.x = x;
    destination.y = y;
    destination.w = w;
    destination.h = h;

    //SDL_QueryTexture(texture, NULL, NULL, &player->destination->w, &player->destination->h);

    SDL_RenderCopy(game.renderer, texture, NULL, &destination);

}


void doInput(Character *player) {

    SDL_Event event;
    /*

    while(SDL_PollEvent(&event)) {

        switch(event.type) {

            case SDL_QUIT:

                exit(0);
                break;

            case SDL_KEYDOWN:

                switch (event.key.keysym.sym) {

                    case SDLK_UP:

                        player->destination->y -= 1;
                        break;

                    case SDLK_DOWN:

                        player->destination->y += 1;
                        break;

                    case SDLK_LEFT:

                        player->destination->x -= 1;
                        break;

                    case SDLK_RIGHT:

                        player->destination->x += 1;
                        break;

                }

        }

    }
    */

}

void pressKey(SDL_KeyboardEvent *event, Character *player) {

    if (event -> repeat == 0) {

            if (event -> keysym.sym == SDL_SCANCODE_UP) {

                game.up = 1;

            } else if (event -> keysym.scancode == SDL_SCANCODE_DOWN) {

                game.down = 1;

            } else if (event -> keysym.scancode == SDL_SCANCODE_LEFT) {

                game.left = 1;

            } else if (event -> keysym.scancode == SDL_SCANCODE_RIGHT) {

                game.right = 1;

            }

    }

}

void releaseKey(SDL_KeyboardEvent *event) {

    if (event -> repeat == 0) {

            if (event -> keysym.scancode == SDL_SCANCODE_UP) {

                game.up = 0;

            } else if (event -> keysym.scancode == SDL_SCANCODE_DOWN) {

                game.down = 0;

            } else if (event -> keysym.scancode == SDL_SCANCODE_LEFT) {

                game.left = 0;

            } else if (event -> keysym.scancode == SDL_SCANCODE_RIGHT) {

                game.right = 0;

            }

    }

}

/*
void moveCharacter(Character player) {

    if (game.up) {

        player.y -= 4;

    } else if (game.down) {

        player.y += 4;

    } else if (game.left) {

        player.x -= 4;

    } else if (game.right) {

        player.x += 4;

    }

}
*/



















int main (int argc, char **argv) {

    createWindow();

	//SDL_Init(SDL_INIT_VIDEO);

	//Game game;

	//game.window = SDL_CreateWindow("Wizard of Wor", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                    //width, height, SDL_WINDOW_OPENGL);

	//game.renderer = SDL_CreateRenderer(game.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	//SDL_SetRenderDrawColor(game.renderer, 0, 255, 0, 255);

	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);



	//SDL_Surface *image = IMG_Load("Worrior.png");
	//player.texture = SDL_CreateTextureFromSurface(game.renderer, image);
	//SDL_FreeSurface(image);






    Character player;

	player.texture = IMG_LoadTexture(game.renderer,"Images/Worrior.png");

	player.x = 100;
	player.y = 100;
	player.w = 100;
	player.h = 100;



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

	//Action action;


    SDL_Event event;

	while(1) {

        SDL_SetRenderDrawColor(game.renderer, 0, 255, 0, 255);
        SDL_RenderClear(game.renderer);

        //doInput(&player);

        SDL_PollEvent(&event);

        if (event.type == SDL_QUIT) {

            exit(0);
            break;

        } else if (event.type == SDL_KEYDOWN) {

            switch (event.key.keysym.sym) {

                    case SDLK_UP:

                        player.y -= 4;
                        break;

                    case SDLK_DOWN:

                        player.y += 4;
                        break;

                    case SDLK_LEFT:

                        player.x -= 4;
                        break;

                    case SDLK_RIGHT:

                        player.x += 4;
                        break;

            }

        }

        setPosition(player.texture, player.x, player.y, player.w, player.h);




        //moveCharacter(player);

        /*
        if (action.up == 1) {

            player.y -= 4;

        } else if (action.down) {

            player.y += 4;

        } else if (action.left) {

            player.x -= 4;

        } else if (action.right) {

            player.x += 4;

        }
        */

        //printf("player.x = %d\n", player.x);


        //setPosition(player.texture, player.x, player.y, player.w, player.h);














		//SDL_RenderCopy(game.renderer, player1.image_texture, NULL, &player1.texture_destination);
		//SDL_RenderCopy(game.renderer, player2.image_texture, NULL, &player2.texture_destination);











		SDL_RenderPresent(game.renderer);

		SDL_Delay(16);

	}

	SDL_DestroyTexture(player.texture);
	//SDL_DestroyTexture(player2.image_texture);

	IMG_Quit();
	SDL_DestroyRenderer(game.renderer);
	SDL_DestroyWindow(game.window);
	SDL_Quit();

	return 0;

}













