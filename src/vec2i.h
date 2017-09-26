#pragma once

#include <cmath>

struct vec2i {
  int x, y;

  vec2i() {
    this->x = 0;
    this->y = 0;
  }

  vec2i(int x, int y) {
    this->x = x;
    this->y = y;
  }

  double dist() {
    return sqrt(dist_squared());
  }

  int dist_squared() {
    return x*x + y*y;
  }

  bool operator==(vec2i a) {
    return a.x == x && a.y == y;
  }

  bool operator!=(vec2i a) {
    return a.x != x || a.y != y;
  }

  vec2i operator+(vec2i a) {
    return { x + a.x, y + a.y };
  }

  vec2i operator-(vec2i a) {
    return { x - a.x, y - a.y };
  }

  vec2i operator*(vec2i a) {
    return { x*a.x, y*a.y };
  }

  vec2i operator*(int a) {
    return{ x*a, y*a };
  }
};