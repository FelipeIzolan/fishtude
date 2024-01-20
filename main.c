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
#include "entity.c"
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
  // Init
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
/*  SDL_SetRelativeMouseMode(SDL_TRUE);*/

  SDL_Rect background;

  background.x = 0;
  background.y = 0;
  background.w = WINDOW_WIDTH;
  background.h = WINDOW_HEIGHT;

  Entity player = createEntity(renderer, "./sprites/player.bmp", 0, 24, 16, 16);
 
  SDL_Point player_fishing_rod_start;
  player_fishing_rod_start.x = 14;
  player_fishing_rod_start.y = 30;

  // Game-Loop
  while (1) { 
    framerate(1);
    
    SDL_Event event;
    const Uint8* state = SDL_GetKeyboardState(NULL);

    while(SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT: goto game_free;
        case SDL_KEYDOWN: if (event.key.keysym.sym == SDLK_ESCAPE) goto game_free; break;        
      }
    }

    if (state[SDL_SCANCODE_LEFT]) { player.velocity.x -= 1; }
    if (state[SDL_SCANCODE_RIGHT]) { player.velocity.x += 1; }

    if (player.velocity.x == 0) setAnimation(&player.animation, 1, 4, 4);
    else setAnimation(&player.animation, 2, 2, 4);

    updateEntity(&player);

    SDL_RenderClear(renderer);
    
    SDL_SetRenderDrawColor(renderer, 20, 120, 80, 255);
    SDL_RenderFillRect(renderer, &background);

    drawEntity(renderer, &player);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderPresent(renderer);

    framerate(0);
  }

  // Free
  game_free:

  destroyEntity(&player);
/*  SDL_FreeSurface(surface);*/
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 1;
}
