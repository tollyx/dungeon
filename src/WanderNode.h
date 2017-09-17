#pragma once
#include "BehaviourTreeNode.h"
#include "vec2i.h"

class WanderNode :
  public BehaviourTreeNode {
  std::vector<vec2i> previous;
public:
  WanderNode(BehaviourTreeNode* parent);
  ~WanderNode();
  BehaviourTreeStatus tick(BTTick* tick);
};

