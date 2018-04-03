#include "StateStack.h"

void StateStack::update(float dt) {
  update_state(stack.size() - 1, dt);
}

void StateStack::draw() {
  draw_state(stack.size() - 1);
}

void StateStack::push(std::unique_ptr<IState> state) {
  state->enter();
  stack.emplace_back(std::move(state));
}

void StateStack::swap(std::unique_ptr<IState> state) {
  pop();
  push(std::move(state));
}

void StateStack::pop() {
  stack.back()->exit();
  stack.pop_back();
}

void StateStack::input(InputEvent & input_event) {
  input_state(stack.size() - 1, input_event);
}

void StateStack::draw_state(unsigned int i) {
  IState &state = *stack[i];
  if (state.should_draw_previous() && i > 0) {
    draw_state(i - 1);
  }
  state.draw();
}

void StateStack::update_state(unsigned int i, float dt) {
  IState &state = *stack[i];
  if (state.should_update_previous() && i > 0) {
    update_state(i - 1, dt);
  }
  StateResult r = state.update(dt);


  if (r.action != StateAction::None && i+1 != stack.size()) {
    while (i < stack.size()) {
      pop();
    }
  }
  switch (r.action) {
  case StateAction::Pop:
    pop();
  case StateAction::Push:
    push(std::move(r.state.value()));
  case StateAction::Swap:
    swap(std::move(r.state.value()));
  default:
    break;
  }
}

void StateStack::input_state(unsigned int i, InputEvent & input_event) {
  IState &state = *stack[i];
  if (state.should_update_previous() && i > 0) {
    input_state(i - 1, input_event);
  }
  state.input(input_event);
}
