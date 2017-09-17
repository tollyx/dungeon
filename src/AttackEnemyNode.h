#pragma once
#include "BehaviourTreeNode.h"

class AttackEnemyNode :
  public BehaviourTreeNode {
public:
  AttackEnemyNode(BehaviourTreeNode* parent);
  ~AttackEnemyNode();

  BehaviourTreeStatus tick(BTTick* tick);
};

