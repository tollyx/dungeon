#pragma once
#include "BehaviourTreeNode.h"
class BehaviourTreeSequencer :
  public BehaviourTreeNode {
public:
  BehaviourTreeSequencer(BehaviourTreeNode* parent);
  BehaviourTreeStatus tick(BTTick* tick);
};

