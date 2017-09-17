#pragma once
#include "BehaviourTreeNode.h"
class ExploreNode :
  public BehaviourTreeNode {
public:
  ExploreNode(BehaviourTreeNode* parent);
  ~ExploreNode();
  BehaviourTreeStatus tick(BTTick* tick);
};

