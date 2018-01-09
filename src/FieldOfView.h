#pragma once

#include "vec2i.h"
#include "Tilemap.h"
#include <vector>

class FieldOfView {
  Tilemap* map;
  unsigned int counter = 0;
  std::vector<unsigned int> seen;

  void cast_light(int row, float start, float end, int xx, int xy, int yx, int yy, int startX, int startY, float radius);
public:
  unsigned int seen_cutoff = 0;
  FieldOfView();
  FieldOfView(Tilemap* map);
  void calc(vec2i pos, float range);
  bool can_see(vec2i pos);
  bool has_seen(vec2i pos);
};
bool line_of_sight(Tilemap* map, vec2i start, vec2i end);
