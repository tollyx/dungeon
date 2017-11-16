#pragma once
#include "Actor.h"
class Shaman :
  public Actor {
public:
  Shaman(Level* map, vec2i pos);
  ~Shaman();
  bool is_type_of(Actors actor) override { return actor == ACT_SHAMAN || Actor::is_type_of(actor); };
  Actors type() override { return ACT_SHAMAN; }
};

