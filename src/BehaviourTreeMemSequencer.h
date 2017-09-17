#pragma once
#include "BehaviourTreeNode.h"
class BehaviourTreeMemSequencer :
  public BehaviourTreeNode {
  int currentnode;
public:
  BehaviourTreeMemSequencer(BehaviourTreeNode* parent);
  void open(BTTick* tick);
  BehaviourTreeStatus tick(BTTick* tick);
};

