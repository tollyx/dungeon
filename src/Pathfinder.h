#pragma once

#include "vec2i.h"
#include <vector>

class Level;

namespace Pathfinder
{
  struct AStarNode
  {
    vec2i pos;
    float f, g, h = 0;
    AStarNode* from = nullptr;
  };

  struct DijkstraMap {
    float* tilemap = nullptr;
    int width, height;

    float getValue(int x, int y) {
      if (x >= 0 && x < width && y >= 0 && y < height) {
        return tilemap[y*width + x];
      }
      else {
        return 99999999;
      }
    }

    void setValue(int x, int y, float val) {
      if (x >= 0 && x < width && y >= 0 && y < height) {
        tilemap[y*width + x] = val;
      }
    }

    void mult(float val) {
      for (int i = 0; i < width * height; i++) {
        tilemap[i] *= val;
      }
    }

    void add(float val) {
      for (int i = 0; i < width * height; i++) {
        tilemap[i] += val;
      }
    }

    void abs() {
      for (int i = 0; i < width * height; i++) {
        if (tilemap[i] < 0) {
          tilemap[i] *= -1;
        }
      }
    }

    void clamp(float min, float max) {
      for (int i = 0; i < width * height; i++) {
        if (tilemap[i] < min) {
          tilemap[i] = min;
        }
        else if (tilemap[i] > max) {
          tilemap[i] = max;
        }
      }
    }
  };

  float distance(vec2i a, vec2i b);
  std::vector<vec2i> aStar(Level* map, vec2i start, vec2i goal);
  void calcDijkstraMap(Level* map, std::vector<vec2i>* goals, DijkstraMap* out, float maxValue = 32);
}