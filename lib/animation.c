#pragma once

#include <math.h>

#include <SDL_render.h>
#include <SDL_rect.h>

#include "math.c"

typedef struct Animation {
  SDL_Rect frame;
  float time;
  float multiplier;
  int row;
  int max_row;
  int max_column;
} Animation;

Animation createAnimation(SDL_Texture * texture, int w, int h) {
  Animation animation;
  int tw, th;

  SDL_QueryTexture(texture, NULL, NULL, &tw, &th);

  animation.frame = (SDL_Rect) { 0, 0, w, h };
  animation.max_column = tw / w;
  animation.max_row = th / h;

  return animation;
}

void setAnimation(Animation * animation, int row, int max_column, float multiplier) {
  if (animation->row == row) return;

  animation->time = 0;
  animation->row = row;
  animation->max_column = max_column;
  animation->multiplier = multiplier;

  animation->frame.x = 0;
  animation->frame.y = (row >= animation->max_row ? animation->max_row - 1 : row - 1) * animation->frame.h;
}

void setFrameAnimation(Animation * animation, int column, int row) {
  animation->max_column = 0;
  animation->frame.x = (column - 1) * animation->frame.w;
  animation->frame.y = (row >= animation->max_row ? animation->max_row - 1 : row - 1) * animation->frame.h;
}

void updateAnimation(Animation * animation) {
  if (animation->max_column > 0) {
    animation->time = ICLAMP(animation->time + DELTA * animation->multiplier, 0, animation->max_column);
    animation->frame.x = floorf(animation->time) * animation->frame.w;
  }
}
