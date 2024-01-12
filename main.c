#include <SDL.h>
#include <SDL_error.h>
#include <SDL_video.h>
#include <SDL_render.h>
#include <SDL_keycode.h>
#include <SDL_events.h>

#include <stdio.h>
#include <stdlib.h>

// Game-boy screen size :)
#define WINDOW_WIDTH 160
#define WINDOW_HEIGHT 144


void error() {
  fprintf(stderr, "SDL_Init -> %s\n", SDL_GetError());
  exit(EXIT_FAILURE);
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
      SDL_RENDERER_ACCELERATED
  );
  if (renderer == NULL) error();

  SDL_Surface * surface = SDL_GetWindowSurface(
      window
  );
  if (surface == NULL) error();

  // Keep size and scale
  SDL_RenderSetLogicalSize(renderer, WINDOW_WIDTH, WINDOW_HEIGHT);
  SDL_RenderSetIntegerScale(renderer, 1);
  
  SDL_Rect background;

  background.x = 0;
  background.y = 0;
  background.w = WINDOW_WIDTH;
  background.h = WINDOW_HEIGHT;

  SDL_Rect player;

  player.x = 32;
  player.y = 32;
  player.w = 10;
  player.h = 10;

  // Game-Loop
  while (1) {
    SDL_Event event;
    
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT: goto game_free;
        case SDL_KEYDOWN: 
          if (event.key.keysym.sym == SDLK_ESCAPE) goto game_free;
          if (event.key.keysym.sym == SDLK_RIGHT) player.x += 1;// Inputs!
          if (event.key.keysym.sym == SDLK_LEFT) player.x -= 1;// Inputs!
        break;
      }

      SDL_RenderClear(renderer);
      
      SDL_SetRenderDrawColor(renderer, 20, 120, 80, 255);
      SDL_RenderFillRect(renderer, &background);
      SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
      SDL_RenderDrawRect(renderer, &player);

      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
      SDL_RenderPresent(renderer);
    }

  }

  // Free
  game_free:
  SDL_DestroyWindow(window);
  SDL_Quit();



  return 1;
}
