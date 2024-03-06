#include <math.h>
#include <stdlib.h>
#include <sys/types.h>

#include <SDL_rect.h>
#include <SDL_render.h>

#include "../lib/tree.c"
#include "../lib/sprite.c"

typedef struct Passive {
  int id;
  uint active: 1;
  Sprite sprite;
} Passive;

Passive * createPassive(SDL_Texture * texture, int id, int x, int y, double r, int m) {
  Passive * p = malloc(sizeof(Passive));
  
  p->id = id;
  p->active = 0;
  p->sprite = createSpriteTexture(texture, x + cos(r) * m, y + sin(r) * m, 12, 12);
  
  return p;
}

Node * createTree(SDL_Texture * texture) {
  Node * root = NodeNew(createPassive(texture, 1, 8, 28, 0, 0), 2);
  ((Passive *)root->data)->active = 1;

  NodeNewChild(root, createPassive(texture, 1, ((Passive *)root->data)->sprite.position.x, ((Passive *)root->data)->sprite.position.y - 20, 0, 0), 1);
  Node * p1 = NodeNewChild(root, createPassive(texture, 1, ((Passive *)root->data)->sprite.position.x, ((Passive *)root->data)->sprite.position.y, M_PI / 12, 30), 1);

  NodeNewChild(p1, createPassive(texture, 1, ((Passive *)p1->data)->sprite.position.x, ((Passive *)p1->data)->sprite.position.y, 2 * M_PI, 30), 0);


  return root;
}

void drawPassive(SDL_Renderer * renderer, Node * node) {
  Passive * p = ((Passive *)node->data);

  for (int i = 0; i < node->length; i++) { 
    SDL_RenderDrawLine(
      renderer, 
      p->sprite.position.x + 6,
      p->sprite.position.y + 6, 
      ((Passive *)node->children[i]->data)->sprite.position.x + 6,
      ((Passive *)node->children[i]->data)->sprite.position.y + 6
    );

    drawPassive(renderer, node->children[i]);
  }

  if (p->active) SDL_SetTextureColorMod(p->sprite.texture, 255, 255, 255);
  else SDL_SetTextureColorMod(p->sprite.texture, 100, 100, 100);

  drawSprite(renderer, &((Passive *)node->data)->sprite);
}

void drawTree(SDL_Renderer * renderer, Node * root) {
  SDL_SetRenderDrawColor(renderer, 255,255,255,255);
  drawPassive(renderer, root);
/*  drawSprite(renderer, &((Passive *)root->data)->sprite);*/
/*  drawSprite(renderer, &((Passive *)root->children[0]->data)->sprite);*/
  


/*  SDL_RenderDrawLine(renderer, ((Passive *)root->data)->sprite.position.x + 6, ((Passive *)root->data)->sprite.position.y + 6,*/
  /*((Passive *)root->children[0]->data)->sprite.position.x + 6, ((Passive *)root->children[0]->data)->sprite.position.y + 6*/
  /*      );*/
}
