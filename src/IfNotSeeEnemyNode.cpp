#include "IfNotSeeEnemyNode.h"
#include "BehaviourTree.h"
#include "Actor.h"
#include "Tilemap.h"


IfNotSeeEnemyNode::IfNotSeeEnemyNode(BehaviourTreeNode * parent) : BehaviourTreeNode(parent) {}

IfNotSeeEnemyNode::~IfNotSeeEnemyNode() {}

BehaviourTreeStatus IfNotSeeEnemyNode::tick(BTTick * tick) {
  if (children.size() <= 0) {
    return BT_ERROR;
  }
  bool ishero = tick->target->isTypeOf(ACT_HERO);

  auto actors = tick->target->map->GetActorList();
  for each (auto actor in *actors) {
    if (actor == tick->target) continue;

    if (actor->isTypeOf(ACT_HERO) != ishero) {
      vec2i pos = actor->getPosition();
      if (tick->target->CanSee(pos.x, pos.y)) {
        return BT_FAILED;
      }
    }
  }
  return children[0]->execute(tick);
}
