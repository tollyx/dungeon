#include "BehaviourTree.h"
#include "BehaviourTreeNode.h"


BehaviourTree::BehaviourTree(BehaviourTreeNode* rootnode) {
  root = rootnode;
}


BehaviourTree::~BehaviourTree() {
  if (root != nullptr) {
    delete root;
  }
}

void BehaviourTree::tick(Actor * target) {
  BTTick tick;
  tick.target = target;
  tick.tree = this;

  root->execute(&tick);

  int start = 0;
  int min;
  if (tick.openNodes.size() < lasttick.openNodes.size()) {
    min = tick.openNodes.size();
  }
  else {
    min = lasttick.openNodes.size();
  }
  for (int i = 0; i < min; i++) {
    start = i + 1;
    if (tick.openNodes[i] != lasttick.openNodes[i]) {
      break;
    }
  }

  for (int i = lasttick.openNodes.size()-1; i >= start; i--) {
    lasttick.openNodes[i]->_close(&lasttick);
  }
  lasttick = tick;
}
