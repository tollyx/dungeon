#include "Goblin.h"
#include "BehaviourTree.h"
#include "BehaviourTreeSelector.h"
#include "AttackEnemyNode.h"
#include "WanderNode.h"
#include "RestNode.h"

BehaviourTree* gobtree = nullptr;

Goblin::Goblin(Tilemap* map, vec2i pos) : Actor(map, pos) {
  alive = true;
  health = 4;
  maxhealth = 4;
  strength = 1;
  if (gobtree == nullptr) {
    auto * root = new BehaviourTreeSelector(nullptr);
    gobtree = new BehaviourTree(root);
    {
      new AttackEnemyNode(root);
      new RestNode(root);
      new WanderNode(root);
    }
  }
  bt = gobtree;
}


Goblin::~Goblin() = default;
