#include "Goblin.h"
#include "BehaviourTree.h"
#include "BehaviourTreeSelector.h"
#include "AttackEnemyNode.h"
#include "WanderNode.h"
#include "RestNode.h"

Goblin::Goblin(Tilemap* map, vec2i pos) : Actor(map, pos) {
  alive = true;
  health = 4;
  maxhealth = 4;
  strength = 1;

  BehaviourTreeSelector* root = new BehaviourTreeSelector(nullptr);
  bt = new BehaviourTree(root);
  {


    new AttackEnemyNode(root);
    new RestNode(root);
    new WanderNode(root);
  }
}


Goblin::~Goblin() {}
