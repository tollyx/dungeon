#include "Actor.h"
#include "Tilemap.h"
#include "BehaviourTree.h"

Actor::Actor(Tilemap * map, vec2i pos) {
  this->map = map;
  position = pos;
}

const vec2i Actor::getPosition() {
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

void Actor::Update() {
  if (!alive) return;

  if (bt) {
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

Actor::~Actor() {
  if (bt != nullptr) {
    delete bt;
    bt = nullptr;
  }
}
