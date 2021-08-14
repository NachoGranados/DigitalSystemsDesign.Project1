#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 900;

const int LASER_WIDTH = 40;
const int LASER_HEIGHT = 40;

const int CHARACTER_POSX = 95;
const int CHARACTER_POSY = 47;
const int CHARACTER_WIDTH = 80;
const int CHARACTER_HEIGHT = 80;
const int CHARACTER_SPEED = 13;    // Para el mapa 1 funciona bien

const int LASER_SPEED = 50;
const int LASER_ADJUSTMENT = 18;
const int LASER_MAX_QUANTITY = 20;

const int MAP_1_SIZE = 43;


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

    SDL_Renderer *renderer;
    SDL_Window *window;

} Game;

static Game game;

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

    if (laserArray->quantity < LASER_MAX_QUANTITY) {

        Laser laser;
        laser.texture = IMG_LoadTexture(game.renderer,"Images/Laser.png");
        laser.x = player->x + LASER_ADJUSTMENT;
        laser.y = player->y + LASER_ADJUSTMENT;
        laser.w = LASER_WIDTH;
        laser.h = LASER_HEIGHT;

        laserArray->array[laserArray->quantity] = laser;

        laserArray->quantity++;

    }

    if (laserArray->quantity >= LASER_MAX_QUANTITY) {

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

                        player->texture = IMG_LoadTexture(game.renderer,"Images/Worrior/Up.png");
                        player->y -= CHARACTER_SPEED;
                        break;

                    case SDLK_DOWN:

                        player->texture = IMG_LoadTexture(game.renderer,"Images/Worrior/Down.png");
                        player->y += CHARACTER_SPEED;
                        break;

                    case SDLK_LEFT:

                        player->texture = IMG_LoadTexture(game.renderer,"Images/Worrior/Left.png");
                        player->x -= CHARACTER_SPEED;
                        break;

                    case SDLK_RIGHT:

                        player->texture = IMG_LoadTexture(game.renderer,"Images/Worrior/Right.png");
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

            laserArray->array[i].x += LASER_SPEED;

            setPosition(laserArray->array[i].texture, laserArray->array[i].x,
                        laserArray->array[i].y, laserArray->array[i].w,
                        laserArray->array[i].h);

        }

    }

}

void createMap1(SDL_Rect *mapArray) {

    int ptr = 0;

    // Top horizontal line
    SDL_Rect rectangleTop;

    rectangleTop.x = 80;
    rectangleTop.y = 37;
    rectangleTop.w = 1039;
    rectangleTop.h = 9;

    mapArray[ptr] = rectangleTop;
    ptr++;


    //First level center horizontal line
    SDL_Rect rectangleCenter;

    rectangleCenter.x = 555;
    rectangleCenter.y = 127;
    rectangleCenter.w = 90;
    rectangleCenter.h = 9;

    mapArray[ptr] = rectangleCenter;
    ptr++;


    // First level middle horizontal lines
    for (int i = 0; i < 2; i++) {

        SDL_Rect rectangle;

        rectangle.x = 180 + i * 656;
        rectangle.y = 127;
        rectangle.w = 185;
        rectangle.h = 9;

        mapArray[ptr] = rectangle;
        ptr++;

    }


    // Second level horizontal lines
    for (int i = 0; i < 7; i++) {

        SDL_Rect rectangle;

        rectangle.x = -6 + i * 186;
        rectangle.y = 220;
        rectangle.w = 95;
        rectangle.h = 9;

        mapArray[ptr] = rectangle;
        ptr++;

    }


    // Third level side horizontal lines
    for (int i = 0; i < 2; i++) {

        SDL_Rect rectangle;

        rectangle.x = 0 + i * 1024;
        rectangle.y = 314;
        rectangle.w = 177;
        rectangle.h = 9;

        mapArray[ptr] = rectangle;
        ptr++;

    }


    // Third level center horizontal lines
    for (int i = 0; i < 2; i++) {

        SDL_Rect rectangle;

        rectangle.x = 460 + i * 185;
        rectangle.y = 314;
        rectangle.w = 95;
        rectangle.h = 9;

        mapArray[ptr] = rectangle;
        ptr++;

    }


    // Fourth level horizontal lines
    for (int i = 0; i < 2; i++) {

        SDL_Rect rectangle;

        rectangle.x = 180 + i * 750;
        rectangle.y = 408;
        rectangle.w = 95;
        rectangle.h = 9;

        mapArray[ptr] = rectangle;
        ptr++;

    }


    // Fifth level horizontal lines
    for (int i = 0; i < 2; i++) {

        SDL_Rect rectangle;

        rectangle.x = 273 + i * 560;
        rectangle.y = 502;
        rectangle.w = 95;
        rectangle.h = 9;

        mapArray[ptr] = rectangle;
        ptr++;

    }


    // Bottom horizontal line 1
    SDL_Rect rectangleBottom1;

    rectangleBottom1.x = 80;
    rectangleBottom1.y = 596;
    rectangleBottom1.w = 947;
    rectangleBottom1.h = 9;

    mapArray[ptr] = rectangleBottom1;
    ptr++;


    // Bottom horizontal line 2 ---- door
    SDL_Rect rectangleBottom2;

    rectangleBottom2.x = 1026;
    rectangleBottom2.y = 596;
    rectangleBottom2.w = 93;
    rectangleBottom2.h = 9;

    mapArray[ptr] = rectangleBottom2;
    ptr++;


    // Down horizontal line
    SDL_Rect rectangleDown;

    rectangleDown.x = 1018;
    rectangleDown.y = 685;
    rectangleDown.w = 100;
    rectangleDown.h = 9;

    mapArray[ptr] = rectangleDown;
    ptr++;


    // First level side vertical lines
    for (int i = 0; i < 2; i++) {

        SDL_Rect rectangle;

        rectangle.x = 80 + i * 1030;
        rectangle.y = 37;
        rectangle.w = 9;
        rectangle.h = 190;

        mapArray[ptr] = rectangle;
        ptr++;

    }


    // First level center vertical lines
    for (int i = 0; i < 2; i++) {

        SDL_Rect rectangle;

        rectangle.x = 455 + i * 282;
        rectangle.y = 128;
        rectangle.w = 9;
        rectangle.h = 95;

        mapArray[ptr] = rectangle;
        ptr++;

    }


    // Second level vertical lines
    for (int i = 0; i < 2; i++) {

        SDL_Rect rectangle;

        rectangle.x = 362 + i * 468;
        rectangle.y = 225;
        rectangle.w = 9;
        rectangle.h = 93;

        mapArray[ptr] = rectangle;
        ptr++;

    }

    // Left vertical line
    SDL_Rect rectangleLeftVertical;

    rectangleLeftVertical.x = 80;
    rectangleLeftVertical.y = 314;
    rectangleLeftVertical.w = 9;
    rectangleLeftVertical.h = 290;

    mapArray[ptr] = rectangleLeftVertical;
    ptr++;


    // Third level left vertical lines
    for (int i = 0; i < 2; i++) {

        SDL_Rect rectangle;

        rectangle.x = 268 + i * 188;
        rectangle.y = 320;
        rectangle.w = 9;
        rectangle.h = 92;

        mapArray[ptr] = rectangle;
        ptr++;

    }


    // Third level right vertical lines
    for (int i = 0; i < 2; i++) {

        SDL_Rect rectangle;

        rectangle.x = 736 + i * 188;
        rectangle.y = 320;
        rectangle.w = 9;
        rectangle.h = 92;

        mapArray[ptr] = rectangle;
        ptr++;

    }


    // Fourth level left vertical lines
    for (int i = 0; i < 3; i++) {

        SDL_Rect rectangle;

        rectangle.x = 174 + i * 187;
        rectangle.y = 413;
        rectangle.w = 9;
        rectangle.h = 92;

        mapArray[ptr] = rectangle;
        ptr++;

    }


    // Fourth level right vertical lines
    for (int i = 0; i < 3; i++) {

        SDL_Rect rectangle;

        rectangle.x = 644 + i * 187;
        rectangle.y = 413;
        rectangle.w = 9;
        rectangle.h = 92;

        mapArray[ptr] = rectangle;
        ptr++;

    }


    // Fifth level vertical lines
    for (int i = 0; i < 2; i++) {

        SDL_Rect rectangle;

        rectangle.x = 455 + i * 281;
        rectangle.y = 508;
        rectangle.w = 9;
        rectangle.h = 92;

        mapArray[ptr] = rectangle;
        ptr++;

    }

    // Right vertical line
    SDL_Rect rectangleRightVertical;

    rectangleRightVertical.x = 1110;
    rectangleRightVertical.y = 314;
    rectangleRightVertical.w = 9;
    rectangleRightVertical.h = 380;

    mapArray[ptr] = rectangleRightVertical;
    ptr++;


    // Sixth level vertical line
    SDL_Rect rectangleDownVertical;

    rectangleDownVertical.x = 1018;
    rectangleDownVertical.y = 604;
    rectangleDownVertical.w = 9;
    rectangleDownVertical.h = 90;

    mapArray[ptr] = rectangleDownVertical;

}

void showMap(SDL_Rect *mapArray, int arraySize) {

    for (int i = 0; i < arraySize; i++) {

        SDL_RenderFillRect(game.renderer, &mapArray[i]);

    }

}

















int main (int argc, char **argv) {

    createWindow();

	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);


    SDL_Texture *background = IMG_LoadTexture(game.renderer,"Images/Background.png");


    Character player;
    Character *player_ptr = &player;
	player.texture = IMG_LoadTexture(game.renderer,"Images/Worrior/Right.png");
	player.x = CHARACTER_POSX;
	player.y = CHARACTER_POSY;
	player.w = CHARACTER_WIDTH;
	player.h = CHARACTER_HEIGHT;


	LaserArray laserArray;
	LaserArray *laserArray_ptr = &laserArray;
	laserArray.array = (Laser*)malloc(10 * sizeof(Laser));
	laserArray.quantity = 0;


	SDL_Rect *mapArray = (SDL_Rect*)malloc(43 * sizeof(SDL_Rect));

	createMap1(mapArray);













    /*
	Character player2;
	player2.texture = IMG_LoadTexture(game.renderer,"Images/Wizard of Wor.png");
	player2.x = 300;
	player2.y = 300;
	player2.w = 100;
	player2.h = 100;
	*/



	while(1) {

        SDL_SetRenderDrawColor(game.renderer, 0, 102, 204, 0);
        SDL_RenderClear(game.renderer);





        setPosition(background, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

        setPosition(player.texture, player.x, player.y, player.w, player.h);

        //setPosition(player2.texture, player2.x, player2.y, player2.w, player2.h);

        showMap(mapArray, MAP_1_SIZE);





        inputAction(player_ptr, laserArray_ptr);

        laserAction(player_ptr, laserArray_ptr);





		SDL_RenderPresent(game.renderer);

		SDL_Delay(50);

	}

	SDL_DestroyTexture(player.texture);
	//SDL_DestroyTexture(player2.texture);

	free(laserArray.array);
	free(mapArray);

	IMG_Quit();
	SDL_DestroyRenderer(game.renderer);
	SDL_DestroyWindow(game.window);
	SDL_Quit();

	return 0;

}
