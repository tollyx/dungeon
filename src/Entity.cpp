//
// Created by Adrian on 2017-09-25.
//

#include "Entity.h"
#include "Tilemap.h"

Entity::Entity(vec2i pos) {
  position = pos;
  collision = false;
  sprite_id = '?';
}

vec2i Entity::get_position() {
  return position;
}

bool Entity::move(vec2i dpos, Tilemap* map) {
  return move(dpos.x, dpos.y, map);
}

bool Entity::move(int dx, int dy, Tilemap* map) {
  vec2i newpos = position + vec2i(dx, dy);
  if (!collision || !map->is_blocked(newpos.x, newpos.y)) {
    position = newpos;
    return true;
  }
  return false;
}

void Entity::set_position(vec2i pos) {
  position = pos;
}

bool Entity::has_collision() {
  return collision;
}
