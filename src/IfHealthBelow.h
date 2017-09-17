#pragma once
#include "BehaviourTreeNode.h"
class IfHealthBelow :
  public BehaviourTreeNode {
  int healthBelow;
public:
  IfHealthBelow(BehaviourTreeNode* root, int healthBelow);
  ~IfHealthBelow();
  BehaviourTreeStatus tick(BTTick* tick);
};

