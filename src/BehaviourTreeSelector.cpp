#include "BehaviourTreeSelector.h"

BehaviourTreeSelector::BehaviourTreeSelector(BehaviourTreeNode* parent) : BehaviourTreeNode(parent) {}

BehaviourTreeStatus BehaviourTreeSelector::tick(BTTick* tick) {
  for (int i = 0; i < children.size(); i++) {
    BehaviourTreeStatus status = children[i]->execute(tick);
    if (status != BT_FAILED) {
      return status;
    }
  }

  return BT_FAILED;
}
