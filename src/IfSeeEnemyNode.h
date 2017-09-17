#pragma once
#include "BehaviourTreeNode.h"
class IfSeeEnemyNode :
  public BehaviourTreeNode {
public:
  IfSeeEnemyNode(BehaviourTreeNode* parent);
  ~IfSeeEnemyNode();
  BehaviourTreeStatus tick(BTTick* tick);
};

