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
  health = 6;
  health_max = 6;
  strength = 2;
  sprite_id = '@';
  team = TEAM_PLAYER;
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


Hero::~Hero() {}
