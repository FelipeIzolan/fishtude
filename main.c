/*#define DEBUG*/

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include <SDL.h>
#include <SDL_timer.h>
#include <SDL_error.h>
#include <SDL_video.h>
#include <SDL_render.h>
#include <SDL_events.h>
#include <SDL_stdinc.h>
#include <SDL_keycode.h>

#include "lib/cvector.h"
#include "lib/sprite.c"
#include "lib/entity.c"
#include "lib/utils.c"
#include "lib/math.c"

#include "src/fish.c"
#include "src/cloud.c"
#include "src/player.c"
#include "src/fishing.c"

void error() {
  fprintf(stderr, "SDL_Init -> %s\n", SDL_GetError());
  exit(EXIT_FAILURE);
}

void framerate(int start) {
  static int s;

  if (start) {
    s = SDL_GetTicks();
  } else {
    int e = SDL_GetTicks() - s;
    int dd = DELTA * 1000;
    if (e < dd) SDL_Delay(dd - e);
  }
}

int main() {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) error();

  SDL_Window * window = SDL_CreateWindow(
      "fishtude", 
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      WINDOW_WIDTH,
      WINDOW_HEIGHT,
      SDL_WINDOW_FULLSCREEN_DESKTOP |
      SDL_WINDOW_BORDERLESS
  );
  if (window == NULL) error();

  SDL_Renderer * renderer = SDL_CreateRenderer(
      window,
      -1,
      SDL_RENDERER_ACCELERATED |
      SDL_RENDERER_PRESENTVSYNC
  );
  if (renderer == NULL) error();

  srand(time(NULL));
  SDL_RenderSetLogicalSize(renderer, WINDOW_WIDTH, WINDOW_HEIGHT);
  SDL_RenderSetIntegerScale(renderer, SDL_TRUE);
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  
  SDL_Texture * cloud_texture = createTexture(renderer, "./assets/clouds.bmp");
  SDL_Texture * fish_texture = createTexture(renderer, "./assets/fish.bmp");
  
  SDL_Rect water = { 0, 36, WINDOW_WIDTH, WINDOW_HEIGHT}; 
  Sprite cloud[8];
  Sprite ui_x = createSprite(renderer, "./assets/ui_x.bmp", 2, 124, 0, 0);
  Sprite background = createSprite(renderer, "./assets/background.bmp", 0, 0, 0, 0);
  createCloud(cloud, cloud_texture);

  Player player = { PLAYER_DEFAULT, 0, createEntity(renderer, "./assets/player.bmp", 0, 24, 16, 16) };
  Fishing fishing = {
    { 14, 30 },
    { 14, 30 },
    { -10, 10, 0, 2 },
    { 0, 28, 0, 1 },
    createSprite(renderer, "./assets/fishing_mechanic.bmp", 0, 0, 0, 0),
    createSprite(renderer, "./assets/fishing_pointer.bmp", 0, 0, 0, 0)
  };
  
  cvector_vector_type(Fish) fish = NULL;
  float fish_time = 0;

  // Game-Loop
  while (1) { 
    framerate(1);

    SDL_Event event;
    const Uint8* keyboard = SDL_GetKeyboardState(NULL);

    while(SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT: goto game_free;
        case SDL_KEYDOWN: 
          if (event.key.keysym.sym == SDLK_ESCAPE) goto game_free; 
          if (event.key.keysym.sym == SDLK_x && player.state != PLAYER_BACK) {
            player.state = player.state == PLAYER_DEFAULT ? PLAYER_MECHANIC : 
                           player.state == PLAYER_MECHANIC ? PLAYER_FISHING : 
                           player.state == PLAYER_FISHING ? PLAYER_BACK : PLAYER_DEFAULT;

            if (player.state == PLAYER_MECHANIC) setPreFishing(&fishing, &player);
            if (player.state == PLAYER_FISHING) setFishing(&fishing);
          }
        break;
      }
    }

    // ------ Update
    fish_time -= DELTA;
    updatePlayer(keyboard, &player);
    updateFishing(&fishing, &player);
    for (int i = 0; i < cvector_size(fish); i++) updateFish(&fish[i], &player.entity, &fishing, fish, i); 
    for (int i = 0; i < 8; i++) updateCloud(&cloud[i]);
    // ------


    // ------ Spawner
    if (fish_time <= 0) {
      for (int i = 0; i < 10; i++)
        cvector_push_back(fish, createFish(fish_texture));

      fish_time = 6;
    }
    // ------

    // ------ Render
    SDL_RenderClear(renderer);   

    drawSprite(renderer, &background);
    
    for (int i = 0; i < 8; i++) drawSprite(renderer, &cloud[i]); 
    drawEntity(renderer, &player.entity);
    drawFishingLine(renderer, &fishing, &player);
    for (int i = 0; i < cvector_size(fish); i++) drawEntity(renderer, &fish[i].entity); 

    SDL_SetRenderDrawColor(renderer, 15, 94, 156, 75);
    SDL_RenderFillRect(renderer, &water);

    drawFishingInterface(renderer, &fishing, &player);
    drawSprite(renderer, &ui_x);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderPresent(renderer);
    // ------

    framerate(0);
  }

  // Free
  game_free:

  destroyEntity(&player.entity);

  destroySprite(&background);
  destroySprite(&ui_x);

  destroySprite(&fishing.frame);
  destroySprite(&fishing.pointer);
  
  SDL_DestroyTexture(cloud_texture);
  SDL_DestroyTexture(fish_texture);

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
 
  cvector_free(fish);

  SDL_Quit();

  return 1;
}
