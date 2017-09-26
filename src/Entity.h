//
// Created by Adrian on 2017-09-25.
//

#ifndef DUNGEON_ENTITY_H
#define DUNGEON_ENTITY_H


#include "vec2i.h"

class Tilemap;

enum EntityTypes {
  ENTITY_BASE, // All entities are objects that can be placed on the map and can be interacted with
  ENTITY_ACTOR, // Actors are characters with AI or controlled by the player
  ENTITY_ITEM, // Items can be picked up
};

class Entity {
  vec2i position;
  Tilemap* map;
protected:
  unsigned int sprite_id;
  bool collision;
public:
  Entity(Tilemap* map, vec2i pos);

  Tilemap* get_map();
  vec2i get_position();
  bool has_collision();
  bool move(int dx, int dy); // returns false if movement failed
  bool move(vec2i dpos);
  void set_position(vec2i pos);
  unsigned int get_sprite_id() { return sprite_id; };
  virtual EntityTypes entity_type() { return ENTITY_BASE; };
};


#endif //DUNGEON_ENTITY_H
