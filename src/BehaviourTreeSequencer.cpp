#include "BehaviourTreeSequencer.h"



BehaviourTreeSequencer::BehaviourTreeSequencer(BehaviourTreeNode* parent) : BehaviourTreeNode(parent) {}

BehaviourTreeStatus BehaviourTreeSequencer::tick(BTTick * tick) {
  for (int i = 0; i < children.size(); i++) {
    
    BehaviourTreeStatus status = children[i]->execute(tick);
    if (status != BT_SUCCEEDED) {
      return status;
    }
  }

  return BT_SUCCEEDED;
}
