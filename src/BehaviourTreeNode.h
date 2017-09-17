#pragma once
#include <vector>

struct BTTick;
class Actor;

enum BehaviourTreeStatus {
  BT_SUCCEEDED,
  BT_FAILED,
  BT_RUNNING,
  BT_ERROR,
};

class BehaviourTreeNode {
  bool isOpen;
protected:

  virtual void enter(BTTick* tick) {};
  virtual void open(BTTick* tick) {};
  virtual BehaviourTreeStatus tick(BTTick* tick) = 0;
  virtual void close(BTTick* tick) {};
  virtual void exit(BTTick* tick) {};

  std::vector<BehaviourTreeNode*> children;
  void AddChild(BehaviourTreeNode* node);
public:
  BehaviourTreeNode(BehaviourTreeNode* parent);
  ~BehaviourTreeNode();
  BehaviourTreeStatus execute(BTTick* tick);
  void _open(BTTick* tick);
  void _close(BTTick* tick);
};

