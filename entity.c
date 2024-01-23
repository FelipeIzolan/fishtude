#pragma once

#include <SDL_surface.h>
#include <SDL_render.h>
#include <SDL_rect.h>

#include "animation.c"
#include "utils.c"

typedef struct Entity {
  SDL_Rect position;
  SDL_Point velocity;
  Animation animation;
  SDL_Texture * texture;
} Entity;

Entity createEntity(SDL_Renderer * renderer, char * src, int x, int y, int w, int h) {  
  Entity entity;

  entity.position.x = x;
  entity.position.y = y;
  entity.position.w = w;
  entity.position.h = h;

  entity.velocity.x = 0;
  entity.velocity.y = 0;

  entity.texture = createTexture(renderer, src);
  entity.animation = createAnimation(entity.texture, w, h);

  setAnimation(&entity.animation, 1, 1, 0);

  return entity;
}

Entity createEntityTexture(SDL_Texture * texture, int x, int y, int w, int h) {
  Entity entity;

  entity.position.x = x;
  entity.position.y = y;
  entity.position.w = w;
  entity.position.h = h;

  entity.velocity.x = 0;
  entity.velocity.y = 0;
  
  entity.texture = texture;
  entity.animation = createAnimation(entity.texture, w, h);
  
  return entity;
}

void updateEntity(Entity * entity) {
  updateAnimation(&entity->animation);
  
  entity->position.x += entity->velocity.x;
  entity->position.y += entity->velocity.y;
  entity->velocity.x = 0;
  entity->velocity.y = 0;
}

void drawEntity(SDL_Renderer * renderer, Entity * entity) {
  SDL_RenderCopy(renderer, entity->texture, &entity->animation.frame, &entity->position);
}

void destroyEntity(Entity * entity) {
  SDL_DestroyTexture(entity->texture);
  entity->texture = NULL;
}

void destroyEntityTexture(Entity * entity) {
  if (entity->texture != NULL) {
    SDL_DestroyTexture(entity->texture);
    entity->texture = NULL;
  }
}

int checkEntityCollision(Entity * e1, Entity * e2) {
  return (e1->position.x >= e2->position.x && e1->position.x <= e2->position.x + e2->position.w) &&
         (e1->position.y >= e2->position.y && e1->position.y <= e2->position.y + e2->position.h);
}
