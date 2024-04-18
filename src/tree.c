#pragma once

#include "../lib/sprite.c"
#include "../lib/tree.c"
#include "./player.c"

#include "passive.c"

typedef struct PassiveTree {
  Node * root;
  Node * selected;
  SDL_Rect camera;
  Sprite pointer;
} PassiveTree;

Node * __getNearestPassive(Node * node, char dir) {
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
  Node * nearest_node = node;

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

void updatePassiveTreeSelect(PassiveTree * passiveTree, char dir) {
  passiveTree->selected = __getNearestPassive(passiveTree->selected, dir);
  
  Passive * current = (Passive *) passiveTree->selected->data;
  
  passiveTree->camera.x = (current->base.x + current->sprite.frame.w / 2) - passiveTree->camera.w / 2;
  passiveTree->camera.y = (current->base.y + current->sprite.frame.h / 2) - passiveTree->camera.h / 2;
  
  passiveTree->pointer.position.x = current->base.x - 1 - passiveTree->camera.x;
  passiveTree->pointer.position.y = current->base.y - 1 - passiveTree->camera.y;

  updatePassive(&passiveTree->camera, passiveTree->root);
}

void activePassiveTreeSelect(PassiveTree * passiveTree, Player * player) {
  Passive * parent = passiveTree->selected->parent != NULL ? (Passive *) passiveTree->selected->parent->data : NULL;
  Passive * current = (Passive *) passiveTree->selected->data;
  int price = (int)((current->id < SYELLOW ? 100 : 200) * player->status.passive_price_multiplier);

  if (player->gold >= price && !current->active && (parent == NULL || parent->active)) {
    player->gold -= price;
    current->active = 1;
    applyToPlayerStatus(current->id, player);
  }
}

/*  _       __  __ ___      _   ___ _   _  _ */
/* |_) /\  (_  (_   | \  / |_ __ | |_) |_ |_ */
/* |  /--\ __) __) _|_ \/  |_    | | \ |_ |_ */
/*                                           */
void createPassiveTree(SDL_Renderer * renderer, PassiveTree * tree, SDL_Texture * texture) {
  Passive * p = malloc(sizeof(Passive));

  p->id = YELLOW;
  p->active = 0;
  p->sprite = createSpriteTexture(texture, 74, 66, 14, 14);
  p->base = (SDL_Point) { p->sprite.position.x, p->sprite.position.y };

  Node * root = NodeNew(p, 4);

  Node * rc1 = createPassive(texture, root, SRED, R_90, 2);
  Node * rc2 = createPassive(texture, root, SBLUE, R_270, 2);
  Node * rc3 = createPassive(texture, root, SPURPLE, R_180, 2);
  Node * rc4 = createPassive(texture, root, SGREEN, R_360, 2);
 
  // RC1 -------------------------
  createPassive(texture, createPassive(texture, createPassive(texture, rc1, RED, R_45, 1), RED, R_90, 1), SRED, R_90, 0);
  
  Node * rc1y = createPassive(texture, createPassive(texture, createPassive(texture, rc1, YELLOW, R_135, 1), YELLOW, R_90, 1), SYELLOW, R_90, 3);
  createPassive(texture, rc1y, BLUE, R_45, 0);
  createPassive(texture, rc1y, SBLUE, R_90, 0);
  createPassive(texture, rc1y, BLUE, R_135, 0);
  // -----------------------------

  // RC2 -------------------------
  createPassive(texture, createPassive(texture, createPassive(texture, rc2, BLUE, R_315, 1), BLUE, R_270, 1), SBLUE, R_270, 0);

  Node * rc2y = createPassive(texture, createPassive(texture, createPassive(texture, rc2, YELLOW, R_225, 1), YELLOW, R_270, 1), SYELLOW, R_270, 3);
  createPassive(texture, rc2y, RED, R_315, 0);
  createPassive(texture, rc2y, SRED, R_270, 0);
  createPassive(texture, rc2y, RED, R_225, 0);
  // -----------------------------
  
  // RC3 -------------------------
  createPassive(texture, createPassive(texture, createPassive(texture, rc3, PURPLE, R_135, 1), PURPLE, R_180, 1), SPURPLE, R_180, 0);

  Node * rc3y = createPassive(texture, createPassive(texture, createPassive(texture, rc3, YELLOW, R_225, 1), YELLOW, R_180, 1), SYELLOW, R_180, 3);
  createPassive(texture, rc3y, GREEN, R_225, 0);
  createPassive(texture, rc3y, SGREEN, R_180, 0);
  createPassive(texture, rc3y, GREEN, R_135, 0);
  // -----------------------------
  
  // RC4 -------------------------
  createPassive(texture, createPassive(texture, createPassive(texture, rc4, GREEN, R_45, 1), GREEN, R_360, 1), SGREEN, R_360, 0);

  Node * rc4y = createPassive(texture, createPassive(texture, createPassive(texture, rc4, YELLOW, R_315, 1), YELLOW, R_360, 1), SYELLOW, R_360, 3);
  createPassive(texture, rc4y, PURPLE, R_315, 0);
  createPassive(texture, rc4y, SPURPLE, R_360, 0);
  createPassive(texture, rc4y, PURPLE, R_45, 0);
  // -----------------------------
  
  tree->root = root;
  tree->selected = root;
}
