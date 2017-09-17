#include "RestNode.h"
#include "BehaviourTree.h"
#include "Actor.h"


RestNode::RestNode(BehaviourTreeNode * parent) : BehaviourTreeNode(parent) {}

RestNode::~RestNode() {}

BehaviourTreeStatus RestNode::tick(BTTick * tick) {
  if (tick->target->health < tick->target->maxhealth) {
    return BT_SUCCEEDED;
  }
  return BT_FAILED;
}
