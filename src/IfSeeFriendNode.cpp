#include "IfSeeFriendNode.h"
#include "BehaviourTree.h"
#include "Actor.h"
#include "Tilemap.h"
#include "FieldOfView.h"


IfSeeFriendNode::IfSeeFriendNode(BehaviourTreeNode* parent) : BehaviourTreeNode(parent) {}


IfSeeFriendNode::~IfSeeFriendNode() {}

BehaviourTreeStatus IfSeeFriendNode::tick(BTTick * tick) {
  if (children.size() <= 0) {
    return BT_ERROR;
  }
  bool ishero = tick->target->isTypeOf(ACT_HERO);

  auto actors = tick->target->map->get_actor_list();
  for (auto actor : *actors) {
    if (actor == tick->target) continue;

    if (actor->isTypeOf(ACT_HERO) == ishero) {
      vec2i pos = actor->get_position();
      if (line_of_sight(tick->target->map, tick->target->get_position(), pos)) {
        return children[0]->execute(tick);
      }
    }
  }
  return BT_FAILED;
}
