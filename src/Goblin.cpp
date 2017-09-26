#include "Goblin.h"
#include "BehaviourTree.h"
#include "BehaviourTreeSelector.h"
#include "AttackEnemyNode.h"
#include "WanderNode.h"
#include "RestNode.h"

BehaviourTree* gobtree = nullptr;

Goblin::Goblin(Tilemap* map, vec2i pos) : Actor(map, pos) {
  name = "Goblin";
  alive = true;
  health = 4;
  health_max = 4;
  strength = 1;
  sprite_id = 'g';
  team = TEAM_GOBS;

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
