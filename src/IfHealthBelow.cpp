#include "IfHealthBelow.h"
#include "BehaviourTree.h"
#include "Actor.h"

IfHealthBelow::IfHealthBelow(BehaviourTreeNode * root, int healthBelow) : BehaviourTreeNode(root) {
  this->healthBelow = healthBelow;
}

IfHealthBelow::~IfHealthBelow() {}

BehaviourTreeStatus IfHealthBelow::tick(BTTick * tick) {
  if (children.size() == 0) return BT_ERROR;
  if (tick->target->health < healthBelow) {
    return children[0]->execute(tick);
  }
  return BT_FAILED;
}
