#pragma once
#include "BehaviourTreeNode.h"

class BehaviourTreeSelector :
  public BehaviourTreeNode {
public:
  BehaviourTreeSelector(BehaviourTreeNode* parent);
  BehaviourTreeStatus tick(BTTick* tick);
};

