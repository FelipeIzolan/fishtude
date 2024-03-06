#include "../lib/math.c"
#include "../lib/sprite.c"

void createCloud(Sprite * arr, SDL_Texture * texture) {
  short i = 0;
  for (int y = 1; y <= 2; y++) {
    for (int x = 1; x <= 4; x++) {
      arr[i] = createSpriteTexture(texture, rrandom(-1600, WINDOW_WIDTH), rrandom(0, 16), 32, 16);
      setSpriteFrame(&arr[i], x, y);
      i++;
    }
  }
}

void updateCloud(Sprite * sprite) {
  sprite->position.x += 1;

  if (sprite->position.x >= WINDOW_WIDTH + 32) {
    sprite->position.x = rrandom(-1600, -32);
    sprite->position.y = rrandom(0, 12);
  }
}
