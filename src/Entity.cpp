//
// Created by Adrian on 2017-09-25.
//

#include "Entity.h"
#include "Tilemap.h"

Entity::Entity(Tilemap *map, vec2i pos) {
  this->map = map;
  position = pos;
  collision = false;
  sprite_id = '?';
}

vec2i Entity::get_position() {
  return position;
}

bool Entity::move(vec2i dpos) {
  return move(dpos.x, dpos.y);
}

bool Entity::move(int dx, int dy) {
  vec2i newpos = position + vec2i(dx, dy); //GoTo({0,0}, {dx,dy});
  //dir = ParseDir(dx, dy);
  if (!collision || !map->IsBlocked(newpos.x, newpos.y)) {
    position = newpos;
    return true;
  }
  return false;
}

void Entity::set_position(vec2i pos) {
  position = pos;
}

Tilemap *Entity::get_map() {
  return map;
}

bool Entity::has_collision() {
  return collision;
}
