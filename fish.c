#include "lib/entity.c"

typedef struct Fish { 
  int catch: 2;
} Fish;

void updateFish(Entity * entity, Fish * data) {
  entity->velocity.x = 1;
  updateEntity(entity);
}
