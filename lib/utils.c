#pragma once

#include <math.h>
#include <SDL_render.h>
#include <SDL_surface.h>

SDL_Texture * createTexture(SDL_Renderer * renderer, char * src) {
  SDL_Surface * surface = SDL_LoadBMP(src);
  SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);

  SDL_FreeSurface(surface);

  return texture;
}

// --------------------

typedef struct OscillateRange {
  short min;
  short max;
  short current;
  int dir: 4; // -4 to 4
} OscillateRange;

void updateOscillateRange(OscillateRange * or) {
  or->current += or->dir;
  if ((or->current >= or->max && or->dir > 0) || (or->current <= or->min && or->dir < 0)) or->dir = -or->dir;
}

// --------------------

typedef struct SineWave {
  unsigned int frequency: 4; // 0 to 16 | + == slow | - == fast
  unsigned int amplitude: 4;
  short init;
} SineWave;

short calcSineWave(SineWave * wave, short x) {
  return wave->init + wave->amplitude * sin((float) x / wave->frequency);
}
