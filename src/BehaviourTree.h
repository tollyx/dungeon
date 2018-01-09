#pragma once
#include <vector>

class BehaviourTree;
class BehaviourTreeNode;
class Actor;
class Tilemap;

struct BTTick {
  Actor* target;
  Tilemap* map;
  BehaviourTree* tree;
  std::vector<BehaviourTreeNode*> openNodes;
};

class BehaviourTree {
  BehaviourTreeNode* root;
  BTTick lasttick;
public:
  BehaviourTree(BehaviourTreeNode* rootNode);
  ~BehaviourTree();
  void tick(Actor* target, Tilemap* map);
};

