#include <cstdlib>
#include "ExploreNode.h"
#include "Pathfinder.h"
#include "BehaviourTree.h"
#include "Actor.h"
#include "Tilemap.h"
#include <SDL.h>

ExploreNode::ExploreNode(BehaviourTreeNode* parent) : BehaviourTreeNode(parent){}

ExploreNode::~ExploreNode() {}

BehaviourTreeStatus ExploreNode::tick(BTTick * tick) {
  SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Tried to use disabled AutoExplore BehaviourTree node!");
  return BT_FAILED;
  /*
  Pathfinder::DijkstraMap dijkstra;
  Tilemap * map = tick->target->map;
  std::vector<vec2i> unexplored;
  for (int x = 0; x < map->GetWidth(); x++) {
    for (int y = 0; y < map->GetHeight(); y++) {
      if (!tick->target->HasSeen(x,y)) {
        unexplored.push_back(vec2i(x, y));
      }
    }
  }
  Pathfinder::calcDijkstraMap(map, &unexplored, &dijkstra, 99999);

  vec2i pos = tick->target->get_position();

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
    return BT_FAILED;
  }
  while (options.size() > 0) {
    int i = std::rand() % options.size();
    vec2i next = options[i];
    vec2i dp = next - pos;
    if (tick->target->Move(dp.x, dp.y)) {
      //printf("EXPLORE %f\n", lowestval);
      return BT_RUNNING;
    }
    options.erase(options.begin() + i);
    if (options.size() == 0) {
      return BT_FAILED;
    }
  }
  return BT_FAILED;
   */
}
