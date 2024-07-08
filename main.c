/*#define DEBUG*/
#include "src/debug.c"

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

#include "lib/math.c"
#include "lib/utils.c"
#include "lib/number.c"
#include "lib/entity.c"
#include "lib/sprite.c"
#include "lib/cvector.h"

#include "src/fish.c"
#include "src/tree.c"
#include "src/cloud.c"
#include "src/scene.c"
#include "src/player.c"
#include "src/fishing.c"
#include "src/texture.h"

int main() {
  #ifdef DEBUG
  debug_Setup();
  #endif
  
  srand(time(NULL));
  // -- SDL ------------------------------------------------------------------
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) error();

  SDL_Window * window = SDL_CreateWindow(
      "fishtude", 
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      GAME_WIDTH,
      GAME_HEIGHT,
      SDL_WINDOW_FULLSCREEN_DESKTOP |
      SDL_WINDOW_BORDERLESS
  );
  if (window == NULL) error();

  SDL_Renderer * renderer = SDL_CreateRenderer(
      window,
      -1,
      SDL_RENDERER_ACCELERATED 
  );
  if (renderer == NULL) error();

  SDL_RenderSetLogicalSize(renderer, GAME_WIDTH, GAME_HEIGHT);
  SDL_RenderSetIntegerScale(renderer, SDL_TRUE);
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  // -------------------------------------------------------------------------

  
  SDL_Texture * fish_texture = createTexture(renderer, (void *) &FISH_PIXEL_ARRAY, 128, 48);
  SDL_Texture * cloud_texture = createTexture(renderer, (void *) &CLOUD_PIXEL_ARRAY, 128, 32);
  SDL_Texture * passive_texture = createTexture(renderer, (void *) &PASSIVE_PIXEL_ARRAY, 70, 28);

  Sprite background = createSprite(renderer, (void *) &BACKGROUND_PIXEL_ARRAY, 0, 0, 0, 0, 160, 144);
  SDL_Rect water = { 0, 36, GAME_WIDTH, GAME_HEIGHT}; 
  
  Sprite ui_x = createSprite(renderer, (void *) &UI_X_PIXEL_ARRAY, 18, 124, 0, 0, 12, 18);
  Sprite ui_z = createSprite(renderer, (void *) &UI_Z_PIXEL_ARRAY, 2, 123, 0, 0, 14, 19);
  Sprite coin = createSprite(renderer, (void *) &COIN_PIXEL_ARRAY, 2, 2, 0, 0, 4, 6);
  Number number = { 5, 6, createTexture(renderer, (void *) &NUMBER_PIXEL_ARRAY, 50, 6) };

  Sprite cloud[8];
  createCloud(cloud, cloud_texture);

  Scene scene = { GAME };
  
  PassiveTree tree = {
    NULL,
    NULL,
    { 0, 0, GAME_WIDTH, GAME_HEIGHT },
    createSprite(renderer, (void *) &PASSIVE_POINTER_PIXEL_ARRAY, 73, 65, 0, 0, 16, 16)
  };
  createPassiveTree(renderer, &tree, passive_texture);
  
  Player player = { 
    PLAYER_DEFAULT, 
    50,
    createEntity(renderer, (void *) &PLAYER_PIXEL_ARRAY, 0, 24, 16, 16, 64, 32),
    { 0, 1, 1, 0, 0, 1 }
  };

  Fishing fishing = {
    { 14, 30 },
    { 14, 30 },
    { -8, 8, 0, 2 },
    { 0, 28, 0, 1 },
    createSprite(renderer, (void *) &FISHING_FRAME_PIXEL_ARRAY, 0, 0, 0, 0, 15, 33),
    createSprite(renderer, (void *) &FISHING_POINTER_PIXEL_ARRAY, 0, 0, 0, 0, 5, 5)
  };
 
  cvector_vector_type(Fish) fish = NULL; 
  float fish_spawn_time = 0;
  float gold_income_time = 10;

  // Game-Loop
  while (1) { 
    cap(1);

    SDL_Event event;
    const Uint8* keyboard = SDL_GetKeyboardState(NULL);

    while(SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT: goto game_free;
        case SDL_KEYDOWN:
          if (event.key.keysym.sym == SDLK_ESCAPE) goto game_free; 
          if (event.key.keysym.sym == SDLK_z) scene.current = scene.current == GAME ? SKILL_TREE : GAME;
           
          if (scene.current == GAME) {
            if (event.key.keysym.sym == SDLK_x && player.state != PLAYER_BACK) {
              player.state = player.state == PLAYER_DEFAULT ? PLAYER_MECHANIC : 
                             player.state == PLAYER_MECHANIC ? PLAYER_FISHING : 
                             player.state == PLAYER_FISHING ? PLAYER_BACK : PLAYER_DEFAULT;

              if (player.state == PLAYER_MECHANIC) setPreFishing(&fishing, &player);
              if (player.state == PLAYER_FISHING) setFishing(&fishing);
            }
          }

          if (scene.current == SKILL_TREE) {
            if (event.key.keysym.sym == SDLK_UP)    updatePassiveTreeSelect(&tree, 'U');
            if (event.key.keysym.sym == SDLK_DOWN)  updatePassiveTreeSelect(&tree, 'D');
            if (event.key.keysym.sym == SDLK_LEFT)  updatePassiveTreeSelect(&tree, 'L');
            if (event.key.keysym.sym == SDLK_RIGHT) updatePassiveTreeSelect(&tree, 'R');
            if (event.key.keysym.sym == SDLK_x)     activePassiveTreeSelect(&tree, &player);
            #ifdef DEBUG
            if (event.key.keysym.sym == SDLK_x)     debug_Draw(&player.status, &p1, &p2, &p3, &p4, &p5, &co);
            #endif
          }

        break;
      }
    }

    SDL_RenderClear(renderer);   

    if (scene.current == GAME) {
      // ------ Update
      fish_spawn_time -= 0.03333;
      gold_income_time -= 0.03333;
      updatePlayer(keyboard, &player);
      updateFishing(&fishing, &player);
      for (int i = 0; i < cvector_size(fish); i++) updateFish(&fish[i], &player, &fishing, fish, i); 
      for (int i = 0; i < 8; i++) updateCloud(&cloud[i]);
      // ------

      // ------ Gold Income
      if (gold_income_time <= 0) {
        player.gold += player.status.gold_passive_income;
        gold_income_time = 10;
      }
      // ------

      // ------ Spawner
      if (fish_spawn_time <= 0) {
        for (int i = 0; i < 5 + rrandom(1, player.status.fish_spawn_quantity); i++)
          cvector_push_back(fish, createFish(fish_texture));

        fish_spawn_time = 12 - player.status.fish_spawn_time_decrease;
      }
      // ------

      // ------ Render
      drawSprite(renderer, &background);
    
      for (int i = 0; i < 8; i++) drawSprite(renderer, &cloud[i]); 
      drawEntity(renderer, &player.entity);
      
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
      drawFishingLine(renderer, &fishing, &player);
      for (int i = 0; i < cvector_size(fish); i++) drawEntity(renderer, &fish[i].entity); 

      SDL_SetRenderDrawColor(renderer, 15, 94, 156, 40);
      SDL_RenderFillRect(renderer, &water);

      drawFishingInterface(renderer, &fishing, &player);
      // ------
    }

    if (scene.current == SKILL_TREE) {
      SDL_SetRenderDrawColor(renderer, 69, 40, 60, 255);
      SDL_RenderFillRect(renderer, &background.position);
      SDL_SetRenderDrawColor(renderer, 255,255,255,255);
      drawPassive(renderer, tree.root);
      drawSprite(renderer, &tree.pointer);
    }

    drawSprite(renderer, &ui_x);
    drawSprite(renderer, &ui_z);
    drawSprite(renderer, &coin);
    drawNumber(renderer, &number, player.gold, 7, 2);
    
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderPresent(renderer); 

    cap(0);
  }

  // Free
  game_free:

  destroyTexture(player.entity.texture);

  destroyTexture(ui_x.texture);
  destroyTexture(ui_z.texture);
  destroyTexture(coin.texture);
  destroyTexture(number.texture);
  destroyTexture(background.texture);
  
  destroyTexture(fishing.pointer.texture);
  destroyTexture(fishing.frame.texture);
  destroyTexture(tree.pointer.texture);

  destroyTexture(fish_texture);
  destroyTexture(cloud_texture);
  destroyTexture(passive_texture);
  
  NodeFree(tree.root);
  cvector_free(fish);

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  #ifdef DEBUG
  debug_Destroy();
  #endif

  SDL_Quit();

  return 1;
}
