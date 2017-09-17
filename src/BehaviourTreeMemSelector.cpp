#include "BehaviourTreeMemSelector.h"


BehaviourTreeMemSelector::BehaviourTreeMemSelector(BehaviourTreeNode* parent) : BehaviourTreeNode(parent) {
  currentchild = 0;
}

void BehaviourTreeMemSelector::open(BTTick * tick) {
  currentchild = 0;
}

BehaviourTreeStatus BehaviourTreeMemSelector::tick(BTTick* tick) {
  for (; currentchild < children.size(); currentchild++) {
    BehaviourTreeStatus status = children[currentchild]->execute(tick);
    if (status != BT_FAILED) {
      return status;
    }
  }

  return BT_FAILED;
}
