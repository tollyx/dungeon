#include "RangedAttackNode.h"
#include "BehaviourTree.h"
#include "Actor.h"
#include "Tilemap.h"


RangedAttackNode::RangedAttackNode(BehaviourTreeNode* parent) : BehaviourTreeNode(parent) {}


RangedAttackNode::~RangedAttackNode() {}

BehaviourTreeStatus RangedAttackNode::tick(BTTick * tick) {
  if (children.size() <= 0) {
    return BT_ERROR;
  }
  bool ishero = tick->target->isTypeOf(ACT_HERO);

  auto actors = tick->target->map->GetActorList();
  std::vector<Actor*> enemies;
  for each (auto actor in *actors) {
    if (actor == tick->target) continue;

    if (actor->isTypeOf(ACT_HERO) != ishero) {
      vec2i pos = actor->getPosition();
      if (tick->target->CanSee(pos.x, pos.y)) {
        enemies.push_back(actor);
      }
    }
  }

  if (enemies.size() == 0) {
    return BT_FAILED;
  }

  Actor* lowestHpActor = nullptr;
  int lowestHp;
  for each (Actor* actor in enemies) {
    if (lowestHpActor == nullptr || actor->health < lowestHp) {
      lowestHpActor = actor;
      lowestHp = actor->health;
    }
  }

  lowestHpActor->health -= tick->target->strength;
  if (lowestHpActor->health <= 0) {
    lowestHpActor->Kill();
  }
  return BT_SUCCEEDED;
}
