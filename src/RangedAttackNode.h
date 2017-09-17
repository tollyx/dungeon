#pragma once
#include "BehaviourTreeNode.h"
class RangedAttackNode :
  public BehaviourTreeNode {
public:
  RangedAttackNode(BehaviourTreeNode* parent);
  ~RangedAttackNode();
  BehaviourTreeStatus tick(BTTick* tick);
};

