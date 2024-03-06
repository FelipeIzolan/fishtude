#pragma once

#include <SDL_rect.h>

#include "../lib/entity.c"
#include "../lib/utils.c"
#include "../lib/math.c"
#include "../lib/sprite.c"

#include "player.c"

typedef struct Fishing {
  SDL_Point start;
  SDL_Point end;

  OscillateRange control;
  OscillateRange force;

  Sprite frame;
  Sprite pointer;
} Fishing;

void setPreFishing(Fishing * fishing, Player * player) {
  fishing->force.current = 0;

  fishing->frame.position.x = player->entity.position.x + 20;
  fishing->frame.position.y = player->entity.position.y - 10;

  fishing->pointer.position.x = player->entity.position.x + 15;
  fishing->pointer.position.y = player->entity.position.y + 18;
              
  fishing->start.x = player->entity.position.x + 14;
  fishing->start.y = player->entity.position.y + 6;
  fishing->end = fishing->start;
}

void setFishing(Fishing * fishing) {
  fishing->force.current = floorf(((float)fishing->force.current / fishing->force.max) * 108) + 2; 
}

void updateFishing(Fishing * fishing, Player * player) {
  if (player->state == PLAYER_MECHANIC) { 
    updateOscillateRange(&fishing->force);
    fishing->pointer.position.y = player->entity.position.y + 18 - fishing->force.current; 
  }

  if (player->state == PLAYER_FISHING) {
    fishing->end.y = fishing->end.y + 2;
    updateOscillateRange(&fishing->control);

    if (fishing->end.y >= fishing->start.y + fishing->force.current) player->state = PLAYER_BACK;
  }

  if (player->state == PLAYER_BACK) {
    fishing->end.y = fishing->end.y - 2;

    if (fishing->end.y <= fishing->start.y) player->state = PLAYER_DEFAULT;  
  }
}

void drawFishingLine(SDL_Renderer * renderer, Fishing * fishing, Player * player) {
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  
  if (player->state == PLAYER_FISHING) {  
    SDL_Point midpoint = lineMidpoint(fishing->start, fishing->end);
    SDL_Point p1 = lineMidpoint(fishing->start, midpoint);
    SDL_Point p2 = lineMidpoint(midpoint, fishing->end);
     
    p1.x += fishing->control.current;
    p2.x += -fishing->control.current;

    for (float i = 0; i < 1; i += 0.001) {
      SDL_Point p = cubicBenzierCurve(fishing->start, p1, p2, fishing->end, i);
      SDL_RenderDrawPoint(renderer, p.x, p.y);
    }

    #ifdef DEBUG
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawPoint(renderer, p1.x, p1.y);
    SDL_RenderDrawPoint(renderer, p2.x, p2.y);
    SDL_RenderDrawPoint(renderer, fishing->end.x, fishing->end.y);
    #endif
  }

  if (player->state == PLAYER_BACK) {
    SDL_RenderDrawLine(renderer, fishing->start.x, fishing->start.y, fishing->end.x, fishing->end.y);
    #ifdef DEBUG
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawPoint(renderer, fishing->end.x, fishing->end.y);
    #endif
  }

}

void drawFishingInterface(SDL_Renderer * renderer, Fishing * fishing, Player * player) {
  if (player->state == PLAYER_MECHANIC) {
    drawSprite(renderer, &fishing->frame);
    drawSprite(renderer, &fishing->pointer);
  }
}
