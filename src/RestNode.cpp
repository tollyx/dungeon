#include "RestNode.h"
#include "BehaviourTree.h"
#include "Actor.h"


RestNode::RestNode(BehaviourTreeNode * parent) : BehaviourTreeNode(parent) {}

RestNode::~RestNode() = default;

BehaviourTreeStatus RestNode::tick(BTTick * tick) {
  if (tick->target->get_health() < tick->target->get_health_max()) {
    return BT_SUCCEEDED;
  }
  return BT_FAILED;
}
