#pragma once

#include <SDL_render.h>
#include <SDL_surface.h>

enum player_state { 
  PLAYER_NORMAL,
  PLAYER_PRE_FISHING,
  PLAYER_FISHING,
  PLAYER_FISHING_BACK
};

SDL_Texture * createTexture(SDL_Renderer * renderer, char * src) {
  SDL_Surface * surface = SDL_LoadBMP(src);
  SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);

  SDL_FreeSurface(surface);

  return texture;
}

// --------------------

typedef struct OscillateRange {
  int min;
  int max;
  int current;
  int dir: 4; // -4 to 4
} OscillateRange;

void updateOscillateRange(OscillateRange * or) {
  or->current += or->dir;
  if (or->current >= or->max && or->dir > 0 || or->current <= or->min && or->dir < 0) or->dir = -or->dir;
}
