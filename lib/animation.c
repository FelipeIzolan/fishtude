#pragma once

#include <math.h>
#include <sys/types.h>

#include <SDL_render.h>
#include <SDL_rect.h>

#include "math.c"

typedef struct Animation {
  SDL_Rect frame;
  float time;
  ushort fy; // frame-y (NO ZERO-BASED INDEXING)
  ushort fc; // frame-count
  ushort fr; // frame-rate
} Animation;

Animation createAnimation(SDL_Texture * texture, int fw, int fh) {
  Animation animation;
  int tw, th;

  SDL_QueryTexture(texture, NULL, NULL, &tw, &th);

  animation = (Animation) {
    (SDL_Rect) { 0, 0, fw, fh },
    0,
    1,
    0,
    8
  };

  return animation;
}

void setAnimation(Animation * animation, int fy, int fc, int fr) {
  if (animation->fy == fy) return;

  animation->time = 0;
  animation->fy = fy;
  animation->fc = fc;
  animation->fr = fr;

  animation->frame.x = 0;
  animation->frame.y = (fy - 1) * animation->frame.h;
}

void setFrameAnimation(Animation * animation, int fx, int fy) {
  animation->fc = 0;
  animation->fy = fy - 1;

  animation->frame.x = (fx - 1) * animation->frame.w;
  animation->frame.y = (fy - 1) * animation->frame.h;
}

void updateAnimation(Animation * animation) {
  if (animation->fc > 0) {
    animation->time = ICLAMP(animation->time + ((float)animation->fr / 100), 0, animation->fc);
    animation->frame.x = floorf(animation->time) * animation->frame.w;
  }
}
