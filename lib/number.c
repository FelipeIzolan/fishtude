#pragma once

#include "SDL_render.h"
#include "SDL_rect.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Number {
  int w;
  int h;
  SDL_Texture * texture;
} Number;

void drawNumber(SDL_Renderer * renderer, Number * number, uint v, int x, int y) {
  char * n = malloc(sizeof(char) * (int)((v == 0 ? 0 : log10(v)) + 1));
  sprintf(n, "%u", v);

  for (int i = 0; i < strlen(n); i++) {
    switch (n[i]) {
      case 48: 
        SDL_RenderCopy(renderer, 
                       number->texture, 
                       &(SDL_Rect) { 0, 0, number->w, number->h }, 
                       &(SDL_Rect) { x + i * number->w, y, number->w, number->h }
                      );
      break;
      case 49:
        SDL_RenderCopy(renderer, 
                       number->texture, 
                       &(SDL_Rect) { number->w, 0, number->w, number->h }, 
                       &(SDL_Rect) { x + i * number->w, y, number->w, number->h }
                      );
      break;
      case 50:
        SDL_RenderCopy(renderer, 
                       number->texture, 
                       &(SDL_Rect) { number->w * 2, 0, number->w, number->h }, 
                       &(SDL_Rect) { x + i * number->w, y, number->w, number->h }
                      );
      break;
      case 51:
        SDL_RenderCopy(renderer, 
                       number->texture, 
                       &(SDL_Rect) { number->w * 3, 0, number->w, number->h }, 
                       &(SDL_Rect) { x + i * number->w, y, number->w, number->h }
                      );
      break;
      case 52:
        SDL_RenderCopy(renderer, 
                       number->texture, 
                       &(SDL_Rect) { number->w * 4, 0, number->w, number->h }, 
                       &(SDL_Rect) { x + i * number->w, y, number->w, number->h }
                      );
      break;
      case 53:
        SDL_RenderCopy(renderer, 
                       number->texture, 
                       &(SDL_Rect) { number->w * 5, 0, number->w, number->h }, 
                       &(SDL_Rect) { x + i * number->w, y, number->w, number->h }
                      );
      break;
      case 54:
        SDL_RenderCopy(renderer, 
                       number->texture, 
                       &(SDL_Rect) { number->w * 6, 0, number->w, number->h }, 
                       &(SDL_Rect) { x + i * number->w, y, number->w, number->h }
                      );
      break;
      case 55:
        SDL_RenderCopy(renderer, 
                       number->texture, 
                       &(SDL_Rect) { number->w * 7, 0, number->w, number->h }, 
                       &(SDL_Rect) { x + i * number->w, y, number->w, number->h }
                      );
      break;
      case 56:
        SDL_RenderCopy(renderer, 
                       number->texture, 
                       &(SDL_Rect) { number->w * 8, 0, number->w, number->h }, 
                       &(SDL_Rect) { x + i * number->w, y, number->w, number->h }
                      );
      break;
      case 57:
        SDL_RenderCopy(renderer, 
                       number->texture, 
                       &(SDL_Rect) { number->w * 9, 0, number->w, number->h }, 
                       &(SDL_Rect) { x + i * number->w, y, number->w, number->h }
                      );
      break;
    }
  }

  free(n);
  n = NULL;
}
