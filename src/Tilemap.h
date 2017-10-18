#pragma once
#include <vector>
#include "Tileset.h"
#include "Entity.h"

struct vec2i;
class Renderer;
class FieldOfView;

class Tilemap {
  unsigned int* tilemap;
  std::vector<Entity*> entities;
  int width;
  int height;
public:
  Tilemap(int width = 1, int height = 1);
  ~Tilemap();
  int GetWidth();
  int GetHeight();
  int GetIndex(int x, int y); // Converts [x,y] to a 1D index.
  bool IsInsideBounds(int x, int y);
  std::vector<vec2i> get_neighbours(int x, int y, int range = 1);
  void set_tile(int x, int y, unsigned int tile); // "Tile" is inteded for tile ids, but can be anything really.
  int get_tile(int x, int y);
  bool IsBlocked(int x, int y); // Checks if there is an actor blocking the tile.

  void draw(Renderer *renderer, Tileset *tileset, int x, int y, int tx, int ty, int tw, int th, FieldOfView* view);

  void add_entity(Entity *actor);
  void remove_entity(Entity *actor);

  void debug_print();

  Entity* get_entity(int x, int y, EntityTypes type);
  std::vector<Entity*> get_entities(int x, int y, int range, EntityTypes type);
  std::vector<Entity*>* get_entity_list();
};

