#pragma once

#include "../lib/cvector.h"
#include "../lib/entity.c"
#include "../lib/math.c"

#include "fishing.c"

typedef struct Fish { 
  int catch: 2;
  int price: 8; // max = 255
  Entity entity;
} Fish;

Fish createFish(SDL_Texture * texture) {
  Fish fish = { 0, 0, createEntityTexture(texture, 0, rrandom(40, 136), 8, 8) };
  setFrameAnimation(&fish.entity.animation, rrandom(1, 16), 1);
 
  if (drandom() > 0.65) fish.entity.flip = SDL_FLIP_HORIZONTAL;

  if (fish.entity.flip == SDL_FLIP_HORIZONTAL) fish.entity.position.x = rrandom(168, 680);
  else fish.entity.position.x = rrandom(-520, -8);

  return fish;
}

void updateFish(Fish * fish, Entity * player, Fishing * fishing, Fish * vector, int index) {
  if (!fish->catch) {  
    fish->entity.position.x += fish->entity.flip == SDL_FLIP_HORIZONTAL ? -1 : 1;

    if (checkEntityAndPointCollision(&fish->entity, &fishing->end)) fish->catch = 1;
    if (fish->entity.flip == SDL_FLIP_NONE && fish->entity.position.x >= 168) cvector_erase(vector, index); 
    if (fish->entity.flip == SDL_FLIP_HORIZONTAL && fish->entity.position.x <= -8) cvector_erase(vector, index);
  } else {
    fish->entity.position.x = fishing->end.x - 4;
    fish->entity.position.y = fishing->end.y - 4;

    if (checkEntityCollision(&fish->entity, player)) cvector_erase(vector, index);
  } 
}
