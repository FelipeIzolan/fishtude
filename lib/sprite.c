#pragma once

#include <SDL_render.h>
#include <SDL_rect.h>

#include "utils.c"

typedef struct Sprite {
  SDL_Rect frame;
  SDL_Rect position;
  SDL_Texture * texture;
} Sprite;

Sprite createSprite(SDL_Renderer * renderer, char * src, int x, int y, int w, int h) {
  Sprite sprite;
  int tw, th;

  sprite.texture = createTexture(renderer, src);  
  SDL_QueryTexture(sprite.texture, NULL, NULL, &tw, &th);

  sprite.position.x = x;
  sprite.position.y = y;
  sprite.position.h = h;

  if (w == 0) { 
    sprite.frame.w = tw;
    sprite.position.w = tw;
  } else {
    sprite.frame.w = w;
    sprite.position.w = w;
  }
  
  if (h == 0) { 
    sprite.frame.h = th;
    sprite.position.h = th;
  } else {
    sprite.frame.h = h;
    sprite.position.h = h;
  }

  return sprite;
}

void drawSprite(SDL_Renderer * renderer, Sprite * sprite) {
  SDL_RenderCopy(renderer, sprite->texture, NULL, &sprite->position);
}

void destroySprite(Sprite * sprite) {
  SDL_DestroyTexture(sprite->texture);
  sprite->texture = NULL;
}
