//
// Created by Adrian on 2017-09-21.
//

#ifndef DUNGEON_FIELDOFVIEW_H
#define DUNGEON_FIELDOFVIEW_H

#include "vec2i.h"
#include "Tilemap.h"

class FieldOfView {
  Tilemap* map;
  unsigned int counter;
  Tilemap seen;

  void cast_light(int row, float start, float end, int xx, int xy, int yx, int yy, int startX, int startY, float radius);
public:
  FieldOfView();
  FieldOfView(Tilemap* map);
  void calc(vec2i pos, float range);
  bool can_see(vec2i pos);
  bool has_seen(vec2i pos);
};
bool line_of_sight(Tilemap* map, vec2i start, vec2i end);

#endif //DUNGEON_FIELDOFVIEW_H
