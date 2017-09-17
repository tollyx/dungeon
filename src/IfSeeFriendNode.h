#pragma once
#include "BehaviourTreeNode.h"

class IfSeeFriendNode :
  public BehaviourTreeNode {
public:
  IfSeeFriendNode(BehaviourTreeNode* parent);
  ~IfSeeFriendNode();

  BehaviourTreeStatus tick(BTTick* tick);
};

