#include "Tilemap.h"
#include "vec2i.h"
#include "Actor.h"
#include "Renderer.h"
#include "Tileset.h"
#include "FieldOfView.h"
#include <SDL.h>

int Tilemap::GetIndex(int x, int y) 
{
  return y * width + x;
}

Tilemap::Tilemap(int width, int height)
{
  this->width = width;
  this->height = height;
  tilemap = new unsigned int[width*height]{0};
}

Tilemap::~Tilemap() 
{
  delete tilemap;
}

int Tilemap::get_width() 
{
  return width;
}

int Tilemap::get_height() 
{
  return height;
}

bool Tilemap::is_inside_bounds(int x, int y) 
{
  return x >= 0 && x < width && y >= 0 && y < height;
}

std::vector<vec2i> Tilemap::get_neighbours(int x, int y, int range) 
{
  std::vector<vec2i> neigh;
  if (range == 0) 
  {
    neigh.emplace_back(x,y);
    return neigh;
  }
  for (int dx = -range; dx <= range; dx++) 
  {
    for (int dy = -range; dy <= range; dy++) 
    {
      if ((dx != 0 || dy != 0) && is_inside_bounds(x + dx, y + dy)) 
      {
        neigh.emplace_back(x+dx,y+dy);
      }
    }
  }
  return neigh;
}

void Tilemap::set_tile(int x, int y, unsigned int tile)
{
  if (is_inside_bounds(x, y)) 
  {
    tilemap[GetIndex(x, y)] = tile;
  }
}

int Tilemap::get_tile(int x, int y) 
{
  if (is_inside_bounds(x, y)) 
  {
    return tilemap[GetIndex(x, y)];
  }
  return -1;
}

void Tilemap::draw(Renderer *renderer, Tileset* tileset, int x, int y, int tx, int ty, int tw, int th, FieldOfView* view) {
  int w = tileset->get_tile_width();
  int h = tileset->get_tile_height();
  for (int ix = 0; ix < tw; ix++) {
    for (int iy = 0; iy < th; iy++) {
      int ax = tx + ix;
      int ay = ty + iy;
      if (is_inside_bounds(ax, ay)) {
        if (view == nullptr || view->has_seen({ax, ay})) {
          renderer->set_color(1, 1, 1, 1);
          renderer->draw_sprite(tileset->get_sprite(get_tile(ax, ay)), x + ix * w, y + iy * h);

          if (view != nullptr && !view->can_see({ax, ay})) {
            renderer->set_color(0, 0, 0, .6f);
            renderer->draw_sprite(tileset->get_sprite(219), x + ix * w, y + iy * h);
          }
        }
      }
    }
  }
}