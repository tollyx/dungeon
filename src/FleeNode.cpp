#include <cstdlib>
#include "FleeNode.h"
#include "Pathfinder.h"
#include "BehaviourTree.h"
#include "Actor.h"
#include "Tilemap.h"


FleeNode::FleeNode(BehaviourTreeNode* parent) : BehaviourTreeNode(parent) {}


FleeNode::~FleeNode() {}

BehaviourTreeStatus FleeNode::tick(BTTick * tick) {
  Pathfinder::DijkstraMap dijkstra;
  Tilemap * map = tick->target->map;
  std::vector<vec2i> enemyPos;
  bool ishero = tick->target->isTypeOf(ACT_HERO);
  auto actors = tick->target->map->GetActorList();
  for (Actor* actor : *actors) {
    if (actor->isTypeOf(ACT_HERO) != ishero) {
      vec2i pos = actor->getPosition();
      if (tick->target->CanSee(pos.x, pos.y)) {
        enemyPos.push_back(pos);
      }
    }
  }
  if (enemyPos.size() <= 0) {
    return BT_FAILED;
  }
  Pathfinder::calcDijkstraMap(map, &enemyPos, &dijkstra, 16);

  dijkstra.add(-16);
  dijkstra.mult(-1);

  std::vector<vec2i> safety;
  for (int x = 0; x < dijkstra.width; x++) {
    for (int y = 0; y < dijkstra.height; y++) {
      if (dijkstra.getValue(x,y) <= 0 && dijkstra.getValue(x, y) >= -10) {
        safety.push_back(vec2i(x, y));
      }
    }
  }

  Pathfinder::calcDijkstraMap(map, &safety, &dijkstra);

  vec2i pos = tick->target->getPosition();

  std::vector<vec2i> neigh = map->getNeighbours(pos.x, pos.y);
  std::vector<vec2i> options;
  float lowestval = 999999;
  for (vec2i npos : neigh) {
    float val = dijkstra.getValue(npos.x, npos.y);
    if (val < lowestval) {
      lowestval = val;
      options.clear();
      options.push_back(npos);
    }
    else if (val == lowestval) {
      options.push_back(npos);
    }
  }
  if (options.size() >= 8) {
    //printf("FLEEING SUCCESS\n");
    return BT_FAILED;
  }
  while (options.size() > 0) {
    int i = rand() % options.size();
    vec2i next = options[i];
    vec2i dp = next - pos;
    if (tick->target->Move(dp.x, dp.y)) {
      //printf("FLEEING val:%f\t(%i,%i)\n", lowestval, next.x, next.y);
      return BT_RUNNING;
    }
    options.erase(options.begin() + i);
    if (options.size() == 0) {
      return BT_FAILED;
    }
  }
  return BT_FAILED;
}
