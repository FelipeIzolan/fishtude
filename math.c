#pragma once

#define MIN(v, min) (v < min ? min : v)
#define MAX(v, max) (v > max ? max : v)
#define CLAMP(v, min, max) (MIN(min, MAX(v, max)))
