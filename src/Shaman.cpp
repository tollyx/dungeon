#include "Shaman.h"
#include "BehaviourTree.h"
#include "BehaviourTreeSelector.h"
#include "WanderNode.h"
#include "RestNode.h"
#include "IfSeeFriendNode.h"
#include "HealFriendNode.h"
#include "RangedAttackNode.h"
#include "FleeNode.h"


Shaman::Shaman(Tilemap* map, vec2i pos) : Actor(map, pos) {
  alive = true;
  health = 2;
  maxhealth = 2;
  strength = 1;

  BehaviourTreeSelector* root = new BehaviourTreeSelector(nullptr);
  bt = new BehaviourTree(root);
  {
    IfSeeFriendNode* seefriend = new IfSeeFriendNode(root);
    BehaviourTreeSelector* fsel = new BehaviourTreeSelector(seefriend);
    {
      new HealFriendNode(fsel);
      //new RangedAttackNode(fsel);
    }

    new FleeNode(root);
    new WanderNode(root);
  }
}


Shaman::~Shaman() {}
