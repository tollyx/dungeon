#pragma once
#include "vec2i.h"

class BehaviourTree;

enum Actors {
  ACT_BASE,
  ACT_HERO,
  ACT_GOBLIN,
  ACT_SHAMAN
};

class Tilemap;

class Actor {
  vec2i position;
  int healcounter;

protected:
  BehaviourTree* bt;
public:
  Tilemap* map;
  bool alive;
  int health;
  int maxhealth;
  int strength;


  Actor(Tilemap* map, vec2i pos);
  const vec2i get_position();
  bool IsAlive(){ return alive; };
  bool Move(int dx, int dy);
  int GetHealth() { return health; }
  void Kill() { alive = false; health = 0; };
  void update();
  virtual bool isTypeOf(Actors actor){ return actor == ACT_BASE; };
  virtual Actors Type() { return ACT_BASE; };
  ~Actor();
};
