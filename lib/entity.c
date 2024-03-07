#pragma once

#include <SDL_surface.h>
#include <SDL_render.h>
#include <SDL_rect.h>

#include "animation.c"
#include "utils.c"

typedef struct Entity {
  SDL_Rect position;
  Animation animation;
  SDL_Texture * texture;
  SDL_RendererFlip flip;
} Entity;

Entity createEntity(SDL_Renderer * renderer, char * src, int x, int y, int w, int h) {  
  Entity entity;

  entity.flip = SDL_FLIP_NONE;
  entity.position = (SDL_Rect) { x, y, w, h };
  entity.texture = createTexture(renderer, src);
  entity.animation = createAnimation(entity.texture, w, h);

  return entity;
}

Entity createEntityTexture(SDL_Texture * texture, int x, int y, int w, int h) {
  Entity entity;

  entity.flip = SDL_FLIP_NONE;
  entity.position = (SDL_Rect) { x, y, w, h };
  entity.texture = texture;
  entity.animation = createAnimation(entity.texture, w, h);
  
  return entity;
}

void drawEntity(SDL_Renderer * renderer, Entity * entity) {
  SDL_RenderCopyEx(renderer, entity->texture, &entity->animation.frame, &entity->position, 0, NULL, entity->flip);
  
  #ifdef DEBUG
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  SDL_RenderDrawRect(renderer, &entity->position);
  #endif
}

int checkEntityCollision(Entity * e1, Entity * e2) {
  return (e1->position.x + e1->position.w >= e2->position.x && e1->position.x <= e2->position.x + e2->position.w) &&
         (e1->position.y + e1->position.h >= e2->position.y && e1->position.y <= e2->position.y + e2->position.h);
}

int checkEntityAndRectCollision(Entity * e, SDL_Rect * r) {
  return (e->position.x + e->position.w >= r->x && e->position.x <= r->x + r->w) &&
         (e->position.y + e->position.h >= r->y && e->position.y <= r->y + r->h);
}

int checkEntityAndPointCollision(Entity * e, SDL_Point * p) {
  return (e->position.x + e->position.w >= p->x && e->position.x <= p->x) &&
         (e->position.y + e->position.h >= p->y && e->position.y <= p->y);
}
