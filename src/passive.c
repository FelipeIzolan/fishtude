#pragma once

#include "../lib/sprite.c"
#include "../lib/tree.c"

enum PassiveId {
  YELLOW,
  BLUE,
  GREEN,
  PURPLE,
  RED,
  // ---
  SYELLOW,
  SBLUE,
  SGREEN,
  SPURPLE,
  SRED
};

typedef struct Passive {
  uint id;
  uint active: 1;
  Sprite sprite;
  SDL_Point base;
} Passive;

Node * createPassive(SDL_Texture * texture, Node * parent, int id, double r, int c) {
  Passive * p = malloc(sizeof(Passive));
  Passive * pp = ((Passive *) parent->data);
  
  int x = pp->sprite.position.x;
  int y = pp->sprite.position.y;

  p->id = id;
  p->active = 0;
  p->sprite = createSpriteTexture(texture, round(x + cos(r) * 24), round(y + sin(r) * 24), 14, 14);
  p->base = (SDL_Point) { p->sprite.position.x, p->sprite.position.y };

  setSpriteFrame(&p->sprite, (id < SYELLOW ? id : id - SYELLOW) + 1, (id < SYELLOW ? 1 : 2));

  return NodeNewChild(parent, p, c);
}

void updatePassive(SDL_Rect * camera, Node * node) {
  Passive * p = ((Passive *)node->data);

  for (size_t i = 0; i < node->size; i++)
    updatePassive(camera, node->children[i]);

  p->sprite.position.x = p->base.x - camera->x;
  p->sprite.position.y = p->base.y - camera->y;
}

void drawPassive(SDL_Renderer * renderer, Node * node) {
  Passive * p = ((Passive *)node->data);

  for (size_t i = 0; i < node->size; i++) {
    Passive * c = (Passive * )node->children[i]->data;
   
    c->active ?
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255) :
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);

    SDL_RenderDrawLine(
      renderer, 
      p->sprite.position.x + p->sprite.position.w / 2,
      p->sprite.position.y + p->sprite.position.h / 2, 
      c->sprite.position.x + c->sprite.position.w / 2,
      c->sprite.position.y + c->sprite.position.h / 2 
    );

    drawPassive(renderer, node->children[i]);
  }

  if (p->active) SDL_SetTextureColorMod(p->sprite.texture, 255, 255, 255);
  else SDL_SetTextureColorMod(p->sprite.texture, 100, 100, 100);

  drawSprite(renderer, &((Passive *)node->data)->sprite);
}
