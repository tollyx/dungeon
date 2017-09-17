#include "BehaviourTreeMemSequencer.h"



BehaviourTreeMemSequencer::BehaviourTreeMemSequencer(BehaviourTreeNode* parent) : BehaviourTreeNode(parent) {
  currentnode = 0;
}

void BehaviourTreeMemSequencer::open(BTTick * tick) {
  currentnode = 0;
}

BehaviourTreeStatus BehaviourTreeMemSequencer::tick(BTTick * tick) {
  for (; currentnode < children.size(); currentnode++) {
    
    BehaviourTreeStatus status = children[currentnode]->execute(tick);
    if (status != BT_SUCCEEDED) {
      return status;
    }
  }

  return BT_SUCCEEDED;
}
