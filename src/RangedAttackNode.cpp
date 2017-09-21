#include "RangedAttackNode.h"
#include "BehaviourTree.h"
#include "Actor.h"
#include "Tilemap.h"
#include "FieldOfView.h"


RangedAttackNode::RangedAttackNode(BehaviourTreeNode* parent) : BehaviourTreeNode(parent) {}


RangedAttackNode::~RangedAttackNode() {}

BehaviourTreeStatus RangedAttackNode::tick(BTTick * tick) {
  if (children.size() <= 0) {
    return BT_ERROR;
  }
  bool ishero = tick->target->isTypeOf(ACT_HERO);

  auto actors = tick->target->map->get_actor_list();
  std::vector<Actor*> enemies;
  for (auto actor : *actors) {
    if (actor == tick->target) continue;

    if (actor->isTypeOf(ACT_HERO) != ishero) {
      vec2i pos = actor->get_position();
      if (line_of_sight(tick->target->map, tick->target->get_position(), pos)) {
        enemies.push_back(actor);
      }
    }
  }

  if (enemies.size() == 0) {
    return BT_FAILED;
  }

  Actor* lowestHpActor = nullptr;
  int lowestHp;
  for (Actor* actor : enemies) {
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
