#pragma once
#include <vector>

class BehaviourTree;
class BehaviourTreeNode;
class Actor;

struct BTTick {
  Actor* target;
  BehaviourTree* tree;
  std::vector<BehaviourTreeNode*> openNodes;
};

class BehaviourTree {
  BehaviourTreeNode* root;
  BTTick lasttick;
public:
  BehaviourTree(BehaviourTreeNode* rootNode);
  ~BehaviourTree();
  void tick(Actor* target);
};

