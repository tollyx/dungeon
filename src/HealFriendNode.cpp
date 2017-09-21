#include "HealFriendNode.h"
#include "BehaviourTree.h"
#include "Actor.h"
#include "Tilemap.h"
#include "FieldOfView.h"


HealFriendNode::HealFriendNode(BehaviourTreeNode * parent) : BehaviourTreeNode(parent){}

HealFriendNode::~HealFriendNode() {}

BehaviourTreeStatus HealFriendNode::tick(BTTick * tick) {
  bool ishero = tick->target->isTypeOf(ACT_HERO);

  auto actors = tick->target->map->get_actor_list();
  std::vector<Actor*> friends;
  for (auto actor : *actors) {
    if (actor == tick->target) continue;

    if (actor->isTypeOf(ACT_HERO) == ishero && actor->health < actor->maxhealth-1) {
      vec2i pos = actor->get_position();
      if (line_of_sight(tick->target->map, tick->target->get_position(), pos)) {
        friends.push_back(actor);
      }
    }
  }

  if (friends.size() == 0) {
    return BT_FAILED;
  }

  Actor* lowestHpActor = nullptr;
  int lowestHp;
  for (Actor* actor : friends) {
    if (lowestHpActor == nullptr || actor->health < lowestHp) {
      lowestHpActor = actor;
      lowestHp = actor->health;
    }
  }

  lowestHpActor->health += 1;
  return BT_SUCCEEDED;
}
