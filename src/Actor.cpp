#include "Actor.h"
#include "Tilemap.h"
#include "BehaviourTree.h"

int id_counter = 0;

Actor::Actor(vec2i pos) : Entity(pos) {
  id = id_counter++;
  name = "Actor";
  range = 1.5f;
  player_controlled = false;
  collision = true;
  bt = nullptr;
  faction = FACTION_NONE;
  sprite_id = 1;
}

void Actor::update(Tilemap* map) {
  if (!alive) return;

  if (!player_controlled && bt != nullptr) {
    bt->tick(this, map);
  }
  if (health < health_max) {
    healcounter--;
    if (healcounter <= 0) {
      heal(1);
      healcounter = 4;
    }
  }
  if (health <= 0) {
    kill();
  }
}

void Actor::damage(int str) {
  health -= str;
  if (health <= 0) {
    kill();
  }
}

void Actor::attack(Actor *act) {
  if (act) {
    vec2i dpos = get_position() - act->get_position();
    if (dpos.dist() <= range) {
      act->damage(strength);
    }
  }
}

void Actor::attack(vec2i dpos, Tilemap* map) {
  if (dpos.dist() <= range) {
    vec2i pos = get_position();
    auto acts = map->get_actors(pos.x + dpos.x, pos.y + dpos.y, 0);
    for (Entity* ent : acts) {
      auto act = (Actor*)ent;
      if (act->is_alive() && act->get_actor_faction() != faction) {
        act->damage(strength);
        break;
      }
    }
  }
}

void Actor::heal(int amount) {
  health += amount;
  if (health > health_max) {
    health = health_max;
  }
}

Actor::~Actor() = default;
