#include "IfNotSeeEnemyNode.h"
#include "BehaviourTree.h"
#include "Actor.h"
#include "Level.h"
#include "FieldOfView.h"


IfNotSeeEnemyNode::IfNotSeeEnemyNode(BehaviourTreeNode * parent) : BehaviourTreeNode(parent) {}

IfNotSeeEnemyNode::~IfNotSeeEnemyNode() {}

BehaviourTreeStatus IfNotSeeEnemyNode::tick(BTTick * tick) {
  if (children.size() <= 0) {
    return BT_ERROR;
  }
  bool ishero = tick->target->is_type_of(ACT_HERO);
  vec2i targetpos = tick->target->get_position();

  auto actors = tick->target->get_map()->get_entities(targetpos.x, targetpos.y, 6, ENTITY_ACTOR);
  for (auto ent : actors) {
    auto actor = (Actor*)ent;
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
