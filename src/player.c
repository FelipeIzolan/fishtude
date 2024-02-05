#pragma once

#include <SDL_scancode.h>

#include "../lib/entity.c"
#include "../lib/animation.c"

typedef struct Player {
  short int state;
  long long int gold;
  Entity entity;
} Player;

void updatePlayer(const Uint8 * keyboard, Player * player) {
  if (player->state == PLAYER_NORMAL) {
    if (keyboard[SDL_SCANCODE_RIGHT]) player->entity.position.x += 1;
    if (keyboard[SDL_SCANCODE_LEFT]) player->entity.position.x -= 1;
    
    if (keyboard[SDL_SCANCODE_LEFT] || keyboard[SDL_SCANCODE_RIGHT]) setAnimation(&player->entity.animation, 2, 2, 4);
    else setAnimation(&player->entity.animation, 1, 4, 4);
  }

  if (player->state != PLAYER_NORMAL) {
    setFrameAnimation(&player->entity.animation, 1, 1);
  }
  
  updateAnimation(&player->entity.animation);
  player->entity.position.x = CLAMP(player->entity.position.x, 0, 144);
}
