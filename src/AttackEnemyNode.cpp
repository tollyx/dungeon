#include "AttackEnemyNode.h"
#include "BehaviourTree.h"
#include "Actor.h"
#include "Tilemap.h"
#include "Pathfinder.h"
#include "FieldOfView.h"

AttackEnemyNode::AttackEnemyNode(BehaviourTreeNode * parent) : BehaviourTreeNode(parent){}

AttackEnemyNode::~AttackEnemyNode() {}

BehaviourTreeStatus AttackEnemyNode::tick(BTTick * tick) {
  bool ishero = tick->target->isTypeOf(ACT_HERO);

  auto actors = tick->target->map->get_actor_list();
  std::vector<Actor*> visibleEnemies;

  for (auto actor : *actors) {
    if (actor == tick->target) continue;

    if (actor->isTypeOf(ACT_HERO) != ishero) {
      vec2i pos = actor->get_position();
      if (line_of_sight(tick->target->map, tick->target->get_position(), pos)) {
        visibleEnemies.push_back(actor);
      }
    }
  }

  if (visibleEnemies.size() == 0) {
    return BT_FAILED;
  }

  Actor* closestActor = nullptr;
  float closestDist;
  for (Actor* actor : visibleEnemies) {
    float dist = Pathfinder::distance(tick->target->get_position(), actor->get_position());
    if (closestActor == nullptr ||
         dist < closestDist) {
      closestActor = actor;
      closestDist = dist;
    }
  }

  if (closestDist < 1.5f) {
    closestActor->health -= tick->target->strength;
    if (closestActor->health <= 0) {
      closestActor->Kill();
      return BT_SUCCEEDED;
    }
    return BT_RUNNING;
  }
  else {
    vec2i pos = tick->target->get_position();
    vec2i goal = closestActor->get_position();
    auto path = Pathfinder::aStar(tick->target->map, pos, goal);
    if (path.size() > 0) {
      //path.pop_back();
      vec2i dpos = path.back() - pos;
      if (tick->target->Move(dpos.x, dpos.y)) {
        return BT_RUNNING;
      }
    }
    return BT_FAILED;
  }
}
