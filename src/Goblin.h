#pragma once
#include "Actor.h"

class Goblin :
  public Actor {
public:
  Goblin(Tilemap* map, vec2i pos);
  ~Goblin();
  bool is_type_of(Actors actor) { return actor == ACT_GOBLIN || Actor::is_type_of(actor); };
  Actors type() { return ACT_GOBLIN; }
};

