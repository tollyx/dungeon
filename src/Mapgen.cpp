#include "Mapgen.h"

Tilemap generate_level(int seed, int width, int height) {
  Tilemap map = Tilemap(width, height);

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      if (x == 0 || x == width-1 || y == 0 || y == height-1) {
        map.set_tile(x,y, '#');
      }
      else {
        map.set_tile(x,y, '.');
      }
    }
  }

  return map;
}
