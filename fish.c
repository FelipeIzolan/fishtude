#pragma once

#include "lib/cvector.h"
#include "lib/entity.c"

#include "lib/math.c"
#include "fishing.c"

typedef struct Fish { 
  int catch: 2;
  int price: 16;
} Fish;

void updateFish(Entity * entity, Fish * data, Entity * player, Fishing * fishing, Entity * efish, Fish * dfish, int index) {
  if (!data->catch) {
    
    entity->position.x += 1;
    
    if (checkEntityAndPointCollision(entity, &fishing->end)) {
      data->catch = 1;
    }
  } else {
    entity->position.x = LERP(entity->position.x, fishing->end.x - 4 + cos(entity->position.y), 0.4);
    entity->position.y = LERP(entity->position.y, fishing->end.y - 4 + sin(entity->position.x), 0.4);
  
    if (checkEntityCollision(entity, player)) {
      cvector_erase(efish, index);
      cvector_erase(dfish, index);
    };
  }
  
  if (entity->position.x >= 168) entity->position.x = -8;
}
