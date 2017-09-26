#pragma once
#include "vec2i.h"
#include "Entity.h"
#include <string>

class BehaviourTree;

enum Actors {
  ACT_BASE,
  ACT_HERO,
  ACT_GOBLIN,
  ACT_SHAMAN
};

enum ActorTeams {
  TEAM_NONE,
  TEAM_PLAYER,
  TEAM_GOBS
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
  ActorTeams team;
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
  ActorTeams get_actor_team() { return team; }
  float get_range() { return range; }
  void kill() { alive = false; health = 0; collision = false; };
  void update();
  virtual bool is_type_of(Actors actor){ return actor == ACT_BASE; };
  virtual Actors type() { return ACT_BASE; };
  EntityTypes entity_type() override { return ENTITY_ACTOR; };
  ~Actor();
};
