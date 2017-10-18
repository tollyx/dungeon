#pragma once
#include "vec2i.h"
#include "Entity.h"
#include <string>

class BehaviourTree;

enum Actors { // TODO: Softcode this
  ACT_BASE,
  ACT_HERO,
  ACT_GOBLIN,
  ACT_SHAMAN
};

enum ActorFactions { // TODO: Make factions dynamic
  FACTION_NONE,
  FACTION_PLAYER,
  FACTION_GOBS
};

class Tilemap;

class Actor : public Entity {
  int healcounter;

protected:
  BehaviourTree* bt;
  int health;
  int health_max;
  int strength;
  float range;
  bool alive;
  ActorFactions faction;
public:
  int id;
  std::string name;

  Actor(Tilemap *map, vec2i pos);
  bool is_alive(){ return alive; };
  void attack(vec2i dpos); // basic melee attack
  void attack(Actor* act);
  void heal(int amount);
  void damage(int strength);
  int get_strength() { return strength; }
  int get_health() { return health; }
  int get_health_max() { return health_max; }
  ActorFactions get_actor_faction() { return faction; }
  float get_range() { return range; }
  void kill() { alive = false; health = 0; collision = false; };
  void update();
  virtual bool is_type_of(Actors actor){ return actor == ACT_BASE; };
  virtual Actors type() { return ACT_BASE; };
  EntityTypes entity_type() override { return ENTITY_ACTOR; };
  ~Actor();
};
