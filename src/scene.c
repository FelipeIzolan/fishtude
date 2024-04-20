#include <sys/types.h>

enum Scenes { GAME, SKILL_TREE };

typedef struct Scene {
  uint current;
} Scene;
