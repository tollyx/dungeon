#pragma once

#include <vector>
#include "Entity.h"

class Tilemap;
class Tileset;

enum TileFlags : char {
  TILE_COLLISION = 1 << 0,
  TILE_OPAQUE = 1 << 1,
  TILE_WALKABLE = 1 << 2,
  TILE_WALL = 1 << 3,
  TILE_WATER = 1 << 4,
  // TILE_FLAG = 1 << 5,
  // TILE_FLAG = 1 << 6,
  // TILE_FLAG = 1 << 7,
};

class Level {
  Tilemap *glyphs;
  Tilemap *flags;
  Tileset *tiles;
  std::vector<Entity*> entities;
public:
  Level(int w, int h);
  ~Level();

  void add_entity(Entity *actor);
  void remove_entity(Entity *actor);

  int get_width();
  int get_height();

  Entity* get_entity(int x, int y, EntityTypes type);
  std::vector<Entity*> get_entities(int x, int y, int range, EntityTypes type);
  std::vector<Entity*>* get_entity_list();

  bool has_flags(TileFlags flags, int x, int y);
  bool is_blocked(int x, int y);
  Tilemap* get_glyphs_map() { return glyphs; }
  Tilemap* get_flags_map() { return flags; }
};

