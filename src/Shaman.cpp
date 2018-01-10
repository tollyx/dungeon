#include "Shaman.h"
#include "BehaviourTree.h"
#include "BehaviourTreeSelector.h"
#include "WanderNode.h"
#include "RestNode.h"
#include "IfSeeFriendNode.h"
#include "HealFriendNode.h"
#include "AttackEnemyNode.h"
#include "FleeNode.h"


BehaviourTree* shamtree = nullptr;

Shaman::Shaman(vec2i pos) : Actor(pos) {
  name = "Shaman";
  alive = true;
  health = 2;
  health_max = 2;
  strength = 1;
  range = 6;
  sprite_id = 's';
  faction = FACTION_GOBS;
  sprite_color = Color(0.2, 0.7, 0.6, 1);

  if (shamtree == nullptr) {
    auto * root = new BehaviourTreeSelector(nullptr);
    shamtree = new BehaviourTree(root);
    {
      auto * seefriend = new IfSeeFriendNode(root);
      auto * fsel = new BehaviourTreeSelector(seefriend);
      {
        new HealFriendNode(fsel);
        new AttackEnemyNode(fsel);
      }

      new FleeNode(root);
      new WanderNode(root);
    }
  }
  bt = shamtree;
}

Shaman::~Shaman() = default;
