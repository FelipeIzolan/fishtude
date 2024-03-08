#include "../lib/math.c"
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

Passive * createPassive(SDL_Texture * texture, Passive * parent, int id, double r, int m) {
  Passive * p = malloc(sizeof(Passive));
  
  int x = parent->sprite.position.x;
  int y = parent->sprite.position.y;

  p->id = id;
  p->active = 0;
  p->sprite = id < SYELLOW ? 
    createSpriteTexture(texture, x + cos(r) * m, y + sin(r) * m, 12, 12) : 
    createSpriteTexture(texture, x + cos(r) * m, y + sin(r) * m, 14, 14);

  id < SYELLOW ? 
  setSpriteFrame(&p->sprite, p->id + 1, 1):
  setSpriteFramePosition(&p->sprite, (id - SYELLOW) * 14, 12);

  return p;
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

Node * getNearestPassive(Node * node, char dir) {
  SDL_Rect * c = &((Passive *) node->data)->sprite.position;
  SDL_Point cc = { c->x + c->w / 2, c->y + c->h / 2 };
  
  switch (dir) {
    case 'U':
      cc.y -= c->h / 2;
      break;
    case 'D':
      cc.y += c->h / 2;
      break;
    case 'L':
      cc.x -= c->w / 2;
      break;
    case 'R':
      cc.x += c->w / 2;
      break;
  };

  int d = 9999;
  Node * nn = NULL;

  if (node->parent != NULL) {
    SDL_Rect * pp = &((Passive *) node->parent->data)->sprite.position;
    
    d = distance(cc.x, cc.y, pp->x + pp->w / 2, pp->y + pp->h / 2);
    nn = node->parent;
  }

  for (size_t i = 0; i < node->size; i++) {
    SDL_Rect * cp = &((Passive *) node->children[i]->data)->sprite.position;
    int cd = distance(cc.x, cc.y, cp->x + cp->w / 2, cp->y + cp->h / 2);
    
    if (cd < d) {
      nn = node->children[i];
      d = cd;
    }
  }

  return nn;
}

// ----------------------------------------------------------------

typedef struct PassiveTree {
  Node * root;
  Node * selected;
  Sprite pointer;
} PassiveTree;

PassiveTree createPassiveTree(SDL_Renderer * renderer, SDL_Texture * texture) {
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
  
  PassiveTree passiveTree = {
    root,
    root,
    createSprite(renderer, "./assets/passive_pointer.bmp", 72, 64, 0, 0)
  };

  return passiveTree;
}

void drawPassiveTree(SDL_Renderer * renderer, PassiveTree * pt) {
  drawPassive(renderer, pt->root);
}

void updatePassiveTreeSelect(PassiveTree * passiveTree, char dir) {
  passiveTree->selected = getNearestPassive(passiveTree->selected, dir);
  
  Passive * sp = (Passive *) passiveTree->selected->data;
  int d = sp->id < SYELLOW ? 2 : 1;

  passiveTree->pointer.position.x = sp->sprite.position.x - d;
  passiveTree->pointer.position.y = sp->sprite.position.y - d;
}

