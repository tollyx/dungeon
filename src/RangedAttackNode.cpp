#include "RangedAttackNode.h"
#include "BehaviourTree.h"
#include "Actor.h"
#include "Tilemap.h"
#include "FieldOfView.h"


RangedAttackNode::RangedAttackNode(BehaviourTreeNode* parent) : BehaviourTreeNode(parent) {}


RangedAttackNode::~RangedAttackNode() = default;

BehaviourTreeStatus RangedAttackNode::tick(BTTick * tick) {
  bool ishero = tick->target->is_type_of(ACT_HERO);
  vec2i targetpos = tick->target->get_position();

  auto actors = tick->target->get_map()->get_entities(targetpos.x, targetpos.y, 6, ENTITY_ACTOR);
  std::vector<Actor*> enemies;
  for (auto ent : actors) {
    auto actor = (Actor*)ent;
    if (actor == tick->target) continue;

    if (actor->is_type_of(ACT_HERO) != ishero) {
      vec2i pos = actor->get_position();
      if (line_of_sight(tick->target->get_map(), tick->target->get_position(), pos)) {
        enemies.push_back(actor);
      }
    }
  }

  if (enemies.empty()) {
    return BT_FAILED;
  }

  Actor* lowestHpActor = nullptr;
  int lowestHp;
  for (Actor* actor : enemies) {
    if (lowestHpActor == nullptr || actor->get_health() < lowestHp) {
      lowestHpActor = actor;
      lowestHp = actor->get_health();
    }
  }

  tick->target->attack(lowestHpActor);
  return BT_SUCCEEDED;
}
