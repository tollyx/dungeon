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
  tilemap = std::vector<unsigned int>(width*height, 0);
}

Tilemap::~Tilemap() 
{
  for (auto var : entities) {
    delete var;
  }
}

int Tilemap::GetWidth() 
{
  return width;
}

int Tilemap::GetHeight() 
{
  return height;
}

bool Tilemap::IsInsideBounds(int x, int y) 
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
      if ((dx != 0 || dy != 0) && IsInsideBounds(x + dx, y + dy)) 
      {
        neigh.emplace_back(x+dx,y+dy);
      }
    }
  }
  return neigh;
}

void Tilemap::set_tile(int x, int y, unsigned int tile)
{
  if (IsInsideBounds(x, y)) 
  {
    tilemap[GetIndex(x, y)] = tile;
  }
}

int Tilemap::get_tile(int x, int y) 
{
  if (IsInsideBounds(x, y)) 
  {
    return tilemap[GetIndex(x, y)];
  }
  return -1;
}

bool Tilemap::IsBlocked(int x, int y) 
{
  if (IsInsideBounds(x, y)) 
  {
    if (tilemap[GetIndex(x,y)] == '#') { // TODO: Replace hardcoded tiles
      return true;
    }
    for (Entity* var : entities) {
      vec2i pos = var->get_position();
      if (var->has_collision() && pos == vec2i(x, y))
        return true;
    }
    return false;
  }
  return true;
}

void Tilemap::add_entity(Entity *actor) {
  for (Entity* var : entities) {
    if (var == actor) {
      return;
    }
  }
  entities.push_back(actor);
}

void Tilemap::remove_entity(Entity * actor) {
  for (auto it = entities.begin(); it != entities.end(); it++) {
    if ((*it) == actor) {
      entities.erase(it);
      return;
    }
  }
}

Entity * Tilemap::get_entity(int x, int y, EntityTypes type) {
  vec2i pos = { x,y };
  for (Entity* ent : entities) {
    if (ent->entity_type() == type) {
      vec2i apos = ent->get_position();
      if (apos == pos) {
        return ent;
      }
    }
  }
  return nullptr;
}

std::vector<Entity*> Tilemap::get_entities(int x, int y, int range, EntityTypes type) {
  std::vector<Entity*> found;
  std::vector<vec2i> neigh = get_neighbours(x, y, range);
  for (Entity* ent : entities) {
    for (vec2i pos : neigh) {
      if (ent->entity_type() == type) {
        vec2i apos = ent->get_position();
        if (apos == pos) {
          found.emplace_back(ent);
          break;
        }
      }
    }
  }
  return found;
}

std::vector<Entity*>* Tilemap::get_entity_list() {
  return &entities;
}

void Tilemap::draw(Renderer *renderer, Tileset* tileset, int x, int y, int tx, int ty, int tw, int th, FieldOfView* view) {
  int w = tileset->get_tile_width();
  int h = tileset->get_tile_height();
  for (int ix = 0; ix < tw; ix++) {
    for (int iy = 0; iy < th; iy++) {
      int ax = tx + ix;
      int ay = ty + iy;
      if (IsInsideBounds(ax, ay)) {
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

void Tilemap::debug_print() {
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      printf("\t%d", get_tile(x, y));
    }
    printf("\n");
  }
}
