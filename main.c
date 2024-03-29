/*#define DEBUG*/
#ifdef DEBUG
#include "lib/tdb.h"
#endif

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

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
#include "src/cloud.c"
#include "src/player.c"
#include "src/fishing.c"
#include "src/passive/tree.c"

enum { GAME, SKILL_TREE };

void error() {
  fprintf(stderr, "SDL_Init -> %s\n", SDL_GetError());
  exit(EXIT_FAILURE);
}

void cap(int start) {
  static uint64_t s;
  
  if (start) {
    s = SDL_GetPerformanceCounter();
  } else {
    uint64_t e = SDL_GetPerformanceCounter();
    float el = (e - s) / (float)SDL_GetPerformanceFrequency() * 1000;
    SDL_Delay(33.33333 - el); // 30 FPS
  }
}

#ifdef DEBUG
void debugDraw(PlayerStatus * status, TDB_BitMap * p1, TDB_BitMap * p2, TDB_BitMap * p3, TDB_BitMap * p4, TDB_BitMap * p5, TDB_BitMap * coin) {
  TDB_Clear();
  TDB_Size s = TDB_GetSize();

  TDB_DrawLine("|", s.col / 2, 0, s.col / 2, s.row);
    
  TDB_DrawBitMap(p1, TDB_PIXEL, 4, 2, 2);
  TDB_Write("fish_spawn_quantity", 30, 8);
  TDB_WriteF(30, 9, "%hi", status->fish_spawn_quantity);

  TDB_DrawBitMap(p2, TDB_PIXEL, 4, 14, 2);
  TDB_Write("fish_spawn_time_decrease", 30, 20);
  TDB_WriteF(30, 21, "%f", status->fish_spawn_time_decrease);

  TDB_DrawBitMap(p3, TDB_PIXEL, 4, 26, 2);
  TDB_Write("fish_gold_base", 30, 32);
  TDB_WriteF(30, 33, "%hi", status->fish_gold_base);

  TDB_DrawBitMap(p4, TDB_PIXEL, 4, 38, 2);
  TDB_Write("fish_gold_multiplier", 30, 44);
  TDB_WriteF(30, 45, "%f", status->fish_gold_multiplier);

  TDB_DrawBitMap(p5, TDB_PIXEL, 4, 50, 2);
  TDB_SetCursor(30, 56);
  TDB_Write("gold_passive_income", 30, 56);
  TDB_WriteF(30, 57, "%hi", status->gold_passive_income);

  TDB_DrawBitMap(coin, TDB_PIXEL, (s.col / 2) + 4, 4, 2);
  TDB_WriteF((s.col / 2) + 12, 7, "price(100): %i", (int)(100 * status->passive_price_multiplier));
  TDB_WriteF((s.col / 2) + 12, 8, "price(200): %i", (int)(200 * status->passive_price_multiplier));
}

void debugDestroy(TDB_BitMap * p1, TDB_BitMap * p2, TDB_BitMap * p3, TDB_BitMap * p4, TDB_BitMap * p5, TDB_BitMap * coin) {
  TDB_DestroyBitMap(p1);
  TDB_DestroyBitMap(p2);
  TDB_DestroyBitMap(p3);
  TDB_DestroyBitMap(p4);
  TDB_DestroyBitMap(p5);
  TDB_DestroyBitMap(coin);
}
#endif

int main() {
  #ifdef DEBUG
  TDB_Setup(0);

  TDB_BitMap p1 = TDB_LoadBitMap("./assets/passives/1.bmp");
  TDB_BitMap p2 = TDB_LoadBitMap("./assets/passives/2.bmp");
  TDB_BitMap p3 = TDB_LoadBitMap("./assets/passives/3.bmp");
  TDB_BitMap p4 = TDB_LoadBitMap("./assets/passives/4.bmp");
  TDB_BitMap p5 = TDB_LoadBitMap("./assets/passives/5.bmp");
  TDB_BitMap co = TDB_LoadBitMap("./assets/coin.bmp");
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

  SDL_Texture * fish_texture = createTexture(renderer, "./assets/fish.bmp");
  SDL_Texture * cloud_texture = createTexture(renderer, "./assets/clouds.bmp");
  SDL_Texture * passive_texture = createTexture(renderer, "./assets/passives.bmp");

  int scene = GAME;

  Sprite background = createSprite(renderer, "./assets/background.bmp", 0, 0, 0, 0);
  SDL_Rect water = { 0, 36, GAME_WIDTH, GAME_HEIGHT}; 
  
  Sprite ui_x = createSprite(renderer, "./assets/ui_x.bmp", 18, 124, 0, 0);
  Sprite ui_z = createSprite(renderer, "./assets/ui_z.bmp", 2, 123, 0, 0);
  Sprite coin = createSprite(renderer, "./assets/coin.bmp", 2, 2, 4, 6);
  Number number = { 5, 6, createTexture(renderer, "./assets/number.bmp") };

  Sprite cloud[8];
  createCloud(cloud, cloud_texture);

  PassiveTree passiveTree = createPassiveTree(renderer, passive_texture);
  
  Player player = { 
    PLAYER_DEFAULT, 
    0,
    createEntity(renderer, "./assets/player.bmp", 0, 24, 16, 16),
    { 0, 1, 1, 0, 0, 1 }
  };

  Fishing fishing = {
    { 14, 30 },
    { 14, 30 },
    { -8, 8, 0, 2 },
    { 0, 28, 0, 1 },
    createSprite(renderer, "./assets/fishing_mechanic.bmp", 0, 0, 0, 0),
    createSprite(renderer, "./assets/fishing_pointer.bmp", 0, 0, 0, 0)
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
          if (event.key.keysym.sym == SDLK_z) scene = scene == GAME ? SKILL_TREE : GAME;
           
          if (scene == GAME) {
            if (event.key.keysym.sym == SDLK_x && player.state != PLAYER_BACK) {
              player.state = player.state == PLAYER_DEFAULT ? PLAYER_MECHANIC : 
                             player.state == PLAYER_MECHANIC ? PLAYER_FISHING : 
                             player.state == PLAYER_FISHING ? PLAYER_BACK : PLAYER_DEFAULT;

              if (player.state == PLAYER_MECHANIC) setPreFishing(&fishing, &player);
              if (player.state == PLAYER_FISHING) setFishing(&fishing);
            }
          }

          if (scene == SKILL_TREE) {
            if (event.key.keysym.sym == SDLK_UP)    updatePassiveTreeSelect(&passiveTree, 'U');
            if (event.key.keysym.sym == SDLK_DOWN)  updatePassiveTreeSelect(&passiveTree, 'D');
            if (event.key.keysym.sym == SDLK_LEFT)  updatePassiveTreeSelect(&passiveTree, 'L');
            if (event.key.keysym.sym == SDLK_RIGHT) updatePassiveTreeSelect(&passiveTree, 'R');
            if (event.key.keysym.sym == SDLK_x)     activePassiveTreeSelect(&passiveTree, &player);
            #ifdef DEBUG
            if (event.key.keysym.sym == SDLK_x)     debugDraw(&player.status, &p1, &p2, &p3, &p4, &p5, &co);
            #endif
          }

        break;
      }
    }

    SDL_RenderClear(renderer);   

    if (scene == GAME) {
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
        for (int i = 0; i < 7 + rrandom(1, player.status.fish_spawn_quantity); i++)
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

    if (scene == SKILL_TREE) {
      SDL_SetRenderDrawColor(renderer, 69, 40, 60, 255);
      SDL_RenderFillRect(renderer, &background.position);
      SDL_SetRenderDrawColor(renderer, 255,255,255,255);
      drawPassive(renderer, passiveTree.root);
      drawSprite(renderer, &passiveTree.pointer);
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
  destroyTexture(passiveTree.pointer.texture);

  destroyTexture(fish_texture);
  destroyTexture(cloud_texture);
  destroyTexture(passive_texture);
  
  NodeFree(passiveTree.root);
  cvector_free(fish);

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  #ifdef DEBUG
  debugDestroy(&p1, &p2, &p3, &p4, &p5, &co);
  #endif

  SDL_Quit();

  return 1;
}
