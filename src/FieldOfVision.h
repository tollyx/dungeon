#pragma once

class Tilemap;

namespace FOV {
  void DoFOV(Tilemap* map, Tilemap* vision, int visionvalue, int x, int y, float radius);
  void CastLight(Tilemap* map, Tilemap* vision, int visioncounter, int row, float start, float end, int xx, int xy, int yx, int yy, int startX, int startY, float radius);
}