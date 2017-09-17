#pragma once
#include "BehaviourTreeNode.h"

class BehaviourTreeMemSelector :
  public BehaviourTreeNode {
  int currentchild;
public:
  BehaviourTreeMemSelector(BehaviourTreeNode* parent);
  void open(BTTick* tick);
  BehaviourTreeStatus tick(BTTick* tick);
};

