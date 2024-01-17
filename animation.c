#pragma once

#include <math.h>

#include <SDL_render.h>
#include <SDL_rect.h>

typedef struct Animation {
  SDL_Rect frame;
  float time;
  int column;
  int max_row;
  int max_column;
} Animation;

Animation createAnimation(SDL_Texture * texture, int w, int h) {
  Animation animation;
  int tw, th;

  SDL_QueryTexture(texture, NULL, NULL, &tw, &th);

  animation.frame.w = w;
  animation.frame.h = h;

  animation.max_row = tw / w;
  animation.max_column = th / h;

  return animation;
}

void setAnimation(Animation * animation, int column, int max_row) {
  if (animation->column == column) return;

  animation->time = 0;
  animation->max_row = max_row;
  animation->frame.x = 0;
  animation->frame.y = (column >= animation->max_column ? animation->max_column - 1 : column - 1) * animation->frame.h;
}

void updateAnimation(Animation * animation, float delta) {
  animation->time = animation->time + delta >= animation->max_row ? 0 : animation->time + delta;
  animation->frame.x = floorf(animation->time) * animation->frame.w;
}
