#pragma once

#include <SDL_scancode.h>
#include "lib/entity.c"

typedef struct Player {
  short int state;
  long long int gold;
} Player;


void updatePlayer(const Uint8 * keyboard, Entity * entity, Player * data) {
  if (data->state == PLAYER_NORMAL) {
    if (keyboard[SDL_SCANCODE_RIGHT]) entity->velocity.x = 1;
    if (keyboard[SDL_SCANCODE_LEFT]) entity->velocity.x = -1;
  }

  if (entity->velocity.x == 0) setAnimation(&entity->animation, 1, 4, 4);
  else setAnimation(&entity->animation, 2, 2, 4);

  updateEntity(entity);

  entity->position.x = CLAMP(entity->position.x, 0, 144);
}

void drawPlayer(SDL_Renderer * renderer, Entity * entity) {
  drawEntity(renderer, entity);
  #ifdef DEBUG
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  SDL_RenderDrawRect(renderer, &entity->position);
  #endif
}
