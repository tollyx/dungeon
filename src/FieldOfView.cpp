//
// Created by Adrian on 2017-09-21.
//

#include <cmath>
#include "FieldOfView.h"
#include "Tilemap.h"
#include <SDL2/SDL.h>

FieldOfView::FieldOfView() {
}

FieldOfView::FieldOfView(Tilemap *map) {
  this->map = map;
  seen = Tilemap(map->get_width(), map->get_height());
  counter = 0;
}

void FieldOfView::calc(vec2i pos, float range) {
  counter++;
  seen.set_tile(pos.x, pos.y, counter);
  // Once for each octant
  if (map != nullptr) {
    cast_light(1, 1.0f, 0.0f, 0, -1, -1, 0, pos.x, pos.y, range);
    cast_light(1, 1.0f, 0.0f, -1, 0, 0, -1, pos.x, pos.y, range);
    cast_light(1, 1.0f, 0.0f, 0, 1, -1, 0, pos.x, pos.y, range);
    cast_light(1, 1.0f, 0.0f, 1, 0, 0, -1, pos.x, pos.y, range);
    cast_light(1, 1.0f, 0.0f, 0, -1, 1, 0, pos.x, pos.y, range);
    cast_light(1, 1.0f, 0.0f, -1, 0, 0, 1, pos.x, pos.y, range);
    cast_light(1, 1.0f, 0.0f, 0, 1, 1, 0, pos.x, pos.y, range);
    cast_light(1, 1.0f, 0.0f, 1, 0, 0, 1, pos.x, pos.y, range);
  }
  else {
    SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "Tried to calc fov with a null tilemap!\n");
  }
}

bool FieldOfView::can_see(vec2i pos) {
  return seen.get_tile(pos.x, pos.y) >= counter;
}

bool FieldOfView::has_seen(vec2i pos) {
  return seen.get_tile(pos.x, pos.y) > 0;
}

void FieldOfView::cast_light(int row, float start, float end, int xx, int xy, int yx, int yy, int startX, int startY,
                             float radius) {
  float newStart = 0.0f;
  if (start < end) {
    return;
  }
  bool blocked = false;
  for (int distance = row; distance <= radius && !blocked; distance++) {
    int deltaY = -distance;
    for (int deltaX = -distance; deltaX <= 0; deltaX++) {
      int currentX = startX + deltaX * xx + deltaY * xy;
      int currentY = startY + deltaX * yx + deltaY * yy;
      float leftSlope = (deltaX - 0.5f) / (deltaY + 0.5f);
      float rightSlope = (deltaX + 0.5f) / (deltaY - 0.5f);

      if (!(currentX >= 0 && currentY >= 0 && currentX < map->get_width() && currentY < map->get_height()) || start < rightSlope) {
        continue;
      }
      else if (end > leftSlope) {
        break;
      }

      if (sqrt(deltaX*deltaX + deltaY*deltaY) <= radius) {
        seen.set_tile(currentX, currentY, counter);
      }

      if (blocked) {
        if (map->get_tile(currentX, currentY) == '#') { // TODO: Stop hardcoding tiles
          newStart = rightSlope;
          continue;
        }
        else {
          blocked = false;
          start = newStart;
        }
      }
      else {
        if (map->get_tile(currentX, currentY) == '#' && distance < radius) { // TODO: Get rid of hardcoded tiles
          blocked = true;
          cast_light(distance + 1, start, leftSlope, xx, xy, yx, yy, startX, startY, radius);
          newStart = rightSlope;
        }
      }
    }
  }
}

bool line_of_sight(Tilemap *map, vec2i start, vec2i end) {
  if (start == end) return true;
  vec2i delta = end - start;
  const char ix = (delta.x > 0) - (delta.x < 0);
  const char iy = (delta.y > 0) - (delta.y < 0);
  delta.x = std::abs(delta.x) << 1;
  delta.y = std::abs(delta.y) << 1;

  if (delta.x >= delta.y)
  {
    // error may go below zero
    int error(delta.y - (delta.x >> 1));

    while (start.x != end.x && map->get_tile(start.x, start.y) != '#') // TODO: Hardcoded tiles
    {
      // reduce error, while taking into account the corner case of error == 0
      if ((error > 0) || (!error && (ix > 0)))
      {
        error -= delta.x;
        start.y += iy;
      }
      // else do nothing

      error += delta.y;
      start.x += ix;
    }
  }
  else
  {
    // error may go below zero
    int error(delta.x - (delta.y >> 1));

    while (start.y != end.y && map->get_tile(start.x, start.y) != '#') // TODO: Stop hardcoding tiles
    {
      // reduce error, while taking into account the corner case of error == 0
      if ((error > 0) || (!error && (iy > 0)))
      {
        error -= delta.y;
        start.x += ix;
      }
      // else do nothing

      error += delta.x;
      start.y += iy;
    }
  }
  return start == end;
}
