#include "IfSeeEnemyNode.h"
#include "BehaviourTree.h"
#include "Actor.h"
#include "Tilemap.h"
#include "FieldOfView.h"


IfSeeEnemyNode::IfSeeEnemyNode(BehaviourTreeNode * parent) : BehaviourTreeNode(parent) {}

IfSeeEnemyNode::~IfSeeEnemyNode() {}

BehaviourTreeStatus IfSeeEnemyNode::tick(BTTick * tick) {
  if (children.size() <= 0) {
    return BT_ERROR;
  }
  bool ishero = tick->target->isTypeOf(ACT_HERO);

  auto actors = tick->target->map->GetActorList();
  for (auto actor : *actors) {
    if (actor == tick->target) continue;

    if (actor->isTypeOf(ACT_HERO) != ishero) {
      vec2i pos = actor->getPosition();
      if (line_of_sight(tick->target->map, tick->target->getPosition(), pos)) {
        return children[0]->execute(tick);
      }
    }
  }
  return BT_FAILED;
}
