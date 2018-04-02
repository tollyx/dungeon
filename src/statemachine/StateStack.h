#pragma once

#include <memory>
#include <optional>
#include <vector>

#include "../Input.h"

class StateStack;
class IState;

enum class StateAction {
  None,
  Push,
  Pop,
  Swap,
};

struct StateResult {
  StateResult(StateAction action, std::optional<std::unique_ptr<IState>> state) {
    this->action = action;
    this->state.swap(state);
  }

  StateAction action;
  std::optional<std::unique_ptr<IState>> state;

  static StateResult None() {
    return StateResult(StateAction::None, nullptr);
  }

  static StateResult Pop() {
    return StateResult(StateAction::Pop, nullptr);
  }
};

 class IState {
public:
  IState() = default;
  virtual ~IState() = default;
  virtual void enter() = 0;
  virtual void exit() = 0;
  virtual StateResult update(float dt) = 0;
  virtual bool should_update_previous() { return false; };
  virtual void draw() = 0;
  virtual bool should_draw_previous() { return false; };
  virtual void input(InputEvent &const event) = 0;
  virtual bool should_input_previous() { return false; };
};

class StateStack {
public:
  StateStack() = default;
  ~StateStack() = default;
  void update(float dt);
  void draw();
  void push(std::unique_ptr<IState> state);
  void swap(std::unique_ptr<IState> state);
  void pop();
  void input(InputEvent& input_event);
private:
  void draw_state(unsigned int i);
  void update_state(unsigned int i, float dt);
  void input_state(unsigned int i, InputEvent &input_event);
  std::vector<std::unique_ptr<IState>> stack;
};
