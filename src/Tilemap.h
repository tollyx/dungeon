#pragma once
#include <vector>
#include "Tileset.h"
#include "Entity.h"

struct vec2i;
class Renderer;
class FieldOfView;

class Tilemap {
  unsigned int* tilemap;
  int width;
  int height;
public:
  Tilemap(int width = 1, int height = 1);
  ~Tilemap();
  int get_width();
  int get_height();
  int GetIndex(int x, int y); // Converts [x,y] to a 1D index.
  bool is_inside_bounds(int x, int y);
  std::vector<vec2i> get_neighbours(int x, int y, int range = 1);
  void set_tile(int x, int y, unsigned int tile); // "Tile" is inteded for tile ids, but can be anything really.
  int get_tile(int x, int y);

  void draw(Renderer *renderer, Tileset *tileset, int x, int y, int tx, int ty, int tw, int th, FieldOfView* view);
};

