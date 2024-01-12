#include <SDL.h>
#include <SDL_error.h>
#include <SDL_video.h>
#include <SDL_render.h>
#include <SDL_keycode.h>
#include <SDL_events.h>

#include <stdio.h>
#include <stdlib.h>

void error() {
  fprintf(stderr, "SDL_Init -> %s\n", SDL_GetError());
  exit(EXIT_FAILURE);
}

int main() {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) error();

  SDL_Window * window = SDL_CreateWindow("fishtude", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 160, 144, SDL_WINDOW_FULLSCREEN_DESKTOP);
  if (window == NULL) error();
  SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == NULL) error();
  SDL_Surface * surface = SDL_GetWindowSurface(window);
  if (surface == NULL) error();

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

  // Game-Loop
  while (1) {
    SDL_Event event;
    
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT: goto game_free;
        case SDL_KEYDOWN: 
          if (event.key.keysym.sym == SDLK_ESCAPE) goto game_free;
          // Inputs!
        break;
      }

      SDL_RenderClear(renderer);
      SDL_RenderPresent(renderer);
    }

  }

  game_free:
  // Free
  SDL_DestroyWindow(window);
  SDL_Quit();



  return 1;
}
