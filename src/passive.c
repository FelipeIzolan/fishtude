#include "./player.c"
#include "../lib/math.c"
#include "../lib/tree.c"
#include "../lib/sprite.c"

#include <math.h>

enum PassiveId {
  YELLOW,
  BLUE,
  GREEN,
  PURPLE,
  RED,
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

typedef struct PassiveTree {
  Node * root;
  Node * selected;
  Sprite pointer;
  SDL_Rect camera;
} PassiveTree;

Passive * createPassive(SDL_Texture * texture, Passive * parent, int id, double r) {
  Passive * p = malloc(sizeof(Passive));
  
  int x = parent->sprite.position.x;
  int y = parent->sprite.position.y;

  p->id = id;
  p->active = 0;
  p->sprite = createSpriteTexture(texture, x + cos(r) * 24, y + sin(r) * 24, 14, 14);
  p->base = (SDL_Point) { p->sprite.position.x, p->sprite.position.y };

  setSpriteFrame(&p->sprite, (id < SYELLOW ? id + 1 : id - SYELLOW + 1), (id < SYELLOW ? 1 : 2));

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
  SDL_Rect * r = &((Passive *) node->data)->sprite.position;
  SDL_Point p = { r->x + r->w / 2, r->y + r->h / 2 };
  
  switch (dir) {
    case 'U':
      p.y -= r->h / 2;
      break;
    case 'D':
      p.y += r->h / 2;
      break;
    case 'L':
      p.x -= r->w / 2;
      break;
    case 'R':
      p.x += r->w / 2;
      break;
  };

  int nearest = 999;
  Node * nearest_node = NULL;

  if (node->parent != NULL) {
    SDL_Rect * parent = &((Passive *) node->parent->data)->sprite.position;
    
    nearest = distance(p.x, p.y, parent->x + parent->w / 2, parent->y + parent->h / 2);
    nearest_node = node->parent;
  }

  for (size_t i = 0; i < node->size; i++) {
    SDL_Rect * child = &((Passive *) node->children[i]->data)->sprite.position;
    int d = distance(p.x, p.y, child->x + child->w / 2, child->y + child->h / 2);
    
    if (d < nearest) {
      nearest = d;
      nearest_node = node->children[i];
    }
  }

  return nearest_node;
}

// ----------------------------------------------------------------

PassiveTree createPassiveTree(SDL_Renderer * renderer, SDL_Texture * texture) {
  Passive * p = malloc(sizeof(Passive));

  p->id = YELLOW;
  p->active = 0;
  p->sprite = createSpriteTexture(texture, 74, 66, 14, 14);
  p->base = (SDL_Point) { p->sprite.position.x, p->sprite.position.y };

  Node * root = NodeNew(p, 4);
  
  // ---
  Node * rc1 = NodeNewChild(root, createPassive(texture, (Passive *) root->data, SYELLOW, 3 * M_PI / 2), 3);
  
  NodeNewChild(rc1, createPassive(texture, (Passive * ) rc1->data, SPURPLE, 3 * M_PI / 2), 0);
  
  Node * sc1 = NodeNewChild(rc1, createPassive(texture, (Passive * ) rc1->data, YELLOW, 5 * M_PI / 4), 2);
  Node * sc2 = NodeNewChild(rc1, createPassive(texture, (Passive * ) rc1->data, YELLOW, 7 * M_PI / 4), 2);

  Node * ssc1 = NodeNewChild(sc1, createPassive(texture, (Passive * ) sc1->data, SBLUE, 3 * M_PI / 2), 0);
  Node * ssc2 = NodeNewChild(sc2, createPassive(texture, (Passive * ) sc2->data, SGREEN, 3 * M_PI / 2), 0);

  Node * sc1c = NodeNewChild(sc1, createPassive(texture, (Passive * ) sc1->data, YELLOW, 5 * M_PI / 4), 2);
  Node * sc2c = NodeNewChild(sc2, createPassive(texture, (Passive * ) sc2->data, YELLOW, 7 * M_PI / 4), 2);
 
  NodeNewChild(sc1c, createPassive(texture, (Passive *) sc1c->data, SYELLOW, M_PI / 2), 0);
  NodeNewChild(sc2c, createPassive(texture, (Passive *) sc2c->data, SYELLOW, M_PI / 2), 0);
  
  Node * sc1cc = NodeNewChild(sc1c, createPassive(texture, (Passive *) sc1c->data, YELLOW, 5 * M_PI / 4), 1);
  Node * sc2cc = NodeNewChild(sc2c, createPassive(texture, (Passive *) sc2c->data, YELLOW, 7 * M_PI / 4), 1);
 
  NodeNewChild(sc1cc, createPassive(texture, (Passive *) sc1cc->data, SYELLOW, 5 * M_PI / 4), 0);
  NodeNewChild(sc2cc, createPassive(texture, (Passive *) sc2cc->data, SYELLOW, 7 * M_PI / 4), 0);
  // ---
  
  // ---
  Node * rc2 = NodeNewChild(root, createPassive(texture, (Passive *) root->data, SRED, M_PI / 2), 1);
  Node * wc1 = NodeNewChild(rc2, createPassive(texture, (Passive *) rc2->data, RED, M_PI / 2), 3);
  
  NodeNewChild(wc1, createPassive(texture, (Passive *) wc1->data, RED, 3 * M_PI / 4), 0);
  NodeNewChild(wc1, createPassive(texture, (Passive *) wc1->data, RED, M_PI / 4), 0);

  NodeNewChild(wc1, createPassive(texture, (Passive *) wc1->data, SRED, M_PI / 2), 0);
  // ---
  

  // ---
  Node * rc3 = NodeNewChild(root, createPassive(texture, (Passive *) root->data, SGREEN, M_PI), 1);
  // ---
  
  // ---
  Node * rc4 = NodeNewChild(root, createPassive(texture, (Passive *) root->data, SPURPLE, 2 * M_PI), 1);
  // ---
  
  PassiveTree passiveTree = {
    root,
    root,
    createSprite(renderer, "./assets/passive_pointer.bmp", 73, 65, 0, 0),
    { 0, 0, GAME_WIDTH, GAME_HEIGHT }
  };

  return passiveTree;
}

void updatePassiveTreeSelect(PassiveTree * passiveTree, char dir) {
  passiveTree->selected = getNearestPassive(passiveTree->selected, dir);
  
  Passive * current = (Passive *) passiveTree->selected->data;
  
  passiveTree->camera.x = (current->base.x + current->sprite.frame.w / 2) - passiveTree->camera.w / 2;
  passiveTree->camera.y = (current->base.y + current->sprite.frame.h / 2) - passiveTree->camera.h / 2;
  
  passiveTree->pointer.position.x = current->base.x - 1 - passiveTree->camera.x;
  passiveTree->pointer.position.y = current->base.y - 1 - passiveTree->camera.y;

  updatePassive(&passiveTree->camera, passiveTree->root);
}

void applyPassiveStatus(uint id, PlayerStatus * status) {
  switch (id) {
    case YELLOW: status->fish_spawn_quantity += 2; break;
    case BLUE: status->fish_spawn_time_decrease += 0.2; break;
    case GREEN: status->fish_gold_base += 1; break;
    case PURPLE: status->fish_gold_multiplier += 0.08; break;
    case RED: status->gold_passive_income += 5; break;

    case SYELLOW: status->fish_spawn_quantity += 5; status->fish_spawn_time_decrease += 0.1; break;
    case SBLUE: status->fish_spawn_time_decrease += 0.5; status->fish_spawn_quantity += 1; break;
    case SGREEN: status->fish_gold_base += 3; status->fish_gold_multiplier += 0.05; break;
    case SPURPLE: status->fish_gold_multiplier += 0.1; status->fish_gold_base += 1; break;
    case SRED: status->gold_passive_income += 12; break;
  }

  status->passive_price_multiplier += 0.05;
}

void activePassiveTreeSelect(PassiveTree * passiveTree, Player * player) {
  Passive * parent = passiveTree->selected->parent != NULL ? (Passive *) passiveTree->selected->parent->data : NULL;
  Passive * current = (Passive *) passiveTree->selected->data;
  int price = (int) ((current->id < SYELLOW ? 100 : 200) * player->status.passive_price_multiplier);

  if (player->gold >= price && !current->active && (parent == NULL || parent->active)) {
    player->gold -= price;
    current->active = 1;
    applyPassiveStatus(current->id, &player->status);
  }
}
