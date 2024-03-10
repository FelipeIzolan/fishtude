#include "../lib/math.c"
#include "../lib/tree.c"
#include "../lib/sprite.c"

enum passive_id {
  YELLOW,
  RED,
  GREEN,
  BLUE,
  PURPLE,
  SYELLOW
};

typedef struct Passive {
  uint id;
  uint active: 1;
  Sprite sprite;
  SDL_Point base;
} Passive;

Passive * createPassive(SDL_Texture * texture, Passive * parent, int id, double r) {
  Passive * p = malloc(sizeof(Passive));
  
  int x = parent->sprite.position.x;
  int y = parent->sprite.position.y;
  int m = id < SYELLOW ? 0 : 1;

  p->id = id;
  p->active = 0;
  p->sprite = id < SYELLOW ? 
    createSpriteTexture(texture, x - m + cos(r) * 24, y - m + sin(r) * 24, 12, 12) : 
    createSpriteTexture(texture, x - m + cos(r) * 24, y - m + sin(r) * 24, 14, 14);  
  p->base = (SDL_Point) { p->sprite.position.x, p->sprite.position.y };

  id < SYELLOW ? 
  setSpriteFrame(&p->sprite, p->id + 1, 1):
  setSpriteFramePosition(&p->sprite, (id - SYELLOW) * 14, 12);

  return p;
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
  p->base = (SDL_Point) { p->sprite.position.x, p->sprite.position.y };

  Node * root = NodeNew(p, 3);
  NodeNewChild(root, createPassive(texture, ((Passive *)root->data), SYELLOW, 3 * M_PI / 2), 2);

  PassiveTree passiveTree = {
    root,
    root,
    createSprite(renderer, "./assets/passive_pointer.bmp", 72, 64, 0, 0)
  };

  return passiveTree;
}

void updatePassiveTreeSelect(PassiveTree * passiveTree, SDL_Rect * camera, char dir) {
  passiveTree->selected = getNearestPassive(passiveTree->selected, dir);
  
  Passive * sp = (Passive *) passiveTree->selected->data;
  int m = sp->id < SYELLOW ? 2 : 1;
  
  camera->x = (sp->base.x + sp->sprite.frame.w / 2) - camera->w / 2;
  camera->y = (sp->base.y + sp->sprite.frame.h / 2) - camera->h / 2;
  
  passiveTree->pointer.position.x = (sp->base.x - m) - camera->x;
  passiveTree->pointer.position.y = (sp->base.y - m) - camera->y;

  updatePassive(camera, passiveTree->root);
}

