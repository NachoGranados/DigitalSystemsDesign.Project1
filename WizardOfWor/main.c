// Libraries
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

// Constants
const int TITLE_SCREEN_WIDTH = 950;
const int TITLE_SCREEN_HEIGHT = 1226;

const int PRESS_ENTER_WIDTH = 512;
const int PRESS_ENTER_HEIGHT = 121;

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 900;

const int LASER_WIDTH = 36;
const int LASER_HEIGHT = 36;
const int LASER_SPEED = 50;
const int LASER_ADJUSTMENT = 13;
const int LASER_MAX_QUANTITY = 20;

const int ENTITY_POSX = 94;
const int ENTITY_POSY = 54;
const int ENTITY_WIDTH = 65;
const int ENTITY_HEIGHT = 65;
const int ENTITY_SPEED = 13;
const int ENTITY_MAX_QUANTITY = 6;
const int ENTITY_MAX_INITIAL_STEPS = 15;

const int ENEMY_SPEED = 5;

const int RADAR_ENEMY_WIDTH = 15;
const int RADAR_ENEMY_HEIGHT = 15;
const int RADAR_ENEMY_SPEED = 2;
const double RADAR_X_RANGE_DIMENSION = 2.85;
const double RADAR_Y_RANGE_DIMENSION = 2.41;
const int RADAR_X_RANGE_ADJUSTMENT = 440;
const int RADAR_Y_RANGE_ADJUSTMENT = 655;

const int MAP_1_3_SIZE = 48;
const int MAP_2_SIZE = 38;

const int MAP_MIN_X_MARGIN = 90;
const int MAP_MAX_X_MARGIN = 850;

const int MAP_MIN_Y_MARGIN = 50;
const int MAP_MAX_Y_MARGIN = 520;

const int UP = 0;
const int DOWN = 1;
const int LEFT = 2;
const int RIGHT = 3;

const int BURWOR = 1;
const int GARWOR = 2;
const int THORWOR = 3;
const int WIZARD = 4;
const int WORLUK = 5;

const int HIGH_SCORE = 600;

const int TROPHY_WIDTH = 430;
const int TROPHY_HEIGHT = 240;

// Structures definition
typedef struct {

    SDL_Renderer *renderer;
    SDL_Window *window;

} Game;

typedef struct {

    int x;
    int y;
    int w;
    int h;

    int direction;

    int health;

    int number;

    int score;

    SDL_Texture *texture;

} Entity;

typedef struct {

    int quantity;

    Entity *array;

} LaserArray;

typedef struct {

    int collision;
    int i;
    int j;

} Coordinate;

// Functions definition
/*
  This function creates and sets specific characteristics to the window
*/
void createWindow(Game *gameWindow) {

    SDL_Init(SDL_INIT_VIDEO);

	gameWindow->window = SDL_CreateWindow("Wizard of Wor", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                      SCREEN_WIDTH, SCREEN_HEIGHT, 0);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

	gameWindow->renderer = SDL_CreateRenderer(gameWindow->window, -1, SDL_RENDERER_ACCELERATED);

}

/*
  This function creates and stores the laser's structures in the give array and it assigns them
  the attributes they need
  */
void createLaser(Game *gameWindow, Entity *player, LaserArray *laserArray) {

    if (laserArray->quantity < LASER_MAX_QUANTITY) {

        Entity laser;
        laser.x = player->x + LASER_ADJUSTMENT;
        laser.y = player->y + LASER_ADJUSTMENT;
        laser.w = LASER_WIDTH;
        laser.h = LASER_HEIGHT;
        laser.direction = player->direction;
        laser.health = 1;

        // Vertical texture
        if (player->direction < 2) {

            laser.texture = IMG_LoadTexture(gameWindow->renderer,"Images/Laser/Vertical.png");

        // Horizontal texture
        } else {

            laser.texture = IMG_LoadTexture(gameWindow->renderer,"Images/Laser/Horizontal.png");

        }

        laserArray->array[laserArray->quantity] = laser;

        laserArray->quantity++;

    }

    if (laserArray->quantity >= LASER_MAX_QUANTITY) {

        laserArray->quantity = 0;

    }

}

/*
  This function checks recursively if the given entity collides up with a rectangle
*/
int checkUpCollison(Entity *entity, SDL_Rect *mapArray, int randomMapSize,
                    int width, int height, int adjustment, int entityDimension,
                    int entityFuturePosition, int i) {

    // Stop case
    if (i >= randomMapSize) {

        return 0;

    }

    // Horizontal lines
    // Rectangles farthest up from the entity
    if (mapArray[i].y + mapArray[i].h <= entityDimension &&

        // The entity is within the X range of the rectangle
        entity->x - adjustment >= mapArray[i].x &&
        entity->x - adjustment + width <= mapArray[i].x + mapArray[i].w &&

        // The entity oversteps the rectangle
        entityFuturePosition < mapArray[i].y + mapArray[i].h) {

        return 1;

    // Vertical rectangles
    // Rectangles farthest up from the entity
    } else if (mapArray[i].y + mapArray[i].h <= entityDimension &&

       // The rectangle is within the X range of the entity
        mapArray[i].x >= entity->x - adjustment &&
        mapArray[i].x + mapArray[i].w <= entity->x - adjustment + width &&

        // The entity is next to the rectangle
        entity->y - adjustment >= mapArray[i].y + mapArray[i].h &&
        entity->y - adjustment <= mapArray[i].y + mapArray[i].h + ENTITY_WIDTH &&

        // The entity oversteps the rectangle
        entityFuturePosition < mapArray[i].y + mapArray[i].h) {

        return 1;

    // Head collision
    // Rectangles farthest up from the entity
    } else if (mapArray[i].y + mapArray[i].h <= entityDimension &&

        // The head of the entity is within the X range of the rectangle
        entity->x - adjustment >= mapArray[i].x &&
        entity->x - adjustment <= mapArray[i].x + mapArray[i].w &&
        entity->x - adjustment + width >= mapArray[i].x + mapArray[i].w &&

        // The entity is next to the rectangle
        entity->y - adjustment >= mapArray[i].y + mapArray[i].h &&
        entity->y - adjustment <= mapArray[i].y + mapArray[i].h + ENTITY_WIDTH &&

        // The entity oversteps the rectangle
        entityFuturePosition < mapArray[i].y + mapArray[i].h) {

        return 1;

    // Feet collision
    // Rectangles farthest up from the entity
    } else if (mapArray[i].y + mapArray[i].h <= entityDimension &&

       // The feet of the entity is within the Y=X range of the rectangle
        entity->x - adjustment <= mapArray[i].x &&
        entity->x - adjustment + width >= mapArray[i].x &&
        entity->x - adjustment + width <= mapArray[i].x  + mapArray[i].w &&

        // The entity is next to the rectangle
        entity->y - adjustment >= mapArray[i].y + mapArray[i].h &&
        entity->y - adjustment <= mapArray[i].y + mapArray[i].h + ENTITY_WIDTH &&

        // The entity oversteps the rectangle
        entityFuturePosition < mapArray[i].y + mapArray[i].h) {

        return 1;

    } else {

        return checkUpCollison(entity, mapArray, randomMapSize, width, height, adjustment,
                               entityDimension, entityFuturePosition, i + 1);

    }

}

/*
  This function checks recursively if the given entity collides down with a rectangle
*/
int checkDownCollison(Entity *entity, SDL_Rect *mapArray, int randomMapSize,
                      int width, int height, int adjustment, int entityDimension,
                      int entityFuturePosition, int i) {

    // Stop case
    if (i >= randomMapSize) {

        return 0;

    }

    // Horizontal lines
    // Rectangles farthest down from the entity
    if (mapArray[i].y >= entityDimension &&

        // The entity is within the X range of the rectangle
        entity->x - adjustment >= mapArray[i].x &&
        entity->x - adjustment + width <= mapArray[i].x + mapArray[i].w &&

        // The entity oversteps the rectangle
        entityFuturePosition > mapArray[i].y) {

        return 1;

    // Vertical rectangles
    // Rectangles farthest down from the entity
    } else if (mapArray[i].y >= entityDimension &&

        // The rectangle is within the X range of the entity
        mapArray[i].x >= entity->x - adjustment &&
        mapArray[i].x + mapArray[i].w <= entity->x - adjustment + width &&

        // The entity is next to the rectangle
        entity->y - adjustment + height <= mapArray[i].y &&
        entity->y - adjustment + height >= mapArray[i].y - ENTITY_WIDTH &&

        // The entity oversteps the rectangle
        entityFuturePosition > mapArray[i].y) {

        return 1;

    // Head collision
    // Rectangles farthest down from the entity
    } else if (mapArray[i].y >= entityDimension &&

        // The head of the entity is within the X range of the rectangle
        entity->x - adjustment >= mapArray[i].x &&
        entity->x - adjustment <= mapArray[i].x + mapArray[i].w &&
        entity->x - adjustment + width >= mapArray[i].x + mapArray[i].w &&

        // The entity oversteps the rectangle
        entityFuturePosition > mapArray[i].y) {

        return 1;

    // Feet collision
    // Rectangles farthest down from the entity
    } else if (mapArray[i].y >= entityDimension &&

       // The feet of the entity is within the Y=X range of the rectangle
        entity->x - adjustment <= mapArray[i].x &&
        entity->x - adjustment + width >= mapArray[i].x &&
        entity->x - adjustment + width <= mapArray[i].x  + mapArray[i].w &&

        // The entity oversteps the rectangle
        entityFuturePosition > mapArray[i].y) {

        return 1;

    } else {

        return checkDownCollison(entity, mapArray, randomMapSize, width, height, adjustment,
                                 entityDimension, entityFuturePosition, i + 1);

    }

}

/*
  This function checks recursively if the given entity collides to the left with a rectangle
*/
int checkLeftCollison(Entity *entity, SDL_Rect *mapArray, int randomMapSize,
                      int width, int height, int adjustment, int entityDimension,
                      int entityFuturePosition, int i) {

    // Stop case
    if (i >= randomMapSize) {

        return 0;

    }

    // Vertical rectangles
    // Rectangles farthest left from the entity
    if (mapArray[i].x <= entityDimension &&

        // The entity is within the Y range of the rectangle
        entity->y - adjustment >= mapArray[i].y &&
        entity->y - adjustment + height<= mapArray[i].y + mapArray[i].h &&

        // The entity oversteps the rectangle
        entityFuturePosition < mapArray[i].x + mapArray[i].w) {

        return 1;

    // Horizontal rectangles
    // Rectangles farthest left from the entity
    } else if (mapArray[i].x <= entityDimension &&

        // The rectangle is within the Y range of the entity
        mapArray[i].y >= entity->y - adjustment &&
        mapArray[i].y + mapArray[i].h <= entity->y - adjustment + height &&

        // The entity is next to the rectangle
        entity->x - adjustment >= mapArray[i].x + mapArray[i].w &&
        entity->x - adjustment <= mapArray[i].x + mapArray[i].w + ENTITY_WIDTH &&

        // The entity oversteps the rectangle
        entityFuturePosition < mapArray[i].x + mapArray[i].w) {

        return 1;

    // Head collision
    // Rectangles farthest left from the entity
    } else if (mapArray[i].x <= entityDimension &&

        // The head of the entity is within the Y range of the rectangle
        entity->y - adjustment >= mapArray[i].y &&
        entity->y - adjustment <= mapArray[i].y + mapArray[i].h &&
        entity->y - adjustment + height >= mapArray[i].y + mapArray[i].h &&

        // The entity is next to the rectangle
        entity->x - adjustment >= mapArray[i].x + mapArray[i].w &&
        entity->x - adjustment <= mapArray[i].x + mapArray[i].w + ENTITY_WIDTH &&

        // The entity oversteps the rectangle
        entityFuturePosition < mapArray[i].x + mapArray[i].w) {

        return 1;

    // Feet collision
    // Rectangles farthest left from the entity
    } else if (mapArray[i].x <= entityDimension &&

       // The feet of the entity is within the Y range of the rectangle
        entity->y - adjustment <= mapArray[i].y &&
        entity->y - adjustment + height >= mapArray[i].y &&
        entity->y - adjustment + height <= mapArray[i].y  + mapArray[i].h &&

        // The entity is next to the rectangle
        entity->x - adjustment >= mapArray[i].x + mapArray[i].w &&
        entity->x - adjustment <= mapArray[i].x + mapArray[i].w + ENTITY_WIDTH &&

        // The entity oversteps the rectangle
        entityFuturePosition < mapArray[i].x + mapArray[i].w) {

        return 1;

    } else {

        return checkLeftCollison(entity, mapArray, randomMapSize, width, height, adjustment,
                                 entityDimension, entityFuturePosition, i + 1);

    }

}

/*
  This function checks recursively if the given entity collides to the right with a rectangle
*/
int checkRightCollison(Entity *entity, SDL_Rect *mapArray, int randomMapSize,
                       int width, int height, int adjustment, int entityDimension,
                       int entityFuturePosition, int i) {

    // Stop case
    if (i >= randomMapSize) {

        return 0;

    }

    // Vertical rectangles
    // Rectangles farthest right from the entity
    if (mapArray[i].x >= entityDimension &&

        // The entity is within the Y range of the rectangle
        entity->y - adjustment >= mapArray[i].y &&
        entity->y - adjustment + height <= mapArray[i].y + mapArray[i].h &&

        // The entity oversteps the rectangle
        entityFuturePosition > mapArray[i].x) {

        return 1;

    // Horizontal rectangles
    // Rectangles farthest right from the entity
    } else if (mapArray[i].x >= entityDimension &&

        // The rectangle is within the Y range of the entity
        mapArray[i].y >= entity->y - adjustment &&
        mapArray[i].y + mapArray[i].h <= entity->y - adjustment + height &&

        // The entity is next to the rectangle
        entity->x - adjustment + width >= mapArray[i].x - ENTITY_WIDTH &&
        entity->x - adjustment + width <= mapArray[i].x &&

        // The entity oversteps the rectangle
        entityFuturePosition > mapArray[i].x) {

        return 1;

    // Head collision
    // Rectangles farthest right from the entity
    } else if(mapArray[i].x >= entityDimension &&

        // The head of the entity is within the Y range of the rectangle
        entity->y - adjustment >= mapArray[i].y &&
        entity->y - adjustment <= mapArray[i].y + mapArray[i].h &&
        entity->y - adjustment + height >= mapArray[i].y + mapArray[i].h &&

        // The entity is next to the rectangle
        entity->x - adjustment + width >= mapArray[i].x - ENTITY_WIDTH &&
        entity->x - adjustment + width <= mapArray[i].x &&

        // The entity oversteps the rectangle
        entityFuturePosition > mapArray[i].x) {

        return 1;

    // Feet collision
    // Rectangles farthest right from the entity
    } else if (mapArray[i].x >= entityDimension &&

       // The feet of the entity is within the Y range of the rectangle
        entity->y - adjustment <= mapArray[i].y &&
        entity->y - adjustment + height >= mapArray[i].y &&
        entity->y - adjustment + height <= mapArray[i].y  + mapArray[i].h &&

        // The entity is next to the rectangle
        entity->x - adjustment + width >= mapArray[i].x - ENTITY_WIDTH &&
        entity->x - adjustment + width <= mapArray[i].x &&

        // The entity oversteps the rectangle
        entityFuturePosition > mapArray[i].x) {

        return 1;

    } else {

        return checkRightCollison(entity, mapArray, randomMapSize, width, height, adjustment,
                                  entityDimension, entityFuturePosition, i + 1);

    }

}

/*
  This function checks if the given entity collides with a rectangle in the respective direction
*/
int checkCollison(Entity *entity, SDL_Rect *mapArray, int randomMapSize, int width,
                  int height, int speed, int adjustment, int direction) {

    int entityDimension;

    int entityFuturePosition;

    int result;

    switch (direction) {

        //Up collision
        case 0:

            entityDimension = entity->y - adjustment;

            entityFuturePosition = entityDimension - speed;

            result = checkUpCollison(entity, mapArray, randomMapSize,width, height, adjustment,
                                     entityDimension, entityFuturePosition, 0);

            break;

        //Down collision
        case 1:

            entityDimension = entity->y - adjustment + height;

            entityFuturePosition = entityDimension + speed;

            result = checkDownCollison(entity, mapArray, randomMapSize,width, height, adjustment,
                                       entityDimension, entityFuturePosition, 0);

            break;

        //Left collision
        case 2:

            entityDimension = entity->x - adjustment;

            entityFuturePosition = entityDimension - speed;

            result = checkLeftCollison(entity, mapArray, randomMapSize,width, height, adjustment,
                                       entityDimension, entityFuturePosition, 0);

            break;

        //Right collision
        case 3:

            entityDimension = entity->x - adjustment + width;

            entityFuturePosition = entityDimension + speed;

            result = checkRightCollison(entity, mapArray, randomMapSize,width, height, adjustment,
                                        entityDimension, entityFuturePosition, 0);

            break;

    }

    return result;

}

/*
   This function checks if the given laser collides with enemy in the array.
*/
int collisionEnemy(Entity *laser, Entity *enemiesArray, int j) {

    // Stop case
    if (j == ENTITY_MAX_QUANTITY) {

        return -1;

    }

    // Checking if the enemy is alive
    if (enemiesArray[j].health == 1 &&

        // Similar X position
        laser->x >= enemiesArray[j].x && laser->x <= enemiesArray[j].x + ENTITY_WIDTH &&

        // Similar Y position
        laser->y >= enemiesArray[j].y && laser->y <= enemiesArray[j].y + ENTITY_HEIGHT) {

        return j;

    } else {

        return collisionEnemy(laser, enemiesArray, j + 1);

    }

}

/*
  This function checks if one laser collides with enemy in the array, otherwise compares another
  laser with the rest enemies in the array.
*/
Coordinate collisionLaser(Entity *enemiesArray, LaserArray *laserArray, int i, int j) {

    // Stop case
    if (i == LASER_MAX_QUANTITY) {

        Coordinate coordinate;

        coordinate.collision = 0;
        coordinate.i = i;
        coordinate.j = j;

        return coordinate;

    }

    // Checking if the laser is alive
    if (laserArray->array[i].health == 1) {

        Entity *laser = &laserArray->array[i];

        j = collisionEnemy(laser, enemiesArray, 0);

        if (j == -1) {

            return collisionLaser(enemiesArray, laserArray, i + 1, j);

        } else {

            Coordinate coordinate;

            coordinate.collision = 1;
            coordinate.i = i;
            coordinate.j = j;

            return coordinate;

        }

    } else {

        return collisionLaser(enemiesArray, laserArray, i + 1, j);

    }

}

/*
  This function checks if the player collides with any enemy in the respective direction
*/
int destroyPlayerEnemy(Entity *player, Entity *enemiesArray, int i) {

    // Stop case
    if (i == ENTITY_MAX_QUANTITY) {

        return 0;

    }

    // Checking if the enemy is alive
    if (enemiesArray[i].health == 1 &&

        // Similar X position
        player->x + LASER_ADJUSTMENT >= enemiesArray[i].x && player->x + LASER_ADJUSTMENT <= enemiesArray[i].x + ENTITY_WIDTH &&

        // Similar Y position
        player->y + LASER_ADJUSTMENT >= enemiesArray[i].y && player->y + LASER_ADJUSTMENT <= enemiesArray[i].y + ENTITY_HEIGHT) {

        return 1;

    } else {

        return destroyPlayerEnemy(player, enemiesArray, i + 1);

    }

}

/*
  This function checks if there is a collision between lasers and enemies or player and enemies depending on
  the selection given.
*/
void destroyEntity(Entity *player, Entity *enemiesArray, LaserArray *laserArray, int selection) {

    Coordinate coordinateResult;

    int intResult;

    switch(selection) {

        // Check collisions between lasers and enemies
        case 0:

            coordinateResult = collisionLaser(enemiesArray, laserArray, 0, 0);

            if (coordinateResult.collision == 1) {

                int i = coordinateResult.i;
                int j = coordinateResult.j;

                player->score += 100;

                laserArray->array[i].health = 0;
                laserArray->array[i].x = 0;
                laserArray->array[i].y = 0;

                enemiesArray[j].health = 0;
                enemiesArray[j].x = 0;
                enemiesArray[j].y = 0;

            }

            break;

        // Check collisions between player and enemies
        case 1:

            intResult = destroyPlayerEnemy(player, enemiesArray, 0);

            if (intResult == 1) {

                player->health -= 1;

            }

            break;

    }

}

/*
  This function checks if the player, enemies or laser use the side tunnels.
*/
void checkTunnels(Entity *player, Entity *enemiesArray, LaserArray *laserArray) {

    // Checking player left to right
    if (player->x <= - 60 && player->y >= 230) {

        player->x = 1190;

    // Checking player right to left
    } else if (player->x >= 1190 && player->y >= 230) {

        player->x = - 60;

    }

    // Checking enemies
    for (int i = 0; i < ENTITY_MAX_QUANTITY; i++) {

        // Left to right
        if (enemiesArray[i].x <= - 60 && enemiesArray[i].y >= 230) {

            enemiesArray[i].x = 1190;

        // Right left
        } else if (enemiesArray[i].x >= 1190 && enemiesArray[i].y >= 230) {

            enemiesArray[i].x = - 60;

        }

    }

    // Checking lasers
    for (int i = 0; i < LASER_MAX_QUANTITY; i++) {

        // Left to right
        if (laserArray->array[i].x <= - 30 && laserArray->array[i].y >= 230) {

            laserArray->array[i].x  = 1190;

        // Right left
        } else if (laserArray->array[i].x  >= 1190 && laserArray->array[i].y >= 230) {

            laserArray->array[i].x  = - 30;

        }

    }

}

/*
  This function assigns every attribute of the given texture and it shows them in the window
*/
void setPosition(Game *gameWindow, SDL_Texture *texture, int x, int y, int w, int h) {

    SDL_Rect destination;

    destination.x = x;
    destination.y = y;
    destination.w = w;
    destination.h = h;

    SDL_RenderCopy(gameWindow->renderer, texture, NULL, &destination);

}

/*
  This function checks if an enemy has a similar position relative to another enemy
*/
int overstepEnemy (Entity *enemiesArray, int i) {

    int overstep = 0;

    for(int j = 0; j < ENTITY_MAX_QUANTITY; j++) {

        // Farthest up Y collision
        if (i != j && enemiesArray[i].y <= enemiesArray[j].y &&
            enemiesArray[i].y + ENTITY_HEIGHT >= enemiesArray[j].y) {

                overstep = 1;

        // Farthest down Y collision
        } else if (i != j && enemiesArray[i].y <= enemiesArray[j].y + ENTITY_HEIGHT &&
            enemiesArray[i].y + ENTITY_HEIGHT >= enemiesArray[j].y + ENTITY_HEIGHT) {

                overstep = 1;

        // Farthest left X collision
        } else if (i != j && enemiesArray[i].x <= enemiesArray[j].x  &&
            enemiesArray[i].x + ENTITY_WIDTH >= enemiesArray[j].x) {

            overstep = 1;

        // Farthest right X collision
        } else if (i != j && enemiesArray[i].x <= enemiesArray[j].x + ENTITY_HEIGHT &&
            enemiesArray[i].x + ENTITY_HEIGHT >= enemiesArray[j].x + ENTITY_WIDTH) {

            overstep = 1;

        }

    }

    return overstep;

}

/*
  This function assigns the correct image to the given enemy based on the direction it has.
*/
void loadEnemyImage(Game *gameWindow, Entity *enemy) {

    // Switch of enemy types
    switch(enemy->number) {

        // Burwor
        case 1:

            // Switch of directions
            switch(enemy->direction) {

                // Up
                case 0:

                    enemy->texture = IMG_LoadTexture(gameWindow->renderer,"Images/Burwor/Up.png");
                    break;

                // Down
                case 1:

                    enemy->texture = IMG_LoadTexture(gameWindow->renderer,"Images/Burwor/Down.png");
                    break;

                //Left
                case 2:

                    enemy->texture = IMG_LoadTexture(gameWindow->renderer,"Images/Burwor/Left.png");
                    break;

                // Right
                case 3:

                    enemy->texture = IMG_LoadTexture(gameWindow->renderer,"Images/Burwor/Right.png");
                    break;

            }

            break;

        // Garwor
        case 2:

            // Switch of directions
            switch(enemy->direction) {

                // Up
                case 0:

                    enemy->texture = IMG_LoadTexture(gameWindow->renderer,"Images/Garwor/Up.png");
                    break;

                // Down
                case 1:

                    enemy->texture = IMG_LoadTexture(gameWindow->renderer,"Images/Garwor/Down.png");
                    break;

                //Left
                case 2:

                    enemy->texture = IMG_LoadTexture(gameWindow->renderer,"Images/Garwor/Left.png");
                    break;

                // Right
                case 3:

                    enemy->texture = IMG_LoadTexture(gameWindow->renderer,"Images/Garwor/Right.png");
                    break;

            }

            break;

        // Thorwor
        case 3:

            // Switch of directions
            switch(enemy->direction) {

                // Up
                case 0:

                    enemy->texture = IMG_LoadTexture(gameWindow->renderer,"Images/Thorwor/Up.png");
                    break;

                // Down
                case 1:

                    enemy->texture = IMG_LoadTexture(gameWindow->renderer,"Images/Thorwor/Down.png");
                    break;

                //Left
                case 2:

                    enemy->texture = IMG_LoadTexture(gameWindow->renderer,"Images/Thorwor/Left.png");
                    break;

                // Right
                case 3:

                    enemy->texture = IMG_LoadTexture(gameWindow->renderer,"Images/Thorwor/Right.png");
                    break;

            }

            break;

        // Wizard
        case 4:

            // Switch of directions
            switch(enemy->direction) {

                // Up
                case 0:

                    enemy->texture = IMG_LoadTexture(gameWindow->renderer,"Images/Wizard/Up.png");
                    break;

                // Down
                case 1:

                    enemy->texture = IMG_LoadTexture(gameWindow->renderer,"Images/Wizard/Down.png");
                    break;

                //Left
                case 2:

                    enemy->texture = IMG_LoadTexture(gameWindow->renderer,"Images/Wizard/Left.png");
                    break;

                // Right
                case 3:

                    enemy->texture = IMG_LoadTexture(gameWindow->renderer,"Images/Wizard/Right.png");
                    break;

            }

            break;

        // Worluk
        case 5:

            // Switch of directions
            switch(enemy->direction) {

                // Up
                case 0:

                    enemy->texture = IMG_LoadTexture(gameWindow->renderer,"Images/Worluk/Up.png");
                    break;

                // Down
                case 1:

                    enemy->texture = IMG_LoadTexture(gameWindow->renderer,"Images/Worluk/Down.png");
                    break;

                //Left
                case 2:

                    enemy->texture = IMG_LoadTexture(gameWindow->renderer,"Images/Worluk/Left.png");
                    break;

                // Right
                case 3:

                    enemy->texture = IMG_LoadTexture(gameWindow->renderer,"Images/Worluk/Right.png");
                    break;

            }

            break;

    }

}

/*
  This function creates the array of enemies that will appear during the game
*/
void enemyGeneration(Game *gameWindow, Entity *enemiesArray, SDL_Rect *mapArray, int randomMapSize) {

    int random_x;
    int random_y;
    int random_texture;

    int collision_up;
    int collision_down;
    int collision_left;
    int collision_right;

    int overstep;

    Entity *enemy;

    for(int i = 0; i < ENTITY_MAX_QUANTITY; i++) {

        enemiesArray[i].x = 0;
        enemiesArray[i].y = 0;

    }

    //Set X and Y position for each enemy
    for(int enemy_index = 0; enemy_index < ENTITY_MAX_QUANTITY; enemy_index++) {

        enemy = &enemiesArray[enemy_index];

        // Generates correct random number
        while(1) {

            random_x = (rand() % MAP_MAX_X_MARGIN) + MAP_MIN_X_MARGIN;
            random_y = (rand() % MAP_MAX_Y_MARGIN) + MAP_MIN_Y_MARGIN;

            enemy->x = random_x;
            enemy->y = random_y;

            collision_up = checkCollison(enemy, mapArray, randomMapSize, ENTITY_WIDTH, ENTITY_HEIGHT,
                           ENTITY_SPEED, 0, 0);

            collision_down = checkCollison(enemy, mapArray, randomMapSize, ENTITY_WIDTH, ENTITY_HEIGHT,
                             ENTITY_SPEED, 0, 1);

            collision_left = checkCollison(enemy, mapArray, randomMapSize, ENTITY_WIDTH, ENTITY_HEIGHT,
                             ENTITY_SPEED, 0, 2);

            collision_right = checkCollison(enemy, mapArray, randomMapSize, ENTITY_WIDTH, ENTITY_HEIGHT,
                              ENTITY_SPEED, 0, 3);

            overstep = overstepEnemy(enemiesArray, enemy_index);

            if (collision_up  == 0 && collision_down == 0 && collision_left == 0 && collision_right == 0 &&
                overstep == 0) {

                break;

            }

        }

        random_texture = (rand() % 5) + 1;

        switch (random_texture) {

            // Burwor
            case 1:

                enemy->texture = IMG_LoadTexture(gameWindow->renderer,"Images/Burwor/Right.png");
                enemy->number = BURWOR;

                break;

            // Garwor
            case 2:

                enemy->texture = IMG_LoadTexture(gameWindow->renderer,"Images/Garwor/Right.png");
                enemy->number = GARWOR;

                break;

            // Thorwor
            case 3:

                enemy->texture = IMG_LoadTexture(gameWindow->renderer,"Images/Thorwor/Right.png");
                enemy->number = THORWOR;

                break;

            // Wizard
            case 4:

                enemy->texture = IMG_LoadTexture(gameWindow->renderer,"Images/Wizard/Right.png");
                enemy->number = WIZARD;

                break;

            // Worluk
            case 5:

                enemy->texture = IMG_LoadTexture(gameWindow->renderer,"Images/Worluk/Right.png");
                enemy->number = WORLUK;

                break;

        }

        enemy->w = ENTITY_WIDTH;
        enemy->h = ENTITY_HEIGHT;
        enemy->direction = 3;
        enemy->health = 1;

    }

}

/*
  This function moves every single enemy through the window
*/
void enemiesMovements(Game *gameWindow, Entity *enemiesArray, Entity *player, SDL_Rect *mapArray, int randomMapSize) {

    int collision_up;
    int collision_down;
    int collision_left;
    int collision_right;

    int randomDirection;

    int movement;

    Entity *enemy;

    for(int enemy_index = 0; enemy_index<ENTITY_MAX_QUANTITY; enemy_index++) {

        enemy = &enemiesArray[enemy_index];

        collision_up = checkCollison(enemy,mapArray, randomMapSize, ENTITY_WIDTH, ENTITY_HEIGHT,
                       ENTITY_SPEED, 0, 0);

        collision_down = checkCollison(enemy,mapArray, randomMapSize, ENTITY_WIDTH, ENTITY_HEIGHT,
                         ENTITY_SPEED, 0, 1);

        collision_left = checkCollison(enemy,mapArray, randomMapSize, ENTITY_WIDTH, ENTITY_HEIGHT,
                         ENTITY_SPEED, 0, 2);

        collision_right = checkCollison(enemy,mapArray, randomMapSize, ENTITY_WIDTH, ENTITY_HEIGHT,
                          ENTITY_SPEED, 0, 3);

        // Up movement
        if (enemy->direction == 0 && collision_up == 0) {

            enemy->y -= ENEMY_SPEED;

        // Down movement
        } else if (enemy->direction == 1 && collision_down == 0) {

            enemy->y += ENEMY_SPEED;

        // Left movement
        } else if (enemy->direction == 2 && collision_left == 0) {

            enemy->x -= ENEMY_SPEED;

        // Right movement
        } else if (enemy->direction == 3 && collision_right == 0) {

            enemy->x += ENEMY_SPEED;

        // Assign new random direction
        } else {

            int running = 1;

            movement = 0;

            while(running) {

                randomDirection = rand() % 4;

                // Up movement
                if (randomDirection == 0 && randomDirection != enemy->direction &&
                    collision_up == 0 && movement == 0) {

                    running = 0;
                    movement = 1;

                }

                if (randomDirection == 1 && randomDirection != enemy->direction &&
                    collision_down == 0 && movement == 0) {

                    running = 0;
                    movement = 1;

               }

                // Left movement
                if (randomDirection == 2 && randomDirection != enemy->direction &&
                    collision_left == 0 && movement == 0) {

                    running = 0;
                    movement = 1;

               }

                // Right movement
                if (randomDirection == 3 && randomDirection != enemy->direction &&
                    collision_right == 0 && movement == 0) {

                    running = 0;
                    movement = 1;

                }

            }

            enemy->direction = randomDirection;

        }


        loadEnemyImage(gameWindow, enemy);

        /*
        if (enemy->x >= player->x - 10 || enemy->x <= player->x + 10){


        }
        if (enemy->x > player->x && collision_left == 0 && movement == 0 && enemy->x > player->x + 10){


                enemy->x-= ENEMY_SPEED;
                movement = 1;
        }

       else if (enemy->x < player->x && collision_right == 0 && movement == 0  && enemy->x < player->x - 10){

                enemy->x += ENEMY_SPEED;
                movement = 1;
        }

        if (enemy->y > player->y && collision_up == 0 && movement == 0){

                enemy->y -= ENEMY_SPEED;
                movement = 1;
        }

        else if (enemy->y < player->y && collision_down == 0 && movement == 0){

                enemy->y += ENEMY_SPEED;
                movement = 1;
        }







        //direction_set = rand() % 3;

        //printf("POSITION %d", enemiesArray[enemy_index].x);
        //printf("DIRECTION %d", direction_set);

        //printf("PLAYER X POSITION %d", player->x);
        //printf("PLAYER Y POSITION %d", player->y);

        //direction_set = (rand() % 3 ) + 0;

        //enemiesArray[enemy_index].y += ENTITY_SPEED;
        //enemiesArray[enemy_index].x += ENTITY_SPEED;

        //Must set the user coordinates in order to follow him




        while(enemiesArray[enemy_index].x != player_posx){


            if (player_posx < enemiesArray[enemy_index].x ) {

                printf("ENTRO MENOR %s", "MENOR");

                enemiesArray[enemy_index].x -= ENTITY_SPEED;

            }


            if (player_posx > enemiesArray[enemy_index].x ) {

                printf("ENTRO MAYOR %s", "MAYOR");

                enemiesArray[enemy_index].x += ENTITY_SPEED;

            }


        }
        */


    }

}

/*
  This function set a correct initial position for every enemy in the window
*/
void enemyInitialAdjustment(Entity *enemiesArray, SDL_Rect *mapArray, int randomMapSize) {

    int collision;

    Entity *enemy;

    for(int i = 0; i < ENTITY_MAX_QUANTITY; i++) {

        enemy = &enemiesArray[i];

        // Up movement
        for (int j = 0; j < ENTITY_MAX_INITIAL_STEPS; j++) {

           collision = checkCollison(enemy, mapArray, randomMapSize, ENTITY_WIDTH, ENTITY_HEIGHT,
                                     ENTITY_SPEED, 0, 0);

           if (collision != 1) {

               enemy->y -= ENTITY_SPEED;

           }

        }

        // Down movement
        for (int j = 0; j < ENTITY_MAX_INITIAL_STEPS; j++) {

           collision = checkCollison(enemy, mapArray, randomMapSize, ENTITY_WIDTH, ENTITY_HEIGHT,
                                     ENTITY_SPEED, 0, 1);

           if (collision != 1) {

               enemy->y += ENTITY_SPEED;

           }

        }


        // Left movement
        for (int j = 0; j < ENTITY_MAX_INITIAL_STEPS; j++) {

           collision = checkCollison(enemy, mapArray, randomMapSize, ENTITY_WIDTH, ENTITY_HEIGHT,
                                     ENTITY_SPEED, 0, 2);

           if (collision != 1) {

               enemy->x -= ENTITY_SPEED;

           }

        }

        // Right movement
        for (int j = 0; j < ENTITY_MAX_INITIAL_STEPS; j++) {

           collision = checkCollison(enemy, mapArray, randomMapSize, ENTITY_WIDTH, ENTITY_HEIGHT,
                                     ENTITY_SPEED, 0, 3);

           if (collision != 1) {

               enemy->x += ENTITY_SPEED;

           }

        }

    }

}

/*
  This function show every enemy in the window
*/
void showEnemy(Game *gameWindow, Entity *enemiesArray) {

    //Set position for each initial enemy
    for(int enemy_index = 0; enemy_index < ENTITY_MAX_QUANTITY; enemy_index++) {

        // Checking if the enemy is alive
        if (enemiesArray[enemy_index].health == 1) {

            setPosition(gameWindow, enemiesArray[enemy_index].texture,enemiesArray[enemy_index].x,
                        enemiesArray[enemy_index].y, ENTITY_WIDTH, ENTITY_HEIGHT);

        }

    }

}

/*
  This function execute the given instruction by the computer mouse or the keyboard respectively
*/
int inputAction(Game *gameWindow, Entity *player, LaserArray *laserArray, SDL_Rect *mapArray, int randomMapSize) {

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

                        collision = checkCollison(player, mapArray, randomMapSize, ENTITY_WIDTH,
                                                  ENTITY_HEIGHT, ENTITY_SPEED, 0, 0);

                        if (collision != 1) {

                            player->texture = IMG_LoadTexture(gameWindow->renderer,"Images/Worrior/Up.png");
                            player->y -= ENTITY_SPEED;
                            player->direction = 0;

                        }

                        break;

                    // Move player downwards
                    case SDLK_DOWN:

                        collision = checkCollison(player, mapArray, randomMapSize, ENTITY_WIDTH,
                                                  ENTITY_HEIGHT, ENTITY_SPEED, 0, 1);

                        if (collision != 1) {

                            player->texture = IMG_LoadTexture(gameWindow->renderer,"Images/Worrior/Down.png");
                            player->y += ENTITY_SPEED;
                            player->direction = 1;

                        }

                        break;

                    // Move player leftwards
                    case SDLK_LEFT:

                        collision = checkCollison(player, mapArray, randomMapSize, ENTITY_WIDTH,
                                                  ENTITY_HEIGHT, ENTITY_SPEED, 0, 2);

                        if (collision != 1) {

                            player->texture = IMG_LoadTexture(gameWindow->renderer,"Images/Worrior/Left.png");
                            player->x -= ENTITY_SPEED;
                            player->direction = 2;

                        }

                        break;

                    // Move player rightwards
                    case SDLK_RIGHT:

                       collision = checkCollison(player, mapArray, randomMapSize, ENTITY_WIDTH,
                                                 ENTITY_HEIGHT, ENTITY_SPEED, 0, 3);

                        if (collision != 1) {

                            player->texture = IMG_LoadTexture(gameWindow->renderer,"Images/Worrior/Right.png");
                            player->x += ENTITY_SPEED;
                            player->direction = 3;

                        }

                        break;

                    // Player fire
                    case SDLK_SPACE:

                        createLaser(gameWindow, player, laserArray);

                        break;

             }

        }

    }

    return running;

}

/*
  This function assigns the correct position of every laser in the given array based on its positions
*/
void laserAction(Game *gameWindow, LaserArray *laserArray, SDL_Rect *mapArray, int randomMapSize) {

    int collision;

    Entity *laser;

    for (int i = 0; i < laserArray->quantity; i++) {

        laser = &laserArray->array[i];

        collision = checkCollison(laser, mapArray, randomMapSize, ENTITY_WIDTH, ENTITY_HEIGHT,
                                   LASER_SPEED, LASER_ADJUSTMENT, laser->direction);

        if (collision == 0) {

            switch(laser->direction) {

                // Up
                case 0:

                    laser->y -= LASER_SPEED;

                    break;

                // Down
                case 1:

                    laser->y += LASER_SPEED;

                    break;

                // Left
                case 2:

                    laser->x -= LASER_SPEED;

                    break;

                // Right
                case 3:

                    laser->x += LASER_SPEED;

                    break;

            }

            // Checking if the laser is alive
            if (laser->health == 1) {

                setPosition(gameWindow, laser->texture, laser->x, laser->y, laser->w, laser->h);

            }

        } else {

            laser->health = 0;

        }

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
        rectangle.y = 222;
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


    // Bottom horizontal line
    SDL_Rect rectangleBottom1;

    rectangleBottom1.x = 80;
    rectangleBottom1.y = 596;
    rectangleBottom1.w = 1038;
    rectangleBottom1.h = 9;

    mapArray[ptr] = rectangleBottom1;
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


    // Side vertical lines
    for (int i = 0; i < 2; i++) {

        SDL_Rect rectangle;

        rectangle.x = 80 + i * 1030;
        rectangle.y = 314;
        rectangle.w = 9;
        rectangle.h = 290;

        mapArray[ptr] = rectangle;
        ptr++;

    }


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


    // Radar horizontal lines
    for (int i = 0; i < 2; i++) {

        SDL_Rect rectangle;

        rectangle.x = 459;
        rectangle.y = 661 + i * 230;
        rectangle.w = 378;
        rectangle.h = 9;

        mapArray[ptr] = rectangle;
        ptr++;

    }


    // Radar vertical lines
    for (int i = 0; i < 2; i++) {

        SDL_Rect rectangle;

        rectangle.x = 459 + i * 369;
        rectangle.y = 670;
        rectangle.w = 9;
        rectangle.h = 223;

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
        rectangle.y = 222;
        rectangle.w = 95;
        rectangle.h = 9;

        mapArray[ptr] = rectangle;
        ptr++;

    }

    // Second level central horizontal lines
    for (int i = 0; i < 2; i++) {

        SDL_Rect rectangle;

        rectangle.x = 180 + i * 656;
        rectangle.y = 222;
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


    // Bottom horizontal line
    SDL_Rect rectangleBottom1;

    rectangleBottom1.x = 80;
    rectangleBottom1.y = 596;
    rectangleBottom1.w = 1038;
    rectangleBottom1.h = 9;

    mapArray[ptr] = rectangleBottom1;
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


    // Side vertical lines
    for (int i = 0; i < 2; i++) {

        SDL_Rect rectangle;

        rectangle.x = 80 + i * 1030;
        rectangle.y = 314;
        rectangle.w = 9;
        rectangle.h = 290;

        mapArray[ptr] = rectangle;
        ptr++;

    }


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


    // Radar horizontal lines
    for (int i = 0; i < 2; i++) {

        SDL_Rect rectangle;

        rectangle.x = 459;
        rectangle.y = 661 + i * 230;
        rectangle.w = 378;
        rectangle.h = 9;

        mapArray[ptr] = rectangle;
        ptr++;

    }


    // Radar vertical lines
    for (int i = 0; i < 2; i++) {

        SDL_Rect rectangle;

        rectangle.x = 459 + i * 369;
        rectangle.y = 670;
        rectangle.w = 9;
        rectangle.h = 223;

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
        rectangle.y = 222;
        rectangle.w = 95;
        rectangle.h = 9;

        mapArray[ptr] = rectangle;
        ptr++;

    }


    // Second level horizontal lines
    for (int i = 0; i < 2; i++) {

        SDL_Rect rectangle;

        rectangle.x = 367 + i * 280;
        rectangle.y = 222;
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


    // Bottom horizontal line
    SDL_Rect rectangleBottom1;

    rectangleBottom1.x = 80;
    rectangleBottom1.y = 596;
    rectangleBottom1.w = 1038;
    rectangleBottom1.h = 9;

    mapArray[ptr] = rectangleBottom1;
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

    // Side vertical lines
    for (int i = 0; i < 2; i++) {

        SDL_Rect rectangle;

        rectangle.x = 80 + i * 1030;
        rectangle.y = 314;
        rectangle.w = 9;
        rectangle.h = 290;

        mapArray[ptr] = rectangle;
        ptr++;

    }


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


    // Radar horizontal lines
    for (int i = 0; i < 2; i++) {

        SDL_Rect rectangle;

        rectangle.x = 459;
        rectangle.y = 661 + i * 230;
        rectangle.w = 378;
        rectangle.h = 9;

        mapArray[ptr] = rectangle;
        ptr++;

    }


    // Radar vertical lines
    for (int i = 0; i < 2; i++) {

        SDL_Rect rectangle;

        rectangle.x = 459 + i * 369;
        rectangle.y = 670;
        rectangle.w = 9;
        rectangle.h = 223;

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
void showMap(Game *gameWindow, SDL_Rect *mapArray, int arraySize) {

    for (int i = 0; i < arraySize; i++) {

        SDL_RenderFillRect(gameWindow->renderer, &mapArray[i]);

    }

}

/*
  This function creates the necessary rectangles to build the enemies on the radar space
*/
void createRadarEnemies(SDL_Rect *radarEnemyArray) {

    for (int i = 0; i < ENTITY_MAX_QUANTITY; i++) {

        SDL_Rect rectangle;

        rectangle.w = RADAR_ENEMY_WIDTH;
        rectangle.h = RADAR_ENEMY_HEIGHT;

        radarEnemyArray[i] = rectangle;

    }

}

/*
  This function shows every rectangle given by the array in the radar space
*/
void updateRadarEnemies(SDL_Rect *radarEnemyArray, Entity *enemiesArray) {

    for (int i = 0; i < ENTITY_MAX_QUANTITY; i++) {

        radarEnemyArray[i].x = (enemiesArray[i].x / RADAR_X_RANGE_DIMENSION) + RADAR_X_RANGE_ADJUSTMENT;
        radarEnemyArray[i].y = (enemiesArray[i].y / RADAR_Y_RANGE_DIMENSION) + RADAR_Y_RANGE_ADJUSTMENT;

    }

}

/*
  This function shows every rectangle given by the array in the radar space
*/
void showRadarEnemies(Game *gameWindow, Entity *enemiesArray, SDL_Rect *radarEnemyArray) {

    for (int i = 0; i < ENTITY_MAX_QUANTITY; i++) {

        if (enemiesArray[i].health == 1) {

            SDL_RenderFillRect(gameWindow->renderer, &radarEnemyArray[i]);

        }

    }

}

/*
    This function checks if the player has killed all the enemies o the player has loosed his life
*/
int endGame(Entity *player) {

    if (player->score == HIGH_SCORE || player->health <= 0) {

        return 1;

    }

    return 0;

}

/*
  This function calls and executes the necessary functions to run the game
*/
int main (int argc, char **argv) {

    // Load PNG images in the game
    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

    Game gameWindow;
    Game *gameWindow_ptr = &gameWindow;

    createWindow(gameWindow_ptr);

    SDL_Texture *initBackground = IMG_LoadTexture(gameWindow.renderer,"Images/Title.png");

    SDL_Texture *pressEnter = IMG_LoadTexture(gameWindow.renderer,"Images/PressEnter.png");

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2 , 2048);

    Mix_Music *back_sound = Mix_LoadMUS("background_sound.mp3");

    Mix_PlayMusic(back_sound, -1);

    int initRunning = 1;












    //SDL_Texture *winningTexture = IMG_LoadTexture(gameWindow.renderer,"Images/Trophy.png");





    // Infinite loop
    while(initRunning) {

        SDL_RenderClear(gameWindow.renderer);

        SDL_SetRenderDrawColor(gameWindow.renderer, 0, 0, 0, 0);

        setPosition(gameWindow_ptr, initBackground, 125, -40, TITLE_SCREEN_WIDTH, TITLE_SCREEN_WIDTH);

        setPosition(gameWindow_ptr, pressEnter, 530, 779, PRESS_ENTER_WIDTH, PRESS_ENTER_HEIGHT);

        //setPosition(gameWindow_ptr, winningTexture, 0, 350, TROPHY_WIDTH, TROPHY_HEIGHT);

        SDL_Event event;

        while(SDL_PollEvent(&event)) {

            switch(event.type) {

                case SDL_QUIT:

                    initRunning = 0;

                    break;

                case SDL_KEYDOWN:

                    // Start game
                    if (event.key.keysym.sym == SDLK_RETURN) {

                            SDL_Texture *gameBackground = IMG_LoadTexture(gameWindow.renderer,"Images/Background.png");

                            SDL_Texture *gameWallpaper = IMG_LoadTexture(gameWindow.renderer,"Images/Wallpaper.png");

                            // Map creation
                            SDL_Rect *mapArray;

                            srand(time(0));

                            int randomMap = (rand() % 3) + 1;

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

                            // Player creation
                            Entity player;
                            Entity *player_ptr = &player;
                            player.texture = IMG_LoadTexture(gameWindow.renderer,"Images/Worrior/Right.png");
                            player.x = ENTITY_POSX;
                            player.y = ENTITY_POSY;
                            player.w = ENTITY_WIDTH;
                            player.h = ENTITY_HEIGHT;
                            player.direction = 3;
                            player.health = 3;
                            player.score = 0;

                            // Laser creation
                            LaserArray laserArray;
                            LaserArray *laserArray_ptr = &laserArray;
                            laserArray.array = (Entity*)malloc(10 * sizeof(Entity));
                            laserArray.quantity = 0;

                            //Enemy creation
                            Entity *enemiesArray;
                            enemiesArray = (Entity*)malloc(6 * sizeof(Entity));
                            enemyGeneration(gameWindow_ptr, enemiesArray, mapArray, randomMapSize);
                            enemyInitialAdjustment(enemiesArray, mapArray, randomMapSize);

                            // Radar enemy creation
                            SDL_Rect *radarEnemyArray;
                            radarEnemyArray = (SDL_Rect*)malloc(ENTITY_MAX_QUANTITY * sizeof(SDL_Rect));
                            createRadarEnemies(radarEnemyArray);
                            updateRadarEnemies(radarEnemyArray, enemiesArray);

                            // Score label creation
                            TTF_Init();
                            TTF_Font *scoreFont = TTF_OpenFont("arial.ttf", 15);
                            SDL_Color scoreColor = {255, 255, 0, 0};
                            char playerScore[20];

                            SDL_Surface *scoreSurface = TTF_RenderText_Solid(scoreFont, "0", scoreColor);
                            SDL_Texture *scoreTexture = SDL_CreateTextureFromSurface(gameWindow.renderer, scoreSurface);
                            SDL_FreeSurface(scoreSurface);

                            int gameRunning = 1;

                            int endGameFlag = 0;

                            // Infinite loop
                            while(gameRunning) {

                                SDL_SetRenderDrawColor(gameWindow.renderer, 0, 102, 204, 0);

                                SDL_RenderClear(gameWindow.renderer);

                                setPosition(gameWindow_ptr, gameBackground, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

                                setPosition(gameWindow_ptr, gameWallpaper, 0, 605, 400, 300);

                                setPosition(gameWindow_ptr, scoreTexture, 975, 787, 150, 75);

                                setPosition(gameWindow_ptr, player.texture, player.x, player.y, player.w, player.h);

                                enemiesMovements(gameWindow_ptr, enemiesArray, player_ptr, mapArray, randomMapSize);

                                showEnemy(gameWindow_ptr, enemiesArray);

                                updateRadarEnemies(radarEnemyArray, enemiesArray);

                                showRadarEnemies(gameWindow_ptr, enemiesArray, radarEnemyArray);

                                checkTunnels(player_ptr, enemiesArray, laserArray_ptr);

                                showMap(gameWindow_ptr, mapArray, randomMapSize);

                                gameRunning = inputAction(gameWindow_ptr, player_ptr, laserArray_ptr, mapArray, randomMapSize);

                                laserAction(gameWindow_ptr, laserArray_ptr, mapArray, randomMapSize);

                                destroyEntity(player_ptr, enemiesArray, laserArray_ptr, 0);

                                destroyEntity(player_ptr, enemiesArray, laserArray_ptr, 1);

                                // Converts int to string
                                sprintf(playerScore, "%d", player.score);
                                scoreSurface = TTF_RenderText_Solid(scoreFont, playerScore, scoreColor);
                                scoreTexture = SDL_CreateTextureFromSurface(gameWindow.renderer, scoreSurface);

                                endGameFlag = endGame(player_ptr);

                                if (endGameFlag == 1) {

                                    gameRunning = 0;

                                }

                                SDL_RenderPresent(gameWindow.renderer);

                                SDL_Delay(50);

                            }

                            /*
                            int flag = 0;

                            SDL_Texture *winningTexture;
                            winningTexture = IMG_LoadTexture(gameWindow.renderer,"Images/Trophy.png");

                            while(1) {

                                if (flag <= 1000) {

                                    setPosition(gameWindow_ptr, winningTexture, 600, 450, TROPHY_WIDTH, TROPHY_HEIGHT);

                                    flag++;

                                    SDL_Delay(50);

                                } else {

                                    break;

                                }

                            }
                            */

                            // Free memory
                            SDL_DestroyTexture(player.texture);

                            free(laserArray.array);
                            free(mapArray);
                            free(enemiesArray);
                            free(radarEnemyArray);

                    }

            }

        }

        SDL_RenderPresent(gameWindow_ptr->renderer);

    }

	IMG_Quit();

	TTF_Quit();

	SDL_DestroyRenderer(gameWindow.renderer);
	SDL_DestroyWindow(gameWindow.window);

	//Mix_FreeChunk(back_sound);

	SDL_Quit();

	return 0;

}
