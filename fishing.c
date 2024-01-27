#pragma once

#include <SDL_rect.h>

#include "lib/utils.c"
#include "lib/math.c"
#include "lib/sprite.c"

#include "player.c"

typedef struct Fishing {
  SDL_Point start;
  SDL_Point end;
  OscillateRange control;
  
  unsigned int force: 8;
  int direction: 2;

  Sprite frame;
  Sprite pointer;
} Fishing;


void setPreFishing(Fishing * fishing) {

}

void updateFishing(Fishing * fishing, Entity * pplayer,Player * player) {
  if (player->state == PLAYER_PRE_FISHING) { 
    fishing->pointer.position.y = pplayer->position.y + 18 - fishing->force;      
    fishing->force += fishing->direction;

    if (fishing->direction == 1 && fishing->force >= 28) fishing->direction = -1;
    if (fishing->direction == -1 && fishing->force <= 0) fishing->direction = 1;
  }

  if (player->state == PLAYER_FISHING) {
    fishing->end.y = fishing->end.y + 2;
    updateOscillateRange(&fishing->control);

    if (fishing->end.y >= fishing->start.y + fishing->force) player->state = PLAYER_FISHING_BACK;
  }


  if (player->state == PLAYER_FISHING_BACK) {
    fishing->end.y = fishing->end.y - 2;

    if (fishing->control.current != 0) updateOscillateRange(&fishing->control);
    if (fishing->end.y <= fishing->start.y) player->state = PLAYER_NORMAL;  
  }
}
