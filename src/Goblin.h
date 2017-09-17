#pragma once
#include "Actor.h"

class Goblin :
  public Actor {
public:
  Goblin(Tilemap* map, vec2i pos);
  ~Goblin();
  bool isTypeOf(Actors actor) { return actor == ACT_GOBLIN || Actor::isTypeOf(actor); };
  Actors Type() { return ACT_GOBLIN; }
};

