#include <stdio.h>
#include <stdlib.h>

#include <SDL.h>
#include <SDL_timer.h>
#include <SDL_error.h>
#include <SDL_video.h>
#include <SDL_render.h>
#include <SDL_events.h>
#include <SDL_stdinc.h>
#include <SDL_keycode.h>

#include "lib/animation.c"
#include "lib/sprite.c"
#include "lib/entity.c"
#include "lib/utils.c"
#include "lib/math.c"

#include "player.c"
#include "fishing.c"

void error() {
  fprintf(stderr, "SDL_Init -> %s\n", SDL_GetError());
  exit(EXIT_FAILURE);
}

void framerate(int start) {
  static int s;

  if (start) {
    s = SDL_GetTicks();
  } else {
    int e = SDL_GetTicks() - s;
    int dd = DELTA * 1000;
    if (e < dd) SDL_Delay(dd - e);
  }
}

int main() {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) error();

  SDL_Window * window = SDL_CreateWindow(
      "fishtude", 
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      WINDOW_WIDTH,
      WINDOW_HEIGHT,
      SDL_WINDOW_FULLSCREEN_DESKTOP |
      SDL_WINDOW_BORDERLESS
  );
  if (window == NULL) error();

  SDL_Renderer * renderer = SDL_CreateRenderer(
      window,
      -1,
      SDL_RENDERER_ACCELERATED |
      SDL_RENDERER_PRESENTVSYNC
  );
  if (renderer == NULL) error();

  // Keep size and scale
  SDL_RenderSetLogicalSize(renderer, WINDOW_WIDTH, WINDOW_HEIGHT);
  SDL_RenderSetIntegerScale(renderer, SDL_TRUE);

  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  
  SDL_Rect water = { 0, 36, WINDOW_WIDTH, WINDOW_HEIGHT};

  Sprite background = createSprite(renderer, "./sprites/background.bmp", 0, 0, 0, 0);

  Sprite fishing_mechanic = createSprite(renderer, "./sprites/fishing_mechanic.bmp", 0, 0, 0, 0); 
  Sprite fishing_pointer = createSprite(renderer, "./sprites/fishing_pointer.bmp", 0, 0, 0, 0);
  Sprite fishing_hook = createSprite(renderer, "./sprites/fishing_hook.bmp", 0, 0, 0, 0);

  Fishing fishing = { { 14, 30 }, { 14, 30 }, { -8, 8, -8, 2 }, 0, 1 };
  
  Entity player = createEntity(renderer, "./sprites/player.bmp", 0, 24, 16, 16);
  Player playerD = { PLAYER_NORMAL, 0 };

  SDL_Texture * fish_sprite = createTexture(renderer, "./sprites/fish.bmp");
  Entity fish[128]; // <- SET MAX_FISH HERE!
 
  fish[0] = createEntityTexture(fish_sprite, 2, 64, 8, 8);
  setFrame(&fish[0].animation, 1, 0);

  // Game-Loop
  while (1) { 
    framerate(1);
    
    SDL_Event event;
    const Uint8* keyboard = SDL_GetKeyboardState(NULL);

    while(SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT: goto game_free;
        case SDL_KEYDOWN: 
          if (event.key.keysym.sym == SDLK_ESCAPE) goto game_free; 
          if (event.key.keysym.sym == SDLK_x) {
            playerD.state = playerD.state == PLAYER_NORMAL ? PLAYER_PRE_FISHING : playerD.state == PLAYER_PRE_FISHING ? PLAYER_FISHING : PLAYER_NORMAL;
            
            if (playerD.state == PLAYER_PRE_FISHING) {
              fishing.force = 0;

              fishing_mechanic.position.x = player.position.x + 20;
              fishing_mechanic.position.y = player.position.y - 10;
              fishing_pointer.position.x = player.position.x + 15;
              fishing_pointer.position.y = player.position.y + 18;
              
              fishing.start.x = player.position.x + 14;
              fishing.start.y = player.position.y + 6;
              fishing.end = fishing.start;
/*              fishing_hook.position.x = fishing_end.x - 2;*/
            }

            if (playerD.state == PLAYER_FISHING)
              fishing.force = floorf(((float)fishing.force / 28) * 108) + 2;

          }
        break;
      }
    }

    updatePlayer(keyboard, &player, &playerD);
    updateFishing(&fishing, &playerD);    

    // Render
    SDL_RenderClear(renderer);   

    drawSprite(renderer, &background);
    drawEntity(renderer, &player);
    
    if (playerD.state == PLAYER_FISHING || playerD.state == PLAYER_FISHING_BACK) {
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
      SDL_Point midpoint = lineMidpoint(fishing.start, fishing.end);
      SDL_Point p1 = lineMidpoint(fishing.start, midpoint);
      SDL_Point p2 = lineMidpoint(midpoint, fishing.end);
     
      p1.x += fishing.control.current;
      p2.x += -fishing.control.current;

      p1.x += cos(p1.y) * 5;
      p1.y += sin(p1.x) * 5;

      for (float i = 0; i < 1; i += 0.001) {
        SDL_Point p = cubicBenzierCurve(fishing.start, p1, p2, fishing.end, i);
        SDL_RenderDrawPoint(renderer, p.x, p.y);
      }
    }

    drawEntity(renderer, &fish[0]);

    SDL_SetRenderDrawColor(renderer, 15, 94, 156, 100);
    SDL_RenderFillRect(renderer, &water);
    
    if (playerD.state == PLAYER_PRE_FISHING) {
      drawSprite(renderer, &fishing_mechanic);
      drawSprite(renderer, &fishing_pointer);
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderPresent(renderer);

    framerate(0);
  }

  // Free
  game_free:

  destroyEntity(&player);

  destroySprite(&fishing_hook);
  destroySprite(&fishing_mechanic);
  destroySprite(&fishing_pointer);
  destroySprite(&background);

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  
  SDL_Quit();

  return 1;
}
