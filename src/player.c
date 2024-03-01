#pragma once

#include <SDL_scancode.h>

#include "../lib/entity.c"
#include "../lib/animation.c"

enum player_state { 
  PLAYER_DEFAULT,
  PLAYER_MECHANIC,
  PLAYER_FISHING,
  PLAYER_BACK
};

typedef struct Player {
  uint state: 2;
  uint gold: 14;
  Entity entity;
} Player;

void updatePlayer(const Uint8 * keyboard, Player * player) {
  if (player->state == PLAYER_DEFAULT) {
    if (keyboard[SDL_SCANCODE_RIGHT]) player->entity.position.x += 1;
    if (keyboard[SDL_SCANCODE_LEFT]) player->entity.position.x -= 1;
    
    if (keyboard[SDL_SCANCODE_LEFT] || keyboard[SDL_SCANCODE_RIGHT]) setAnimation(&player->entity.animation, 2, 2, 4);
    else setAnimation(&player->entity.animation, 1, 4, 4);
  
    player->entity.position.x = CLAMP(player->entity.position.x, 0, 144);
  }

  if (player->state != PLAYER_DEFAULT) {
    setFrameAnimation(&player->entity.animation, 1, 1);
  }
  
  updateAnimation(&player->entity.animation);
}
