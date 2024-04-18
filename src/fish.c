#pragma once

#include "../lib/cvector.h"
#include "../lib/entity.c"
#include "../lib/math.c"
#include "../lib/utils.c"

#include "fishing.c"
#include "player.c"
#include <math.h>

enum FishState {
  FISH_DEFAULT,
  FISH_WAVE,
  FISH_BACK,
  FISH_WAVE_BACK
};

struct FishMovement {
  SineWave wave;
  int back;
};

typedef struct Fish {   
  uint catch: 1;
  uint gold: 6; // max=64;
  uint state: 2;
  Entity entity;
  struct FishMovement movement;
} Fish;

Fish createFish(SDL_Texture * texture) {
  Fish fish = { 0, 0, FISH_DEFAULT, createEntityTexture(texture, 0, rrandom(40, 128), 8, 8) };

  // MOVEMENT ---------------------------------------
  double c = drandom();

  if (c <= 0.25) {
    fish.state = FISH_WAVE_BACK;
    fish.movement.back = rrandom(16, 144);
    fish.movement.wave = (SineWave) { rrandom(4, 10), rrandom(2, 8), fish.entity.position.y };
  } else if (c <= 0.5) {
    fish.state = FISH_WAVE;
    fish.movement.wave = (SineWave) { rrandom(4, 10), rrandom(2, 8), fish.entity.position.y };
  } else if (c <= 0.75) {
    fish.state = FISH_BACK; 
    fish.movement.back = rrandom(16, 144);
  }
  // -------------------------------------------------

  // DIRECTION ---------------------------------------
  if (drandom() <= 0.48) fish.entity.flip = SDL_FLIP_HORIZONTAL;
  if (fish.entity.flip == SDL_FLIP_HORIZONTAL) fish.entity.position.x = rrandom(168, 680);
  else fish.entity.position.x = rrandom(-520, -8);
  // -------------------------------------------------
  
  fish.gold = 2 + round((double)(fish.entity.position.y - 40) / 88 * 16);

  setAnimationFrame(&fish.entity.animation, rrandom(1, 16), rrandom(1,4));
  return fish;
}

void updateFish(Fish * fish, Player * player, Fishing * fishing, Fish * vector, int index) {
  if (!fish->catch) {
    fish->entity.position.x += fish->entity.flip == SDL_FLIP_HORIZONTAL ? -1 : 1;
    
    switch (fish->state) {
      case FISH_DEFAULT:      
        break;
      case FISH_WAVE:
        fish->entity.position.y = calcSineWave(&fish->movement.wave, fish->entity.position.x);
        break;
      case FISH_BACK:
        if (fish->entity.position.x == fish->movement.back) 
          fish->entity.flip = fish->entity.flip == SDL_FLIP_HORIZONTAL ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
        break;
      case FISH_WAVE_BACK: 
        fish->entity.position.y = calcSineWave(&fish->movement.wave, fish->entity.position.x);
        if (fish->entity.position.x == fish->movement.back)
          fish->entity.flip = fish->entity.flip == SDL_FLIP_HORIZONTAL ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
        break;
    }

    if (checkEntityAndPointCollision(&fish->entity, &fishing->end)) fish->catch = 1;
    if (fish->entity.flip == SDL_FLIP_NONE && fish->entity.position.x >= 168) cvector_erase(vector, index); 
    if (fish->entity.flip == SDL_FLIP_HORIZONTAL && fish->entity.position.x <= -8) cvector_erase(vector, index);
  } else {
    fish->entity.position.x = fishing->end.x - 4;
    fish->entity.position.y = fishing->end.y - 4;

    if (checkEntityCollision(&fish->entity, &player->entity)) {
      player->gold += (fish->gold + player->status.fish_gold_base) * player->status.fish_gold_multiplier;
      cvector_erase(vector, index);
    }
  }
}
