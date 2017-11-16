#include "Level.h"
#include "Tilemap.h"



Level::Level(int w, int h) {
  glyphs = new Tilemap(w, h);
  flags = new Tilemap(w, h);
}


Level::~Level() {
  if(glyphs != nullptr) delete glyphs;
  glyphs = nullptr;
  if (flags != nullptr) delete flags;
  flags = nullptr;
  for (auto ent : entities) {
    delete ent;
  }
}


bool Level::has_flags(TileFlags f, int x, int y) {
  TileFlags a = (TileFlags)flags->get_tile(x, y);
  TileFlags b = (TileFlags)(a & f);

  return b == f;
}

bool Level::is_blocked(int x, int y) {
  if (glyphs->is_inside_bounds(x, y)) {
    for (Entity* var : entities) {
      vec2i pos = var->get_position();
      if (var->has_collision() && pos == vec2i(x, y))
        return true;
    }
    return has_flags(TILE_COLLISION, x, y);
  }
  return true;
}

void Level::add_entity(Entity *actor) {
  for (Entity* var : entities) {
    if (var == actor) {
      return;
    }
  }
  entities.push_back(actor);
}

void Level::remove_entity(Entity * actor) {
  for (auto it = entities.begin(); it != entities.end(); it++) {
    if ((*it) == actor) {
      entities.erase(it);
      return;
    }
  }
}

int Level::get_width() {
  return glyphs->get_width();
}

int Level::get_height() {
  return glyphs->get_height();
}

Entity * Level::get_entity(int x, int y, EntityTypes type) {
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

std::vector<Entity*> Level::get_entities(int x, int y, int range, EntityTypes type) {
  std::vector<Entity*> found;
  std::vector<vec2i> neigh = glyphs->get_neighbours(x, y, range);
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

std::vector<Entity*>* Level::get_entity_list() {
  return &entities;
}