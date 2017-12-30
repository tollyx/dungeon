#include "HealFriendNode.h"
#include "BehaviourTree.h"
#include "Actor.h"
#include "Tilemap.h"
#include "FieldOfView.h"


HealFriendNode::HealFriendNode(BehaviourTreeNode * parent) : BehaviourTreeNode(parent){}

HealFriendNode::~HealFriendNode() = default;

BehaviourTreeStatus HealFriendNode::tick(BTTick * tick) {
  bool ishero = tick->target->is_type_of(ACT_HERO);
  vec2i targetpos = tick->target->get_position();

  auto actors = tick->target->get_map()->get_actors(targetpos.x, targetpos.y, 6);
  std::vector<Actor*> friends;
  for (Actor* actor : actors) {
    if (actor == tick->target) continue;

    if (actor->is_type_of(ACT_HERO) == ishero && actor->get_health() < actor->get_health_max()) {
      vec2i pos = actor->get_position();
      if (line_of_sight(tick->target->get_map(), tick->target->get_position(), pos)) {
        friends.push_back(actor);
      }
    }
  }

  if (friends.empty()) {
    return BT_FAILED;
  }

  Actor* lowestHpActor = nullptr;
  int lowestHp;
  for (Actor* actor : friends) {
    if (lowestHpActor == nullptr || actor->get_health() < lowestHp) {
      lowestHpActor = actor;
      lowestHp = actor->get_health();
    }
  }

  lowestHpActor->heal(1);
  return BT_SUCCEEDED;
}
