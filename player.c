#pragma once

#include <SDL_scancode.h>

#include "lib/entity.c"
#include "lib/animation.c"

typedef struct Player {
  short int state;
  long long int gold;
} Player;


void updatePlayer(const Uint8 * keyboard, Entity * entity, Player * data) {
  if (data->state == PLAYER_NORMAL) {
    if (keyboard[SDL_SCANCODE_RIGHT]) entity->position.x += 1;
    if (keyboard[SDL_SCANCODE_LEFT]) entity->position.x -= 1;
    
    if (keyboard[SDL_SCANCODE_LEFT] || keyboard[SDL_SCANCODE_RIGHT]) setAnimation(&entity->animation, 2, 2, 4);
    else setAnimation(&entity->animation, 1, 4, 4);
  }

  if (data->state != PLAYER_NORMAL) {
    setFrameAnimation(&entity->animation, 1, 1);
  }
  
  updateAnimation(&entity->animation);
  entity->position.x = CLAMP(entity->position.x, 0, 144);
}
