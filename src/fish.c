#pragma once

#include "../lib/cvector.h"
#include "../lib/entity.c"
#include "../lib/math.c"
#include "../lib/utils.c"

#include "fishing.c"

enum fish_state {
  FISH_DEFAULT,
  FISH_WAVE,
  FISH_BACK
};

union FishMovement {
  SineWave wave;
  int back;
};

typedef struct Fish {   
  int catch: 2;
  int price: 8; // max = 255
  Entity entity;
  uint state: 4;
  union FishMovement movement;
} Fish;

Fish createFish(SDL_Texture * texture) {
  Fish fish = { 0, 0, createEntityTexture(texture, 0, rrandom(40, 128), 8, 8), FISH_DEFAULT };
  setFrameAnimation(&fish.entity.animation, rrandom(1, 16), 1);
 
  if (drandom() < 0.48) fish.entity.flip = SDL_FLIP_HORIZONTAL;
  
  if (fish.entity.flip == SDL_FLIP_HORIZONTAL) fish.entity.position.x = rrandom(168, 680);
  else fish.entity.position.x = rrandom(-520, -8);

  double c = drandom();

  if (c > 0.8) { 
    fish.state = FISH_BACK; 
    fish.movement.back = rrandom(16, 144);
  } else if (c > 0.5) {
    fish.state = FISH_WAVE;
    fish.movement.wave = (SineWave) { rrandom(4, 10), rrandom(1, 8), fish.entity.position.y };
  }

  return fish;
}

void updateFish(Fish * fish, Entity * player, Fishing * fishing, Fish * vector, int index) {
  if (!fish->catch) {
    switch (fish->state) {
      case FISH_DEFAULT:      
        fish->entity.position.x += fish->entity.flip == SDL_FLIP_HORIZONTAL ? -1 : 1;
        break;
      case FISH_WAVE:
        fish->entity.position.x += fish->entity.flip == SDL_FLIP_HORIZONTAL ? -1 : 1;
        fish->entity.position.y = calcSineWave(&fish->movement.wave, fish->entity.position.x);
        break;
      case FISH_BACK:
        fish->entity.position.x += fish->entity.flip == SDL_FLIP_HORIZONTAL ? -1 : 1;
        if (fish->entity.position.x == fish->movement.back) 
          fish->entity.flip = fish->entity.flip == SDL_FLIP_HORIZONTAL ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
    }

    if (checkEntityAndPointCollision(&fish->entity, &fishing->end)) fish->catch = 1;
    if (fish->entity.flip == SDL_FLIP_NONE && fish->entity.position.x >= 168) cvector_erase(vector, index); 
    if (fish->entity.flip == SDL_FLIP_HORIZONTAL && fish->entity.position.x <= -8) cvector_erase(vector, index);
  } else {
    fish->entity.position.x = fishing->end.x - 4;
    fish->entity.position.y = fishing->end.y - 4;

    if (checkEntityCollision(&fish->entity, player)) cvector_erase(vector, index);
  } 
}