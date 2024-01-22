#pragma once

#include <SDL_render.h>
#include <SDL_surface.h>

enum player_state { PLAYER_NORMAL, PLAYER_PRE_FISHING, PLAYER_FISHING };

SDL_Texture * createTexture(SDL_Renderer * renderer, char * src) {
  SDL_Surface * surface = SDL_LoadBMP(src);
  SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);

  SDL_FreeSurface(surface);

  return texture;
}
