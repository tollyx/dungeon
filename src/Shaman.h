#pragma once
#include "Actor.h"
class Shaman :
  public Actor {
public:
  Shaman(Tilemap* map, vec2i pos);
  ~Shaman();
  bool isTypeOf(Actors actor) { return actor == ACT_SHAMAN || Actor::isTypeOf(actor); };
  Actors Type() { return ACT_SHAMAN; }
};

