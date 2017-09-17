#pragma once
#include "BehaviourTreeNode.h"


class FleeNode :
  public BehaviourTreeNode {
public:
  FleeNode(BehaviourTreeNode* parent);
  ~FleeNode();
  BehaviourTreeStatus tick(BTTick* tick);
};

