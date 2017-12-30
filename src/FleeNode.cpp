#include <cstdlib>
#include "FleeNode.h"
#include "Pathfinder.h"
#include "BehaviourTree.h"
#include "Actor.h"
#include "Tilemap.h"
#include "FieldOfView.h"


FleeNode::FleeNode(BehaviourTreeNode* parent) : BehaviourTreeNode(parent) {}


FleeNode::~FleeNode() = default;

BehaviourTreeStatus FleeNode::tick(BTTick * tick) {
  Pathfinder::DijkstraMap dijkstra;
  Tilemap * map = tick->target->get_map();
  std::vector<vec2i> enemyPos;
  bool ishero = tick->target->is_type_of(ACT_HERO);
  vec2i targetpos = tick->target->get_position();
  auto actors = tick->target->get_map()->get_actors(targetpos.x, targetpos.y, 6);
  for (Actor* actor : actors) {
    if (actor->is_type_of(ACT_HERO) != ishero) {
      vec2i pos = actor->get_position();
      if (line_of_sight(tick->target->get_map(), tick->target->get_position(), pos)) {
        enemyPos.push_back(pos);
      }
    }
  }
  if (enemyPos.empty()) {
    return BT_FAILED;
  }
  Pathfinder::calcDijkstraMap(map, &enemyPos, &dijkstra, 16);

  dijkstra.add(-16);
  dijkstra.mult(-1);

  std::vector<vec2i> safety;
  for (int x = 0; x < dijkstra.width; x++) {
    for (int y = 0; y < dijkstra.height; y++) {
      if (dijkstra.getValue(x,y) <= 0 && dijkstra.getValue(x, y) >= -10) {
        safety.emplace_back(x, y);
      }
    }
  }

  Pathfinder::calcDijkstraMap(map, &safety, &dijkstra);

  vec2i pos = tick->target->get_position();

  std::vector<vec2i> neigh = map->get_neighbours(pos.x, pos.y);
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
  while (!options.empty()) {
    int i = rand() % options.size();
    vec2i next = options[i];
    vec2i dp = next - pos;
    if (tick->target->move(dp.x, dp.y)) {
      //printf("FLEEING val:%f\t(%i,%i)\n", lowestval, next.x, next.y);
      return BT_RUNNING;
    }
    options.erase(options.begin() + i);
    if (options.empty()) {
      return BT_FAILED;
    }
  }
  return BT_FAILED;
}
