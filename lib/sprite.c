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

  if (w == 0) sprite.position.w = tw;
  else sprite.position.w = w;
  
  if (h == 0) sprite.position.h = th;
  else sprite.position.h = h;

  sprite.frame.x = 0;
  sprite.frame.y = 0;
  sprite.frame.w = sprite.position.w;
  sprite.frame.h = sprite.position.h;
  
  return sprite;
}

Sprite createSpriteTexture(SDL_Texture * texture, int x, int y, int w, int h) {
  Sprite sprite;
  int tw, th;

  sprite.texture = texture;  
  SDL_QueryTexture(sprite.texture, NULL, NULL, &tw, &th);

  sprite.position.x = x;
  sprite.position.y = y;

  if (w == 0) sprite.position.w = tw;
  else sprite.position.w = w;
  
  if (h == 0) sprite.position.h = th;
  else sprite.position.h = h;

  sprite.frame.x = 0;
  sprite.frame.y = 0;
  sprite.frame.w = sprite.position.w;
  sprite.frame.h = sprite.position.h;
  
  return sprite;
}

void setFrameSprite(Sprite * sprite, int column, int row) {
  sprite->frame.x = (row - 1) * sprite->frame.w;
  sprite->frame.y = (column - 1) * sprite->frame.h;
}

void drawSprite(SDL_Renderer * renderer, Sprite * sprite) {
  SDL_RenderCopy(renderer, sprite->texture, &sprite->frame, &sprite->position);
}

void destroySprite(Sprite * sprite) {
  SDL_DestroyTexture(sprite->texture);
  sprite->texture = NULL;
}
