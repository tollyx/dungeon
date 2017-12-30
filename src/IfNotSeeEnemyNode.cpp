#include "IfNotSeeEnemyNode.h"
#include "BehaviourTree.h"
#include "Actor.h"
#include "Tilemap.h"
#include "FieldOfView.h"


IfNotSeeEnemyNode::IfNotSeeEnemyNode(BehaviourTreeNode * parent) : BehaviourTreeNode(parent) {}

IfNotSeeEnemyNode::~IfNotSeeEnemyNode() {}

BehaviourTreeStatus IfNotSeeEnemyNode::tick(BTTick * tick) {
  if (children.size() <= 0) {
    return BT_ERROR;
  }
  bool ishero = tick->target->is_type_of(ACT_HERO);
  vec2i targetpos = tick->target->get_position();

  auto actors = tick->target->get_map()->get_actors(targetpos.x, targetpos.y, 6);
  for (Actor* actor : actors) {
    if (actor == tick->target) continue;

    if (actor->is_type_of(ACT_HERO) != ishero) {
      vec2i pos = actor->get_position();
      if (line_of_sight(tick->target->get_map(), tick->target->get_position(), pos)) {
        return BT_FAILED;
      }
    }
  }
  return children[0]->execute(tick);
}
