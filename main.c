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

#include "animation.c"
#include "sprite.c"
#include "entity.c"
#include "utils.c"
#include "math.c"

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

  // Keep size and scale
  SDL_RenderSetLogicalSize(renderer, WINDOW_WIDTH, WINDOW_HEIGHT);
  SDL_RenderSetIntegerScale(renderer, SDL_TRUE);

  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  
  SDL_Rect water = { 0, 36, WINDOW_WIDTH, WINDOW_HEIGHT};

  Sprite background = createSprite(renderer, "./sprites/background.bmp", 0, 0);

  Sprite fishing_mechanic = createSprite(renderer, "./sprites/fishing_mechanic.bmp", 0, 0); 
  Sprite fishing_pointer = createSprite(renderer, "./sprites/fishing_pointer.bmp", 0, 0);
  Sprite fishing_hook = createSprite(renderer, "./sprites/fishing_hook.bmp", 0, 0);

  short int fishing_pointer_dir = 1;
  short int fishing_force = 0;

  SDL_Point fishing_start;
  SDL_Point fishing_end;

  short int player_state = PLAYER_NORMAL;
  Entity player = createEntity(renderer, "./sprites/player.bmp", 0, 24, 16, 16); 

  // Game-Loop
  while (1) { 
    framerate(1);
    
    SDL_Event event;
    const Uint8* state = SDL_GetKeyboardState(NULL);

    while(SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT: goto game_free;
        case SDL_KEYDOWN: 
          if (event.key.keysym.sym == SDLK_ESCAPE) goto game_free; 
          if (event.key.keysym.sym == SDLK_x) {
            player_state = player_state == PLAYER_NORMAL ? PLAYER_PRE_FISHING : player_state == PLAYER_PRE_FISHING ? PLAYER_FISHING : PLAYER_NORMAL;
            
            if (player_state == PLAYER_PRE_FISHING) {
              fishing_force = 0;

              fishing_mechanic.position.x = player.position.x + 20;
              fishing_mechanic.position.y = player.position.y - 10;
              fishing_pointer.position.x = player.position.x + 15;
              fishing_pointer.position.y = player.position.y + 18;
              
              fishing_start.x = player.position.x + 14;
              fishing_start.y = player.position.y + 6;
              fishing_end = fishing_start;
              fishing_hook.position.x = fishing_end.x - 2;
            }

            if (player_state == PLAYER_FISHING)
              fishing_force = floorf(((float)fishing_force / 28) * 108) + 2;

          }
        break;
      }
    }

    if (state[SDL_SCANCODE_LEFT] && player_state == PLAYER_NORMAL) { player.velocity.x -= 1; }
    if (state[SDL_SCANCODE_RIGHT] && player_state == PLAYER_NORMAL) { player.velocity.x += 1; } 

    if (player.velocity.x == 0) setAnimation(&player.animation, 1, 4, 4);
    else setAnimation(&player.animation, 2, 2, 4);

    updateEntity(&player);
    player.position.x = CLAMP(player.position.x, 0, 144);
    
    if (player_state == PLAYER_PRE_FISHING) { 
      fishing_pointer.position.y = player.position.y + 18 - fishing_force;
      
      fishing_force += fishing_pointer_dir;
      if (fishing_pointer_dir == 1 && fishing_force >= 28) fishing_pointer_dir = -1;
      if (fishing_pointer_dir == -1 && fishing_force <= 0) fishing_pointer_dir = 1;
    }


    if (player_state == PLAYER_FISHING) {
      fishing_end.y = fishing_end.y + 2;
      fishing_hook.position.y = fishing_end.y;
      if (fishing_end.y >= fishing_start.y + fishing_force) player_state = PLAYER_FISHING_BACK;
    }


    if (player_state == PLAYER_FISHING_BACK) {
      fishing_end.y = fishing_end.y - 2;
      fishing_hook.position.y = fishing_end.y;
      if (fishing_end.y <= fishing_start.y) player_state = PLAYER_NORMAL;
    }

    // Render
    SDL_RenderClear(renderer);   

    drawSprite(renderer, &background);
    drawEntity(renderer, &player);
    
    if (player_state == PLAYER_FISHING || player_state == PLAYER_FISHING_BACK) {
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
      SDL_RenderDrawLine(renderer, fishing_start.x, fishing_start.y, fishing_end.x, fishing_end.y);
      drawSprite(renderer, &fishing_hook);
    }

    SDL_SetRenderDrawColor(renderer, 15, 94, 156, 100);
    SDL_RenderFillRect(renderer, &water);
    
    if (player_state == PLAYER_PRE_FISHING) {
      drawSprite(renderer, &fishing_mechanic);
      drawSprite(renderer, &fishing_pointer);
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderPresent(renderer);

    framerate(0);
  }

  // Free
  game_free:

  destroyEntity(&player);

  destroySprite(&fishing_hook);
  destroySprite(&fishing_mechanic);
  destroySprite(&fishing_pointer);
  destroySprite(&background);

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  
  SDL_Quit();

  return 1;
}
