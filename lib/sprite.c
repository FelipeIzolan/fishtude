#pragma once

#include <SDL_render.h>
#include <SDL_rect.h>

#include "utils.c"

typedef struct Sprite {
  SDL_Rect frame;
  SDL_Rect position;
  SDL_Texture * texture;
} Sprite;

Sprite createSprite(SDL_Renderer * renderer, void * data, int x, int y, int w, int h, int sw, int sh) {
  Sprite sprite;

  sprite.position.x = x;
  sprite.position.y = y;
  sprite.texture = createTexture(renderer, data, sw, sh);

  if (w == 0) sprite.position.w = sw;
  else sprite.position.w = w;
  
  if (h == 0) sprite.position.h = sh;
  else sprite.position.h = h;

  sprite.frame = (SDL_Rect) { 
    0, 
    0,
    sprite.position.w,
    sprite.position.h
  };
  
  return sprite;
}

Sprite createSpriteTexture(SDL_Texture * texture, int x, int y, int w, int h) {
  Sprite sprite;
  int sw, sh;

  sprite.texture = texture;  
  SDL_QueryTexture(sprite.texture, NULL, NULL, &sw, &sh);

  sprite.position.x = x;
  sprite.position.y = y;

  if (w == 0) sprite.position.w = sw;
  else sprite.position.w = w;
  
  if (h == 0) sprite.position.h = sh;
  else sprite.position.h = h;

  sprite.frame = (SDL_Rect) { 
    0, 
    0,
    sprite.position.w,
    sprite.position.h
  };
 
  return sprite;
}

void setSpriteFrame(Sprite * sprite, int column, int row) {
  sprite->frame.x = (column - 1) * sprite->frame.w;
  sprite->frame.y = (row - 1) * sprite->frame.h;
}

void drawSprite(SDL_Renderer * renderer, Sprite * sprite) {
  SDL_RenderCopy(renderer, sprite->texture, &sprite->frame, &sprite->position);
}
