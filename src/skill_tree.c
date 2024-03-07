#include <math.h>
#include <stdlib.h>
#include <sys/types.h>

#include <SDL_rect.h>
#include <SDL_render.h>

#include "../lib/tree.c"
#include "../lib/sprite.c"

enum passive_id {
  YELLOW,
  BLUE,
  GREEN,
  PURPLE,
  RED,
  SYELLOW
};

typedef struct Passive {
  int id;
  uint active: 1;
  Sprite sprite;
} Passive;

typedef struct PassivePointer {
  Node * node;
  Sprite sprite;
} PassivePointer;

Passive * createPassive(SDL_Texture * texture, Passive * pp, int id, double r, int m) {
  Passive * p = malloc(sizeof(Passive));
  
  int x = pp->sprite.position.x;
  int y = pp->sprite.position.y;

  p->id = id;
  p->active = 0;
  p->sprite = id < SYELLOW ? 
    createSpriteTexture(texture, x + cos(r) * m, y + sin(r) * m, 12, 12) : 
    createSpriteTexture(texture, x + cos(r) * m, y + sin(r) * m, 14, 14);

  id < SYELLOW ? 
  setSpriteFrame(&p->sprite, p->id + 1, 1):
  setSpriteFramePosition(&p->sprite, (id - 5) * 14, 12);

  return p;
}

Node * createSkillTree(SDL_Texture * texture) {
  Passive * p = malloc(sizeof(Passive));

  p->id = 1;
  p->active = 0;
  p->sprite = createSpriteTexture(texture, 74, 66, 12, 12);


  Node * root = NodeNew(p, 4);
  Node * s1 = NodeNewChild(root, createPassive(texture, ((Passive *)root->data), SYELLOW, 3 * M_PI / 2, 18), 2);

  NodeNewChild(root, createPassive(texture, ((Passive *)root->data), BLUE, 2 * M_PI, 18), 0);
  NodeNewChild(root, createPassive(texture, ((Passive *)root->data), GREEN, M_PI, 18), 0);
  NodeNewChild(root, createPassive(texture, ((Passive *)root->data), PURPLE, M_PI / 2, 18), 0);

  NodeNewChild(s1, createPassive(texture, ((Passive *)s1->data), YELLOW, (4 * M_PI) / 3, 18), 0);
  NodeNewChild(s1, createPassive(texture, ((Passive *)s1->data), YELLOW, (5 * M_PI) / 3, 18), 0);

  return root;
}

void drawSkillTree(SDL_Renderer * renderer, Node * node) {
  Passive * p = ((Passive *)node->data);

  for (int i = 0; i < node->length; i++) {
    ((Passive * )node->children[i]->data)->active ?
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255) :
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);

    SDL_RenderDrawLine(
      renderer, 
      p->sprite.position.x + 6,
      p->sprite.position.y + 6, 
      ((Passive *)node->children[i]->data)->sprite.position.x + 6,
      ((Passive *)node->children[i]->data)->sprite.position.y + 6
    );

    drawSkillTree(renderer, node->children[i]);
  }

  if (p->active) SDL_SetTextureColorMod(p->sprite.texture, 255, 255, 255);
  else SDL_SetTextureColorMod(p->sprite.texture, 100, 100, 100);

  drawSprite(renderer, &((Passive *)node->data)->sprite);
}
