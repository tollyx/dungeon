#include "IfHealthBelow.h"
#include "BehaviourTree.h"
#include "Actor.h"

IfHealthBelow::IfHealthBelow(BehaviourTreeNode * root, int healthBelow) : BehaviourTreeNode(root) {
  this->healthBelow = healthBelow;
}

IfHealthBelow::~IfHealthBelow() = default;

BehaviourTreeStatus IfHealthBelow::tick(BTTick * tick) {
  if (children.empty()) return BT_ERROR;
  if (tick->target->get_health() < healthBelow) {
    return children[0]->execute(tick);
  }
  return BT_FAILED;
}
