#pragma once
#include "BehaviourTreeNode.h"
class HealFriendNode :
  public BehaviourTreeNode {
public:
  HealFriendNode(BehaviourTreeNode* parent);
  ~HealFriendNode();
  BehaviourTreeStatus tick(BTTick* tick);
};

