#include "Actor.h"
#include "Tilemap.h"
#include "BehaviourTree.h"

int idcounter = 0;

Actor::Actor(Tilemap * map, vec2i pos) {
  id = idcounter++;
  name = "Actor";
  this->map = map;
  position = pos;
  bt = nullptr;
}

const vec2i Actor::get_position() {
  return position;
}

bool Actor::Move(int dx, int dy) {
  vec2i newpos = position + vec2i(dx, dy); //GoTo({0,0}, {dx,dy});
  //dir = ParseDir(dx, dy);
  if (!map->IsBlocked(newpos.x, newpos.y)) {
    position = newpos;
    return true;
  }
  return false;
}

void Actor::update() {
  if (!alive) return;

  if (bt != nullptr) {
    bt->tick(this);
  }
  if (health < maxhealth) {
    if (healcounter <= 0) {
      health++;
      healcounter = 4;
    }
    healcounter--;
  }

  if (health <= 0) {
    Kill();
  }
}

Actor::~Actor() = default;
