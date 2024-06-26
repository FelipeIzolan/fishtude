#pragma once

#include <SDL_timer.h>
#include <SDL_render.h>
#include <SDL_surface.h>

// create-texture-by-file
/*SDL_Texture * createTexture(SDL_Renderer * renderer, char * src) {*/
/*  SDL_Surface * surface = SDL_LoadBMP(src);*/
/*  SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);*/

/*  SDL_FreeSurface(surface);*/

/*  return texture;*/
/*}*/

// create-texture-by-pixels-raw-data
SDL_Texture * createTexture(SDL_Renderer * renderer, void * data, int w, int h) {
  SDL_Texture * texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, w, h);
  
  SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
  SDL_UpdateTexture(texture, NULL, data, w * 4); // RGBA == 4 bytes
                                                 
  return texture;
}

void destroyTexture(SDL_Texture * texture) {
  SDL_DestroyTexture(texture);
  texture = NULL;
}

// --------------------

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
