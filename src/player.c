#pragma once

#include <SDL_scancode.h>

#include "../lib/entity.c"
#include "../lib/animation.c"
#include "./passive/passive.c"

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

void applyToPlayerStatus(uint id, Player * player) {
  PlayerStatus * status = &player->status;

  switch (id) {
    case YELLOW: status->fish_spawn_quantity += 2; break;
    case BLUE: status->fish_spawn_time_decrease += 0.2; break;
    case GREEN: status->fish_gold_base += 1; break;
    case PURPLE: status->fish_gold_multiplier += 0.08; break;
    case RED: status->gold_passive_income += 5; break;

    case SYELLOW: status->fish_spawn_quantity += 5; status->fish_spawn_time_decrease += 0.1; break;
    case SBLUE: status->fish_spawn_time_decrease += 0.5; status->fish_spawn_quantity += 1; break;
    case SGREEN: status->fish_gold_base += 3; status->fish_gold_multiplier += 0.05; break;
    case SPURPLE: status->fish_gold_multiplier += 0.1; status->fish_gold_base += 1; break;
    case SRED: status->gold_passive_income += 12; break;
  }

  status->passive_price_multiplier += 0.05;
}
