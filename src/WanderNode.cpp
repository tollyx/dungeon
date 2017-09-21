#include <cstdlib>
#include "WanderNode.h"
#include "BehaviourTree.h"
#include "Actor.h"
#include "Tilemap.h"

WanderNode::WanderNode(BehaviourTreeNode* parent) : BehaviourTreeNode(parent){}


WanderNode::~WanderNode() {}

BehaviourTreeStatus WanderNode::tick(BTTick * tick) {
  vec2i pos = tick->target->get_position();
  std::vector<vec2i> neighbours = tick->target->map->getNeighbours(pos.x, pos.y);
  while (true) {
    if (neighbours.size() <= 0) {
      previous.clear();
      return BT_FAILED;
    }
    int i = rand() % neighbours.size();
    vec2i dp = neighbours[i] - pos;
    bool valid = true;
    for (vec2i var : previous) {
      if (var == neighbours[i]) {
        valid = false;
        break;
      }
    }
    if (valid && tick->target->Move(dp.x, dp.y)) {
      previous.push_back(neighbours[i]);
      if (previous.size() > 5) {
        previous.erase(previous.begin());
      }
      return BT_SUCCEEDED;
    }
    else {
      neighbours.erase(neighbours.begin() + i);
    }
  }
}
