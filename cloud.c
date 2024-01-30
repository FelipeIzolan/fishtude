#include <math.h>
#include "SDL_log.h"
#include "SDL_stdinc.h"

#include "lib/math.c"
#include "lib/sprite.c"


void createCloud(Sprite * arr, SDL_Texture * texture) {
  for (int i = 0; i < 8; i++) {
    arr[i] = createSpriteTexture(texture, rrandom(-WINDOW_WIDTH * 10, WINDOW_WIDTH), rrandom(0, 16), 32, 16);
    setFrameSprite(&arr[i], (int) ceilf((float)(i+1)/4), i + 1 - (i > 3 ? 4 : 0));
  }
}

void updateCloud(Sprite * sprite) {
  sprite->position.x += 1;

  if (sprite->position.x >= WINDOW_WIDTH + 32) {
    sprite->position.x = rrandom(-WINDOW_WIDTH  * 10, -32);
    sprite->position.y = rrandom(0, 12);
  }
}
