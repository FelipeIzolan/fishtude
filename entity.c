#pragma once

#include <SDL_surface.h>
#include <SDL_render.h>

#include "animation.c"

typedef struct Entity {
  SDL_Rect position;
  Animation animation;
  SDL_Texture * texture;
} Entity;

Entity createEntity(SDL_Renderer * renderer, char * src, int x, int y, int w, int h) {
  SDL_Surface * surface = SDL_LoadBMP(src);
  
  Entity entity;

  entity.position.x = x;
  entity.position.y = y;
  entity.position.w = w;
  entity.position.h = h;

  entity.texture = SDL_CreateTextureFromSurface(renderer, surface);
  entity.animation = createAnimation(entity.texture, w, h);

  SDL_FreeSurface(surface);

  return entity;
}

void drawEntity(SDL_Renderer * renderer, Entity * entity) {
  SDL_RenderCopy(renderer, entity->texture, &entity->animation.frame, &entity->position);
}

void destroyEntity(Entity * entity) {
  SDL_DestroyTexture(entity->texture);
}
