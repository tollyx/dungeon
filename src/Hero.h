#pragma once
#include "Actor.h"

class Hero : public Actor {
public:
  Hero(Tilemap* map, vec2i pos);
  ~Hero();
  bool isTypeOf(Actors actor) { return actor == ACT_HERO || Actor::isTypeOf(actor); };
  Actors Type() { return ACT_HERO; }
};

