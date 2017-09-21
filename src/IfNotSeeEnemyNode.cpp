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
  bool ishero = tick->target->isTypeOf(ACT_HERO);

  auto actors = tick->target->map->get_actor_list();
  for (auto actor : *actors) {
    if (actor == tick->target) continue;

    if (actor->isTypeOf(ACT_HERO) != ishero) {
      vec2i pos = actor->get_position();
      if (line_of_sight(tick->target->map, tick->target->get_position(), pos)) {
        return BT_FAILED;
      }
    }
  }
  return children[0]->execute(tick);
}
