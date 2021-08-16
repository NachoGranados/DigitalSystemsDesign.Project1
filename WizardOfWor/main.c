#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


// Constants
const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 900;

const int LASER_WIDTH = 40;
const int LASER_HEIGHT = 40;

const int CHARACTER_POSX = 94;
const int CHARACTER_POSY = 47;
const int CHARACTER_WIDTH = 71;
const int CHARACTER_HEIGHT = 71;
const int CHARACTER_SPEED = 13;

const int LASER_SPEED = 50;
const int LASER_ADJUSTMENT = 18;
const int LASER_MAX_QUANTITY = 20;

const int MAP_1_3_SIZE = 51;
const int MAP_2_SIZE = 41;

const int RECTANGLE_MARGIN = 1;




// Structures definition
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

    int direction; // up = 0
                   // down = 1
                   // left = 2
                   // right = 3

    int health;

    SDL_Texture *texture;

} Character;

typedef struct {

    int x;
    int y;
    int w;
    int h;

    int direction; // up = 0
                   // down = 1
                   // left = 2
                   // right = 3

    SDL_Texture *texture;

} Laser;

typedef struct {

    int quantity;

    Laser *array;

} LaserArray;


/*
  This function creates and sets specific characteristics to the window
*/
void createWindow() {

    SDL_Init(SDL_INIT_VIDEO);

	game.window = SDL_CreateWindow("Wizard of Wor", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                    SCREEN_WIDTH, SCREEN_HEIGHT, 0);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

	game.renderer = SDL_CreateRenderer(game.window, -1, SDL_RENDERER_ACCELERATED);

}

/*
  This function assigns every attribute of the given texture and it shows them in the window
*/
void setPosition(SDL_Texture *texture, int x, int y, int w, int h) {

    SDL_Rect destination;

    destination.x = x;
    destination.y = y;
    destination.w = w;
    destination.h = h;

    SDL_RenderCopy(game.renderer, texture, NULL, &destination);

}

/*
  This function creates and stores the laser's structures in the give array and it assigns them
  the attributes they need
  */
void createLaser(Character *player, LaserArray *laserArray) {

    if (laserArray->quantity < LASER_MAX_QUANTITY) {

        Laser laser;
        laser.x = player->x + LASER_ADJUSTMENT;
        laser.y = player->y + LASER_ADJUSTMENT;
        laser.w = LASER_WIDTH;
        laser.h = LASER_HEIGHT;
        laser.direction = player->direction;

        // Vertical texture
        if (player->direction < 2) {

            laser.texture = IMG_LoadTexture(game.renderer,"Images/Laser/Vertical.png");

        // Horizontal texture
        } else {

            laser.texture = IMG_LoadTexture(game.renderer,"Images/Laser/Horizontal.png");

        }

        laserArray->array[laserArray->quantity] = laser;

        laserArray->quantity++;

    }

    if (laserArray->quantity >= LASER_MAX_QUANTITY) {

        laserArray->quantity = 0;

    }

}

/*
  This function execute the given by the computer mouse or the keyboard respectively
*/
int inputAction(Character *player, LaserArray *laserArray, SDL_Rect *mapArray, int randomMapSize) {

    SDL_Event event;

    int running = 1;

    int collision;

    while(SDL_PollEvent(&event)) {

        switch(event.type) {

            case SDL_QUIT:

                running = 0;
                break;

            case SDL_KEYDOWN:

                switch (event.key.keysym.sym) {

                    // Move player upwards
                    case SDLK_UP:

                        collision = checkCollison(player, mapArray, randomMapSize, 0);

                        if (collision != 1){

                            player->texture = IMG_LoadTexture(game.renderer,"Images/Worrior/Up.png");
                            player->y -= CHARACTER_SPEED;
                            player->direction = 0;

                        }

                        break;

                    // Move player downwards
                    case SDLK_DOWN:

                        collision = checkCollison(player, mapArray, randomMapSize, 1);

                        if (collision != 1) {

                        player->texture = IMG_LoadTexture(game.renderer,"Images/Worrior/Down.png");
                        player->y += CHARACTER_SPEED;
                        player->direction = 1;

                        }

                        break;

                    // Move player leftwards
                    case SDLK_LEFT:

                        collision = checkCollison(player, mapArray, randomMapSize, 2);

                        if (collision != 1){

                        player->texture = IMG_LoadTexture(game.renderer,"Images/Worrior/Left.png");
                        player->x -= CHARACTER_SPEED;
                        player->direction = 2;

                        }
                        break;

                    // Move player rightwards
                    case SDLK_RIGHT:

                        collision = checkCollison(player, mapArray, randomMapSize, 3);

                        if (collision != 1){

                        player->texture = IMG_LoadTexture(game.renderer,"Images/Worrior/Right.png");
                        player->x += CHARACTER_SPEED;
                        player->direction = 3;

                        }
                        break;

                    // Player fire
                    case SDLK_SPACE:

                        createLaser(player, laserArray);
                        break;

             }

        }

    }

    return running;

}


int checkCollison(Character *player, SDL_Rect *mapArray, int randomMapSize, int direction) {

    int playerDimension;

    int playerFuturePosition;

    switch (direction) {

        //Up collision
        case 0:

            playerDimension = player->y;

            playerFuturePosition = playerDimension - CHARACTER_SPEED;

            for(int i = 0; i < randomMapSize; i++) {

                // Rectangles farthest up from the player
                if (mapArray[i].y + mapArray[i].h <= playerDimension &&

                    // The player is within the X range of the rectangle
                    player->x >= mapArray[i].x - RECTANGLE_MARGIN &&
                    player->x <= mapArray[i].x + mapArray[i].w + RECTANGLE_MARGIN &&

                    // The player oversteps the rectangle
                    playerFuturePosition < mapArray[i].y + mapArray[i].h) {

                    printf(" UP \nplayer.y = %d, rectangle.y = %d\n\n",
                           playerFuturePosition, mapArray[i].y + mapArray[i].h);

                    return 1;

                }

            }

            break;

        //Down collision
        case 1:

            playerDimension = player->y + player->h;

            playerFuturePosition = playerDimension + CHARACTER_SPEED;

            for(int i = 0; i < randomMapSize; i++) {

                // Rectangles farthest down from the player
                if (mapArray[i].y >= playerDimension &&

                    // The player is within the X range of the rectangle
                    player->x >= mapArray[i].x - RECTANGLE_MARGIN &&
                    player->x <= mapArray[i].x + mapArray[i].w + RECTANGLE_MARGIN &&

                    // The player oversteps the rectangle
                    playerFuturePosition > mapArray[i].y) {

                    printf("DOWN \nplayer.y = %d, rectangle.y = %d\n\n",
                           playerFuturePosition, mapArray[i].y);

                    return 1;

                }

            }

            break;

        //Left collision
        case 2:

            playerDimension = player->x;

            playerFuturePosition = playerDimension - CHARACTER_SPEED;

            for(int i = 0; i < randomMapSize; i++) {

                // Rectangles farthest left from the player
                if (mapArray[i].x <= playerDimension &&

                    // The player is within the Y range of the rectangle
                    player->y >= mapArray[i].y - RECTANGLE_MARGIN &&
                    player->y <= mapArray[i].y + mapArray[i].h + RECTANGLE_MARGIN &&

                    // The player oversteps the rectangle
                    playerFuturePosition < mapArray[i].x + mapArray[i].w) {

                    printf("LEFT \nplayer.x = %d, rectangle.x = %d\n\n",
                           playerFuturePosition, mapArray[i].x + mapArray[i].w);

                    return 1;

                }

            }

            break;

        //Right collision
        case 3:

            playerDimension = player->x + player->w;

            playerFuturePosition = playerDimension + CHARACTER_SPEED;

            for(int i = 0; i < randomMapSize; i++) {

                // Rectangles farthest right from the player
                if (mapArray[i].x >= playerDimension &&

                    // The player is within the Y range of the rectangle
                    player->y >= mapArray[i].y - RECTANGLE_MARGIN &&
                    player->y <= mapArray[i].y + mapArray[i].h + RECTANGLE_MARGIN &&

                    // The player oversteps the rectangle
                    playerFuturePosition > mapArray[i].x) {

                    printf("RIGHT \nplayer.x = %d, rectangle.x = %d\n\n",
                           playerFuturePosition, mapArray[i].x);

                    return 1;

                }

            }

            break;

    }

    return 0;

}
















/*
  This function assigns the correct position of every laser in the given array based on its positions
*/
void laserAction(Character *player, LaserArray *laserArray) {

    for (int i = 0; i < laserArray->quantity; i++) {

        // Up
        if (laserArray->array[i].direction == 0 && laserArray->array[i].y > -LASER_HEIGHT) {

            laserArray->array[i].y -= LASER_SPEED;

        // Down
        } else if (laserArray->array[i].direction == 1 && laserArray->array[i].y < SCREEN_HEIGHT)  {

            laserArray->array[i].y += LASER_SPEED;

        // Left
        } else if (laserArray->array[i].direction == 2 && laserArray->array[i].x > -LASER_WIDTH) {

            laserArray->array[i].x -= LASER_SPEED;

        // Right
        } else if (laserArray->array[i].direction == 3 && laserArray->array[i].x < SCREEN_WIDTH) {

            laserArray->array[i].x += LASER_SPEED;

        }

        setPosition(laserArray->array[i].texture, laserArray->array[i].x, laserArray->array[i].y,
                    laserArray->array[i].w, laserArray->array[i].h);

    }

}

/*
  This function creates the necessary rectangles to build the first map
*/
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

    //printf("%d \n",mapArray[0].x);
    // First level side horizontal lines
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
    ptr++;


    // Radar horizontal lines
    for (int i = 0; i < 2; i++) {

        SDL_Rect rectangle;

        rectangle.x = 463;
        rectangle.y = 673 + i * 218;
        rectangle.w = 374;
        rectangle.h = 9;

        mapArray[ptr] = rectangle;
        ptr++;

    }


    // Radar vertical lines
    for (int i = 0; i < 2; i++) {

        SDL_Rect rectangle;

        rectangle.x = 463 + i * 365;
        rectangle.y = 673;
        rectangle.w = 9;
        rectangle.h = 220;

        mapArray[ptr] = rectangle;
        ptr++;

    }


    // Score horizontal lines
    for (int i = 0; i < 2; i++) {

        SDL_Rect rectangle;

        rectangle.x = 900;
        rectangle.y = 750 + i * 112;
        rectangle.w = 300;
        rectangle.h = 38;

        mapArray[ptr] = rectangle;
        ptr++;

    }


    // Score vertical lines
    for (int i = 0; i < 2; i++) {

        SDL_Rect rectangle;

        rectangle.x = 900 + i * 262;
        rectangle.y = 750;
        rectangle.w = 38;
        rectangle.h = 150;

        mapArray[ptr] = rectangle;
        ptr++;

    }

}

/*
  This function creates the necessary rectangles to build the second map
*/
void createMap2(SDL_Rect *mapArray) {

    int ptr = 0;

    // Top horizontal line
    SDL_Rect rectangleTop;

    rectangleTop.x = 80;
    rectangleTop.y = 37;
    rectangleTop.w = 1039;
    rectangleTop.h = 9;

    mapArray[ptr] = rectangleTop;
    ptr++;


    // First level horizontal lines
    for (int i = 0; i < 2; i++) {

        SDL_Rect rectangle;

        rectangle.x = 461 + i * 188;
        rectangle.y = 127;
        rectangle.w = 92;
        rectangle.h = 9;

        mapArray[ptr] = rectangle;
        ptr++;

    }


    // Second level side horizontal lines
    for (int i = 0; i < 2; i++) {

        SDL_Rect rectangle;

        rectangle.x = -6 + i * 1116;
        rectangle.y = 220;
        rectangle.w = 95;
        rectangle.h = 9;

        mapArray[ptr] = rectangle;
        ptr++;

    }

    // Second level central horizontal lines
    for (int i = 0; i < 2; i++) {

        SDL_Rect rectangle;

        rectangle.x = 180 + i * 656;
        rectangle.y = 220;
        rectangle.w = 186;
        rectangle.h = 9;

        mapArray[ptr] = rectangle;
        ptr++;

    }


    // Third level side horizontal lines
    for (int i = 0; i < 2; i++) {

        SDL_Rect rectangle;

        rectangle.x = -6 + i * 1120;
        rectangle.y = 314;
        rectangle.w = 95;
        rectangle.h = 9;

        mapArray[ptr] = rectangle;
        ptr++;

    }


    // Third level center horizontal lines
    for (int i = 0; i < 2; i++) {

        SDL_Rect rectangle;

        rectangle.x = 367 + i * 282;
        rectangle.y = 314;
        rectangle.w = 185;
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

        rectangle.x = 273 + i * 470;
        rectangle.y = 502;
        rectangle.w = 187;
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


    // First level middle vertical lines
    for (int i = 0; i < 2; i++) {

        SDL_Rect rectangle;

        rectangle.x = 268 + i * 656;
        rectangle.y = 128;
        rectangle.w = 9;
        rectangle.h = 190;

        mapArray[ptr] = rectangle;
        ptr++;

    }


    // First level center vertical lines
    for (int i = 0; i < 2; i++) {

        SDL_Rect rectangle;

        rectangle.x = 548 + i * 95;
        rectangle.y = 128;
        rectangle.w = 9;
        rectangle.h = 95;

        mapArray[ptr] = rectangle;
        ptr++;

    }


    // Second level vertical lines
    for (int i = 0; i < 2; i++) {

        SDL_Rect rectangle;

        rectangle.x = 455 + i * 282;
        rectangle.y = 225;
        rectangle.w = 9;
        rectangle.h = 190;

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

        rectangle.x = 174 + i * 844;
        rectangle.y = 320;
        rectangle.w = 9;
        rectangle.h = 92;

        mapArray[ptr] = rectangle;
        ptr++;

    }


    // Fourth level vertical lines
    for (int i = 0; i < 2; i++) {

        SDL_Rect rectangle;

        rectangle.x = 268 + i * 655;
        rectangle.y = 413;
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
    ptr++;


    // Radar horizontal lines
    for (int i = 0; i < 2; i++) {

        SDL_Rect rectangle;

        rectangle.x = 463;
        rectangle.y = 673 + i * 218;
        rectangle.w = 374;
        rectangle.h = 9;

        mapArray[ptr] = rectangle;
        ptr++;

    }


    // Radar vertical lines
    for (int i = 0; i < 2; i++) {

        SDL_Rect rectangle;

        rectangle.x = 463 + i * 365;
        rectangle.y = 673;
        rectangle.w = 9;
        rectangle.h = 220;

        mapArray[ptr] = rectangle;
        ptr++;

    }


    // Score horizontal lines
    for (int i = 0; i < 2; i++) {

        SDL_Rect rectangle;

        rectangle.x = 900;
        rectangle.y = 750 + i * 112;
        rectangle.w = 300;
        rectangle.h = 38;

        mapArray[ptr] = rectangle;
        ptr++;

    }


    // Score vertical lines
    for (int i = 0; i < 2; i++) {

        SDL_Rect rectangle;

        rectangle.x = 900 + i * 262;
        rectangle.y = 750;
        rectangle.w = 38;
        rectangle.h = 150;

        mapArray[ptr] = rectangle;
        ptr++;

    }

}

/*
  This function creates the necessary rectangles to build the third map
*/
void createMap3(SDL_Rect *mapArray) {

    int ptr = 0;

    // Top horizontal line
    SDL_Rect rectangleTop;

    rectangleTop.x = 80;
    rectangleTop.y = 37;
    rectangleTop.w = 1039;
    rectangleTop.h = 9;

    mapArray[ptr] = rectangleTop;
    ptr++;


    // First level side horizontal lines
    for (int i = 0; i < 2; i++) {

        SDL_Rect rectangle;

        rectangle.x = 180 + i * 750;
        rectangle.y = 127;
        rectangle.w = 90;
        rectangle.h = 9;

        mapArray[ptr] = rectangle;
        ptr++;

    }


    // First level center horizontal lines
    for (int i = 0; i < 2; i++) {

        SDL_Rect rectangle;

        rectangle.x = 462 + i * 187;
        rectangle.y = 127;
        rectangle.w = 90;
        rectangle.h = 9;

        mapArray[ptr] = rectangle;
        ptr++;

    }


    // Second level side horizontal lines
    for (int i = 0; i < 2; i++) {

        SDL_Rect rectangle;

        rectangle.x = -6 + i * 1116;
        rectangle.y = 220;
        rectangle.w = 95;
        rectangle.h = 9;

        mapArray[ptr] = rectangle;
        ptr++;

    }


    // Second level horizontal lines
    for (int i = 0; i < 2; i++) {

        SDL_Rect rectangle;

        rectangle.x = 367 + i * 280;
        rectangle.y = 220;
        rectangle.w = 185;
        rectangle.h = 9;

        mapArray[ptr] = rectangle;
        ptr++;

    }


    // Third level side horizontal lines
    for (int i = 0; i < 2; i++) {

        SDL_Rect rectangle;

        rectangle.x = -6 + i * 1116;
        rectangle.y = 314;
        rectangle.w = 95;
        rectangle.h = 9;

        mapArray[ptr] = rectangle;
        ptr++;

    }


    // Third level center horizontal lines
    for (int i = 0; i < 2; i++) {

        SDL_Rect rectangle;

        rectangle.x = 365 + i * 378;
        rectangle.y = 314;
        rectangle.w = 95;
        rectangle.h = 9;

        mapArray[ptr] = rectangle;
        ptr++;

    }


    // Fourth level side horizontal lines
    for (int i = 0; i < 2; i++) {

        SDL_Rect rectangle;

        rectangle.x = 180 + i * 750;
        rectangle.y = 408;
        rectangle.w = 95;
        rectangle.h = 9;

        mapArray[ptr] = rectangle;
        ptr++;

    }


    // Fourth level center horizontal lines
    for (int i = 0; i < 2; i++) {

        SDL_Rect rectangle;

        rectangle.x = 460 + i * 188;
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

        rectangle.x = 361 + i * 470;
        rectangle.y = 37;
        rectangle.w = 9;
        rectangle.h = 95;

        mapArray[ptr] = rectangle;
        ptr++;

    }


    // Second level vertical lines
    for (int i = 0; i < 2; i++) {

        SDL_Rect rectangle;

        rectangle.x = 175 + i * 842;
        rectangle.y = 131;
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


    // Third level side vertical lines
    for (int i = 0; i < 2; i++) {

        SDL_Rect rectangle;

        rectangle.x = 267 + i * 657;
        rectangle.y = 226;
        rectangle.w = 9;
        rectangle.h = 190;

        mapArray[ptr] = rectangle;
        ptr++;

    }


    // Third level center vertical lines
    for (int i = 0; i < 2; i++) {

        SDL_Rect rectangle;

        rectangle.x = 550 + i * 93;
        rectangle.y = 226;
        rectangle.w = 9;
        rectangle.h = 92;

        mapArray[ptr] = rectangle;
        ptr++;

    }


    // Fourth level vertical lines
    for (int i = 0; i < 2; i++) {

        SDL_Rect rectangle;

        rectangle.x = 361 + i * 470;
        rectangle.y = 320;
        rectangle.w = 9;
        rectangle.h = 92;

        mapArray[ptr] = rectangle;
        ptr++;

    }


    // Fifth level side vertical lines
    for (int i = 0; i < 2; i++) {

        SDL_Rect rectangle;

        rectangle.x = 175 + i * 842;
        rectangle.y = 413;
        rectangle.w = 9;
        rectangle.h = 92;

        mapArray[ptr] = rectangle;
        ptr++;

    }


    // Fifth level center vertical lines
    for (int i = 0; i < 2; i++) {

        SDL_Rect rectangle;

        rectangle.x = 550 + i * 92;
        rectangle.y = 413;
        rectangle.w = 9;
        rectangle.h = 92;

        mapArray[ptr] = rectangle;
        ptr++;

    }


    // Sixth level vertical lines
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
    ptr++;


    // Radar horizontal lines
    for (int i = 0; i < 2; i++) {

        SDL_Rect rectangle;

        rectangle.x = 463;
        rectangle.y = 673 + i * 218;
        rectangle.w = 374;
        rectangle.h = 9;

        mapArray[ptr] = rectangle;
        ptr++;

    }


    // Radar vertical lines
    for (int i = 0; i < 2; i++) {

        SDL_Rect rectangle;

        rectangle.x = 463 + i * 365;
        rectangle.y = 673;
        rectangle.w = 9;
        rectangle.h = 220;

        mapArray[ptr] = rectangle;
        ptr++;

    }


    // Score horizontal lines
    for (int i = 0; i < 2; i++) {

        SDL_Rect rectangle;

        rectangle.x = 900;
        rectangle.y = 750 + i * 112;
        rectangle.w = 300;
        rectangle.h = 38;

        mapArray[ptr] = rectangle;
        ptr++;

    }


    // Score vertical lines
    for (int i = 0; i < 2; i++) {

        SDL_Rect rectangle;

        rectangle.x = 900 + i * 262;
        rectangle.y = 750;
        rectangle.w = 38;
        rectangle.h = 150;

        mapArray[ptr] = rectangle;
        ptr++;

    }

}

/*
  This function shows every rectangle given by the array in the window
*/
void showMap(SDL_Rect *mapArray, int arraySize) {

    for (int i = 0; i < arraySize; i++) {

        SDL_RenderFillRect(game.renderer, &mapArray[i]);

    }

}

/*
  This function calls and executes the necessary functions to run the game
*/
int main (int argc, char **argv) {

    createWindow();

	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

    //SDL_Texture *background = IMG_LoadTexture(game.renderer,"Images/Background.png");
    SDL_Texture *background = IMG_LoadTexture(game.renderer,"Images/Map1.png");

    SDL_Texture *wallpaper = IMG_LoadTexture(game.renderer,"Images/Wallpaper.png");

    // Player creation
    Character player;
    Character *player_ptr = &player;
	player.texture = IMG_LoadTexture(game.renderer,"Images/Worrior/Right.png");
	player.x = CHARACTER_POSX;
	player.y = CHARACTER_POSY;
	player.w = CHARACTER_WIDTH;
	player.h = CHARACTER_HEIGHT;
	player.direction = 3; // Cambiarlo con la posicion inicial del jugador

	// Laser creation
	LaserArray laserArray;
	LaserArray *laserArray_ptr = &laserArray;
	laserArray.array = (Laser*)malloc(10 * sizeof(Laser));
	laserArray.quantity = 0;

	// Map creation
	SDL_Rect *mapArray;

    //int randomMap = (rand() % 3) + 1;
    int randomMap = 1;

    int randomMapSize;

    // Load first map
    if (randomMap == 1) {

        mapArray = (SDL_Rect*)malloc(MAP_1_3_SIZE * sizeof(SDL_Rect));
        randomMapSize = MAP_1_3_SIZE;
        createMap1(mapArray);

    // Load second map
    } else if (randomMap == 2) {

        mapArray = (SDL_Rect*)malloc(MAP_2_SIZE * sizeof(SDL_Rect));
        randomMapSize = MAP_2_SIZE;
        createMap2(mapArray);

    // Load third map
    } else {

        mapArray = (SDL_Rect*)malloc(MAP_1_3_SIZE * sizeof(SDL_Rect));
        randomMapSize = MAP_1_3_SIZE;
        createMap3(mapArray);

    }

    int running = 1;

    // Infinite loop
	while(running) {

        SDL_SetRenderDrawColor(game.renderer, 0, 102, 204, 0);
        SDL_RenderClear(game.renderer);

        setPosition(background, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

        setPosition(wallpaper, 0, 605, 400, 300);

        setPosition(player.texture, player.x, player.y, player.w, player.h);

        showMap(mapArray, randomMapSize);

        running = inputAction(player_ptr, laserArray_ptr, mapArray, randomMapSize);

        laserAction(player_ptr, laserArray_ptr);

		SDL_RenderPresent(game.renderer);

		SDL_Delay(50);

	}

	// Free memory
	SDL_DestroyTexture(player.texture);

	free(laserArray.array);
	free(mapArray);

	IMG_Quit();
	SDL_DestroyRenderer(game.renderer);
	SDL_DestroyWindow(game.window);
	SDL_Quit();

	return 0;

}
