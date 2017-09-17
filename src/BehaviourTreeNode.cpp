#include "BehaviourTreeNode.h"
#include "BehaviourTree.h"

BehaviourTreeNode::BehaviourTreeNode(BehaviourTreeNode* parent) {
  if (parent != nullptr) {
    parent->AddChild(this);
  }
  
  isOpen = false;
}

BehaviourTreeNode::~BehaviourTreeNode() {
  for each (BehaviourTreeNode* child in children) {
    delete child;
  }
}

BehaviourTreeStatus BehaviourTreeNode::execute(BTTick* tick) {
  enter(tick);

  if (!isOpen) {
    _open(tick);
  }

  BehaviourTreeStatus status = this->tick(tick);

  if (status != BT_RUNNING) {
    _close(tick);
  }

  exit(tick);

  return status;
}

void BehaviourTreeNode::_open(BTTick * tick) {
  if (!isOpen) {
    isOpen = true;
    tick->openNodes.push_back(this);
    open(tick);
  }
}

void BehaviourTreeNode::_close(BTTick * tick) {
  if (isOpen) {
    isOpen = false;
    if (tick->openNodes.size() > 0) { // Patchwork, no idea why the vector is empty to begin with at times.
      tick->openNodes.pop_back();
    }
    close(tick);
  }
}

void BehaviourTreeNode::AddChild(BehaviourTreeNode* node) {
  children.push_back(node);
}
