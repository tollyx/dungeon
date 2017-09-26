#pragma once
#include "Actor.h"

class Hero : public Actor {
public:
  Hero(Tilemap* map, vec2i pos);
  ~Hero();
  bool is_type_of(Actors actor) { return actor == ACT_HERO || Actor::is_type_of(actor); };
  Actors type() { return ACT_HERO; }
};

