#include "FieldOfVision.h"
#include "Tilemap.h"

namespace FOV {

  void DoFOV(Tilemap* map, Tilemap* vision, int visionvalue, int x, int y, float radius) {
    vision->SetTile(x, y, visionvalue);
    // Once for each octant
    CastLight(map, vision, visionvalue, 1, 1.0f, 0.0f, 0, -1, -1, 0, x, y, radius);
    CastLight(map, vision, visionvalue, 1, 1.0f, 0.0f, -1, 0, 0, -1, x, y, radius);
    CastLight(map, vision, visionvalue, 1, 1.0f, 0.0f, 0, 1, -1, 0, x, y, radius);
    CastLight(map, vision, visionvalue, 1, 1.0f, 0.0f, 1, 0, 0, -1, x, y, radius);
    CastLight(map, vision, visionvalue, 1, 1.0f, 0.0f, 0, -1, 1, 0, x, y, radius);
    CastLight(map, vision, visionvalue, 1, 1.0f, 0.0f, -1, 0, 0, 1, x, y, radius);
    CastLight(map, vision, visionvalue, 1, 1.0f, 0.0f, 0, 1, 1, 0, x, y, radius);
    CastLight(map, vision, visionvalue, 1, 1.0f, 0.0f, 1, 0, 0, 1, x, y, radius);
  }

  void CastLight(Tilemap* map, Tilemap* vision, int visioncounter, int row, float start, float end, int xx, int xy, int yx, int yy, int startX, int startY, float radius) {
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

        if (!(currentX >= 0 && currentY >= 0 && currentX < map->GetWidth() && currentY < map->GetHeight()) || start < rightSlope) {
          continue;
        }
        else if (end > leftSlope) {
          break;
        }

        if (sqrt(deltaX*deltaX + deltaY*deltaY) <= radius) {
          vision->SetTile(currentX, currentY, visioncounter);
        }

        if (blocked) {
          if (map->GetTile(currentX, currentY) == '#') { // TODO: Stop hardcoding tiles
            newStart = rightSlope;
            continue;
          }
          else {
            blocked = false;
            start = newStart;
          }
        }
        else {
          if (map->GetTile(currentX, currentY) == '#' && distance < radius) { // TODO: Get rid of hardcoded tiles
            blocked = true;
            CastLight(map, vision, visioncounter, distance + 1, start, leftSlope, xx, xy, yx, yy, startX, startY, radius);
            newStart = rightSlope;
          }
        }
      }
    }
  }
}
