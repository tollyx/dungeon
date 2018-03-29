#pragma once

#include "Actor.h"

class Item : Entity {
  Item() {};
public:
  virtual bool consume(Actor& consumer) = 0;
  virtual bool equip(Actor& equipper) = 0;
  virtual bool unequip(Actor& equipper) = 0;
  virtual bool can_consume(Actor& consumer) = 0;
  virtual bool can_equip(Actor& equipper) = 0;
  virtual ~Item() {};
};

