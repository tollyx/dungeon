#include "Actor.h"
#include "Tilemap.h"
#include "BehaviourTree.h"
#include "FieldOfVision.h"

void Actor::CalcFOV() {
  FOV::DoFOV(map, vision, ++visioncounter, position.x, position.y, 6);
}

Actor::Actor(Tilemap * map, vec2i pos) {
  this->map = map;
  position = pos;
  vision = new Tilemap(map->GetWidth(), map->GetHeight());
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
    CalcFOV();
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

bool Actor::HasSeen(int x, int y) {
  return vision->GetTile(x,y) != 0;
}

bool Actor::CanSee(int x, int y) {
  return vision->GetTile(x,y) == visioncounter;
}

Actor::~Actor() {
  if (bt != nullptr) {
    delete bt;
    bt = nullptr;
  }
}
