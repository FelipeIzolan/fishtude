#pragma once

#include <SDL_rect.h>

#include "lib/entity.c"
#include "lib/utils.c"
#include "lib/math.c"
#include "lib/sprite.c"

#include "player.c"

typedef struct Fishing {
  SDL_Point start;
  SDL_Point end;
  OscillateRange control;
  
  int force: 8;
  int direction: 2;

  Sprite frame;
  Sprite pointer;
} Fishing;


void setPreFishing(Fishing * fishing, Entity * player) {
  fishing->force = 0;

  fishing->frame.position.x = player->position.x + 20;
  fishing->frame.position.y = player->position.y - 10;
  fishing->pointer.position.x = player->position.x + 15;
  fishing->pointer.position.y = player->position.y + 18;
              
  fishing->start.x = player->position.x + 14;
  fishing->start.y = player->position.y + 6;
  fishing->end = fishing->start;
}

void setFishing(Fishing * fishing) {
  fishing->force = floorf(((float)fishing->force / 28) * 108) + 2; 
}

void updateFishing(Fishing * fishing, Entity * eplayer, Player * dplayer) {
  if (dplayer->state == PLAYER_PRE_FISHING) { 
    fishing->force += fishing->direction;
    fishing->pointer.position.y = eplayer->position.y + 18 - fishing->force;      

    if (fishing->direction == 1 && fishing->force >= 28) fishing->direction = -1;
    if (fishing->direction == -1 && fishing->force <= 0) fishing->direction = 1;
  }

  if (dplayer->state == PLAYER_FISHING) {
    fishing->end.y = fishing->end.y + 2;
    updateOscillateRange(&fishing->control);

    if (fishing->end.y >= fishing->start.y + fishing->force) dplayer->state = PLAYER_FISHING_BACK;
  }


  if (dplayer->state == PLAYER_FISHING_BACK) {
    fishing->end.y = fishing->end.y - 2;

    if (fishing->control.current != 0) updateOscillateRange(&fishing->control);
    if (fishing->end.y <= fishing->start.y) dplayer->state = PLAYER_NORMAL;  
  }
}
