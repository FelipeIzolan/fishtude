#define DEBUG

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

#include "lib/animation.c"
#include "lib/cvector.h"
#include "lib/sprite.c"
#include "lib/entity.c"
#include "lib/utils.c"
#include "lib/math.c"

#include "fish.c"
#include "cloud.c"
#include "player.c"
#include "fishing.c"

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
  
  SDL_Rect water = { 0, 36, WINDOW_WIDTH, WINDOW_HEIGHT};
  
  Sprite ui_x = createSprite(renderer, "./sprites/ui_x.bmp", 2, 124, 0, 0);
  Sprite background = createSprite(renderer, "./sprites/background.bmp", 0, 0, 0, 0);

  SDL_Texture * cloud_texture = createTexture(renderer, "./sprites/clouds.bmp");
  SDL_Texture * fish_texture = createTexture(renderer, "./sprites/fish.bmp");
  
  Fishing fishing = {
    { 14, 30 },
    { 14, 30 },
    { -10, 10, 0, 2 },
    { 0, 28, 0, 1 },
    createSprite(renderer, "./sprites/fishing_mechanic.bmp", 0, 0, 0, 0),
    createSprite(renderer, "./sprites/fishing_pointer.bmp", 0, 0, 0, 0)
  };

  Sprite cloud[8];
  createCloud(cloud, cloud_texture);

  Entity eplayer = createEntity(renderer, "./sprites/player.bmp", 0, 24, 16, 16);
  Player dplayer = { PLAYER_NORMAL, 0 };
  
  cvector_vector_type(Entity) efish = NULL;
  cvector_vector_type(Fish) dfish = NULL;
  cvector_push_back(efish, createEntityTexture(fish_texture, -16, 48, 8, 8));
  cvector_push_back(dfish, (Fish){ 0 });
  setFrameAnimation(&efish[0].animation, 1, 1);

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
          if (event.key.keysym.sym == SDLK_x) {
            dplayer.state = dplayer.state == PLAYER_NORMAL ? PLAYER_PRE_FISHING : dplayer.state == PLAYER_PRE_FISHING ? PLAYER_FISHING : PLAYER_NORMAL;
            
            if (dplayer.state == PLAYER_PRE_FISHING) setPreFishing(&fishing, &eplayer);
            if (dplayer.state == PLAYER_FISHING) setFishing(&fishing);
          }
        break;
      }
    }

    updatePlayer(keyboard, &eplayer, &dplayer);
    updateFishing(&fishing, &eplayer, &dplayer);
    for (int i = 0; i < cvector_size(efish); i++) updateFish(&efish[i], &dfish[i], &eplayer, &fishing, efish, dfish, i); 
    for (int i = 0; i < 8; i++) updateCloud(&cloud[i]);

    // ------ Render
    SDL_RenderClear(renderer);   

    drawSprite(renderer, &background);
    
    for (int i = 0; i < 8; i++) drawSprite(renderer, &cloud[i]); 
    drawEntity(renderer, &eplayer);
    for (int i = 0; i < cvector_size(efish); i++) drawEntity(renderer, &efish[i]); 
    drawFishingLine(renderer, &fishing, &dplayer);

    SDL_SetRenderDrawColor(renderer, 15, 94, 156, 100);
    SDL_RenderFillRect(renderer, &water);

    drawFishingInterface(renderer, &fishing, &dplayer);
    drawSprite(renderer, &ui_x);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderPresent(renderer);
    // ------

    framerate(0);
  }

  // Free
  game_free:

  destroyEntity(&eplayer);

  destroySprite(&background);
  destroySprite(&ui_x);

  destroySprite(&fishing.frame);
  destroySprite(&fishing.pointer);
  
  SDL_DestroyTexture(cloud_texture);
  SDL_DestroyTexture(fish_texture);

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
 
  cvector_free(efish);
  cvector_free(dfish);

  SDL_Quit();

  return 1;
}
