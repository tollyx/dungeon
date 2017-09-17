#pragma once
#include "BehaviourTreeNode.h"
class RestNode :
  public BehaviourTreeNode {
public:
  RestNode(BehaviourTreeNode* parent);
  ~RestNode();
  BehaviourTreeStatus tick(BTTick* tick);
};

