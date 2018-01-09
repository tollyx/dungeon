#pragma once
#include <array>
#include <vector>
#include "SpriteAtlas.h"
#include "Actor.h"
#include "TileSet.h"

struct vec2i;
class Renderer;
class FieldOfView;

class Tilemap {
  std::vector<std::string> tilemap;
  std::vector<Actor*> actors;
  int width;
  int height;
public:
  Tilemap(TileSet tileset = TileSet(), int width = 1, int height = 1);
  TileSet tiles;
  ~Tilemap();
  int get_width();
  int get_height();
  int get_index(int x, int y); // Converts [x,y] to a 1D index.
  bool is_inside_bounds(int x, int y);
  std::vector<vec2i> get_neighbours(int x, int y, int range = 1);
  std::vector<vec2i> get_neighbours(int x, int y, int up, int down, int left, int right);
  void set_tile(int x, int y, std::string tile); // "Tile" is inteded for tile ids, but can be anything really.
  std::string get_tile_id(int x, int y);
  Tile const& get_tile(int x, int y);
  bool is_blocked(int x, int y); // Checks if there is an actor blocking the tile.

  void draw(Renderer *renderer, SpriteAtlas *tileset, int x, int y, int tx, int ty, int tw, int th, FieldOfView* view);

  void add_actor(Actor *actor);
  void remove_actor(Actor *actor);

  void debug_print();

  Actor* get_actor(int x, int y);
  std::vector<Actor*> get_actors(int x, int y, int range);
  std::vector<Actor*>* get_actor_list();
  void delete_actors();
};

