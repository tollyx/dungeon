//
// Created by Adrian on 2017-09-25.
//

#ifndef DUNGEON_STATS_H
#define DUNGEON_STATS_H

struct Stats {
  int vitality;
  int endurance;
  int intelligence;
  int strength;
  int dexterity;
  int perception;
};

struct SecondaryStats { // derived from the primary stats
  int health;
  int health_max;
  int mana;
  int mana_max;
  int damage_melee;
  int damage_magic;
  int damage_ranged;
  int defence;
  int resistance;
};

#endif //DUNGEON_STATS_H
