#include "Hero.h"
#include "BehaviourTree.h"
#include "BehaviourTreeSelector.h"
#include "WanderNode.h"
#include "ExploreNode.h"
#include "AttackEnemyNode.h"
#include "IfHealthBelow.h"
#include "RestNode.h"
#include "FleeNode.h"

Hero::Hero(Tilemap* map, vec2i pos) : Actor(map, pos) {
  name = "Hero";
  alive = true;
  player_controlled = true;
  health = 6;
  health_max = 6;
  strength = 2;
  sprite_id = '@';
  faction = FACTION_PLAYER;
  sprite_color = Color(0.1f, 0.4f, 0.9f, 1);
  /*
  BehaviourTreeSelector* root = new BehaviourTreeSelector(nullptr);
  bt = new BehaviourTree(root);
  {
    IfHealthBelow* hpBelow = new IfHealthBelow(root, 4);
    new FleeNode(hpBelow);

    new AttackEnemyNode(root);
    new RestNode(root);
    //new ExploreNode(root);
    new WanderNode(root);
  }
   */
}


Hero::~Hero() = default;
