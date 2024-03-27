#pragma once

#include <math.h>
#include <stdlib.h>

#include <SDL_rect.h>

#define GAME_WIDTH 160
#define GAME_HEIGHT 144

#define LERP(s, e, w) (s + (e-s) * w)
#define MIN(v, min) (v < min ? min : v)
#define MAX(v, max) (v > max ? max : v)
#define CLAMP(v, min, max) (MIN(min, MAX(v, max)))
#define ICLAMP(v, min, max) (v > max ? min : v < min ? max : v)
#define MID(s, e) ((s + e) / 2)

#define R_45 M_PI / 4
#define R_90 M_PI / 2
#define R_135 3 * M_PI / 4
#define R_180 M_PI
#define R_225 5 * M_PI / 4
#define R_270 3 * M_PI / 2
#define R_315 7 * M_PI / 4
#define R_360 2 * M_PI

SDL_Point quadraticBezierCurve(SDL_Point p1, SDL_Point p2, SDL_Point p3, float w) {
  float x0 = LERP(p1.x, p2.x, w);
  float y0 = LERP(p1.y, p2.y, w);
  float x1 = LERP(p2.x, p3.x, w);
  float y1 = LERP(p2.y, p3.y, w);
    
  return (SDL_Point) { LERP(x0, x1, w),LERP(y0, y1, w) };
}

int distance(int x1, int y1, int x2, int y2) {
  return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

SDL_Point cubicBenzierCurve(SDL_Point p1, SDL_Point p2, SDL_Point p3, SDL_Point p4, float w) {
  SDL_Point v1 = quadraticBezierCurve(p1, p2, p3, w);
  SDL_Point v2 = quadraticBezierCurve(p2, p3, p4, w);

  return (SDL_Point) { LERP(v1.x, v2.x, w), LERP(v1.y, v2.y, w) };
}

SDL_Point lineMidpoint(SDL_Point s, SDL_Point e) {
  return (SDL_Point) { MID(s.x, e.x), MID(s.y, e.y) };
}

int rrandom(int min, int max) {
  return rand() % (max - min + 1) + min;
}

double drandom() {
  return (double) rand() / RAND_MAX;
}
