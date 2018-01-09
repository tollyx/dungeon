#pragma once

#include "vec2i.h"
#include "Color.h"

class Tilemap;

enum EntityTypes {
  ENTITY_BASE, // All entities are objects that can be placed on the map and can be interacted with
  ENTITY_ACTOR, // Actors are characters with AI or controlled by the player
  ENTITY_ITEM, // Items can be picked up
};

class Entity {
  vec2i position;
protected:
  unsigned int sprite_id;
  Color sprite_color;
  bool collision;
public:
  Entity(vec2i pos);

  vec2i get_position();
  bool has_collision();
  bool move(int dx, int dy, Tilemap* map); // returns false if movement failed
  bool move(vec2i dpos, Tilemap* map);
  void set_position(vec2i pos);
  unsigned int get_sprite_id() { return sprite_id; };
  Color get_sprite_color() { return sprite_color; };
  virtual EntityTypes entity_type() { return ENTITY_BASE; };
};
