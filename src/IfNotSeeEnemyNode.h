#pragma once
#include "BehaviourTreeNode.h"
class IfNotSeeEnemyNode :
  public BehaviourTreeNode {
public:
  IfNotSeeEnemyNode(BehaviourTreeNode* parent);
  ~IfNotSeeEnemyNode();
  BehaviourTreeStatus tick(BTTick* tick);
};

