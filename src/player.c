#pragma once

#include <SDL_scancode.h>

#include "../lib/entity.c"
#include "../lib/animation.c"

enum PlayerState { 
  PLAYER_DEFAULT,
  PLAYER_MECHANIC,
  PLAYER_FISHING,
  PLAYER_BACK
};

typedef struct PlayerStatus {
  short fish_gold_base;
  float fish_gold_multiplier;
  short fish_spawn_quantity;
  float fish_spawn_time_decrease;
  short gold_passive_income;
  float passive_price_multiplier;
} PlayerStatus;

typedef struct Player {
  uint state: 2;
  uint gold;
  Entity entity;
  PlayerStatus status;
} Player;

void updatePlayer(const Uint8 * keyboard, Player * player) {
  if (player->state == PLAYER_DEFAULT) {
    if (keyboard[SDL_SCANCODE_RIGHT]) player->entity.position.x += 1;
    if (keyboard[SDL_SCANCODE_LEFT]) player->entity.position.x -= 1;
    player->entity.position.x = CLAMP(player->entity.position.x, 0, 144);
    
    if (keyboard[SDL_SCANCODE_LEFT] || keyboard[SDL_SCANCODE_RIGHT]) setAnimation(&player->entity.animation, 2, 2, 12);
    else setAnimation(&player->entity.animation, 1, 4, 12);
  
  }

  if (player->state != PLAYER_DEFAULT) {
    setFrameAnimation(&player->entity.animation, 1, 1);
  }
  
  updateAnimation(&player->entity.animation);
}
