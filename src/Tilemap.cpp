#include "Tilemap.h"
#include "vec2i.h"
#include "Actor.h"
#include "Renderer.h"
#include "SpriteAtlas.h"
#include "FieldOfView.h"
#include <SDL2/SDL.h>

int Tilemap::get_index(int x, int y) {
  return y * width + x;
}

Tilemap::Tilemap(TileSet tileset, int width, int height) {
  this->width = width;
  this->height = height;
  tilemap = std::vector<std::string>(width*height, "");
  tiles = tileset;
}

Tilemap::~Tilemap() {}

int Tilemap::get_width() {
  return width;
}

int Tilemap::get_height() {
  return height;
}

bool Tilemap::is_inside_bounds(int x, int y) {
  return x >= 0 && x < width && y >= 0 && y < height;
}

std::vector<vec2i> Tilemap::get_neighbours(int x, int y, int range) {
  return get_neighbours(x, y, range, range, range, range);
}

std::vector<vec2i> Tilemap::get_neighbours(int x, int y, int up, int down, int left, int right) {
  std::vector<vec2i> neigh;
  for (int dx = -left; dx <= right; dx++) {
    for (int dy = -up; dy <= down; dy++) {
      if ((dx != 0 || dy != 0) && is_inside_bounds(x + dx, y + dy)) {
        neigh.emplace_back(x + dx, y + dy);
      }
    }
  }
  return neigh;
}

void Tilemap::set_tile(int x, int y, std::string tile) {
  if (is_inside_bounds(x, y)) {
    tilemap[get_index(x, y)] = tile;
  }
}

std::string Tilemap::get_tile_id(int x, int y) {
  if (is_inside_bounds(x, y)) {
    return tilemap[get_index(x, y)];
  }
  return "nil";
}

Tile const& Tilemap::get_tile(int x, int y) {
  return tiles.get_tile(get_tile_id(x,y));
}

bool Tilemap::is_blocked(int x, int y) {
  if (is_inside_bounds(x, y)) {
    if (!tiles.get_tile(tilemap[get_index(x, y)]).passable) {
      return true;
    }
    for (Entity* var : actors) {
      vec2i pos = var->get_position();
      if (var->has_collision() && pos == vec2i(x, y))
        return true;
    }
    return false;
  }
  return true;
}

void Tilemap::add_actor(Actor *actor) {
  for (Actor* var : actors) {
    if (var == actor) {
      return;
    }
  }
  actors.push_back(actor);
}

void Tilemap::remove_actor(Actor * actor) {
  for (auto it = actors.begin(); it != actors.end(); it++) {
    if ((*it) == actor) {
      actors.erase(it);
      return;
    }
  }
}

Actor * Tilemap::get_actor(int x, int y) {
  vec2i pos = { x,y };
  for (Actor* ent : actors) {
    vec2i apos = ent->get_position();
    if (apos == pos) {
      return ent;
    }
  }
  return nullptr;
}

std::vector<Actor*> Tilemap::get_actors(int x, int y, int range = 0) {
  std::vector<Actor*> found;
  std::vector<vec2i> neigh = get_neighbours(x, y, range);
  neigh.emplace_back(vec2i(x, y));
  for (Actor* ent : actors) {
    for (vec2i pos : neigh) {
      vec2i apos = ent->get_position();
      if (apos == pos) {
        found.emplace_back(ent);
        break;
      }
    }
  }
  return found;
}

std::vector<Actor*>* Tilemap::get_actor_list() {
  return &actors;
}

void Tilemap::delete_actors() {
  for (auto var : actors) {
    delete var;
  }
}

void Tilemap::draw(Renderer *renderer, SpriteAtlas* sprites, int x, int y, int tx, int ty, int tw, int th, FieldOfView* view) {
  int w = sprites->get_tile_width();
  int h = sprites->get_tile_height();
  for (int ix = 0; ix < tw; ix++) {
    for (int iy = 0; iy < th; iy++) {
      int ax = tx + ix;
      int ay = ty + iy;
      if (is_inside_bounds(ax, ay)) {
        if (view == nullptr || view->has_seen({ ax, ay })) {
          Tile t = get_tile(ax, ay);
          Color fg = t.fg;
          Color bg = t.bg;
          if (view != nullptr && !view->can_see({ ax, ay })) {
            fg.a = 0.4f;
            bg.a = 0.4f;
          }
          renderer->draw_sprite(sprites->get_sprite(t.glyph), fg, bg, x + ix * w, y + iy * h);
        }
      }
    }
  }
}

void Tilemap::debug_print() {
  /*
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      printf("\t%d", get_tile(x, y));
    }
    printf("\n");
  }
  */
}
