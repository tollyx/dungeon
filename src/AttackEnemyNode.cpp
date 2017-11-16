#include "AttackEnemyNode.h"
#include "BehaviourTree.h"
#include "Actor.h"
#include "Level.h"
#include "Pathfinder.h"
#include "FieldOfView.h"

AttackEnemyNode::AttackEnemyNode(BehaviourTreeNode * parent) : BehaviourTreeNode(parent){}

AttackEnemyNode::~AttackEnemyNode() = default;

BehaviourTreeStatus AttackEnemyNode::tick(BTTick * tick) {
  bool ishero = tick->target->is_type_of(ACT_HERO);
  vec2i targetpos = tick->target->get_position();

  auto actors = tick->target->get_map()->get_entities(targetpos.x, targetpos.y, 6, ENTITY_ACTOR);
  std::vector<Actor*> visibleEnemies;

  for (auto ent : actors) {
    auto actor = (Actor*)ent;
    if (actor == tick->target) continue;


    if (actor->is_type_of(ACT_HERO) != ishero) {
      vec2i pos = actor->get_position();
      if (line_of_sight(tick->target->get_map(), tick->target->get_position(), pos)) {
        visibleEnemies.push_back(actor);
      }
    }
  }

  if (visibleEnemies.empty()) {
    return BT_FAILED;
  }

  Actor* closestActor = nullptr;
  float closestDist = tick->target->get_range();
  for (Actor* actor : visibleEnemies) {
    float dist = Pathfinder::distance(tick->target->get_position(), actor->get_position());
    if (closestActor == nullptr ||
         dist < closestDist) {
      closestActor = actor;
      closestDist = dist;
    }
  }

  if (closestDist < tick->target->get_range()) {
    tick->target->attack(closestActor);
    if (!closestActor->is_alive()) {
      return BT_SUCCEEDED;
    }
    return BT_RUNNING;
  }
  else {
    vec2i pos = tick->target->get_position();
    vec2i goal = closestActor->get_position();
    auto path = Pathfinder::aStar(tick->target->get_map(), pos, goal);
    if (!path.empty()) {
      //path.pop_back();
      vec2i dpos = path.back() - pos;
      if (tick->target->move(dpos.x, dpos.y)) {
        return BT_RUNNING;
      }
    }
    return BT_FAILED;
  }
}
