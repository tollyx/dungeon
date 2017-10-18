#include "Input.h"
#include <SDL.h>

Input::Input()
{
  for (int i = 0; i < INPUTACTION_END; i++)
  {
    ispressed[(InputAction)i] = false;
    justpressed[(InputAction)i] = false;
    justreleased[(InputAction)i] = false;
  }
}


Input::~Input()
{
}

void Input::new_frame() {
  for(int i = 0; i < INPUTACTION_END; i++) {
    justpressed[i] = false;
    justreleased[i] = false;
  }
}

InputEvent Input::set_key(SDL_Keycode key, SDL_Keymod mod, bool pressed)
{
  InputEvent event{};
  event.type = INPUT_KEY_EVENT;
  event.action = ACTION_NONE;
  event.key_press_event.key = key;
  event.key_press_event.mod = mod;
  event.pressed = pressed;

  if (mod & KMOD_NUM) { // let's ignore numlock
    mod = (SDL_Keymod)(mod ^ KMOD_NUM);
  }
  if (mod & KMOD_CAPS) { // let's ignore capslock
    mod = (SDL_Keymod)(mod ^ KMOD_CAPS);
  }
  Bind bind = { key, mod };
  if (binds.find(bind) != binds.end())
  {
    InputAction action = binds[bind];
    event.action = action;
    event.key_press_event.echo = ispressed[action] != pressed;
    if (event.key_press_event.echo) { // Ignore keypress/release if it's already pressed/released, this disables repeats.
      ispressed[action] = pressed;
      if (pressed) {
        justpressed[action] = true;
      }
      else {
        justreleased[action] = true;
      }
    }
  }
  return event;
}

void Input::bind_key(SDL_Keycode key, InputAction action, SDL_Keymod mod) {
  Bind bind = { key, mod };
  if (binds.find(bind) != binds.end()) {
    SDL_LogWarn(SDL_LOG_CATEGORY_INPUT, "Overriding existing bind! (bind: %s)", bind.get_name().c_str());
  }
  binds[bind] = action;
}

void Input::unbind_key(SDL_Keycode key, SDL_Keymod mod) {
  Bind bind = { key, mod };
  binds.erase(bind);
}

bool Input::is_pressed(InputAction action) {
  return ispressed[action];
}

bool Input::was_just_pressed(InputAction action) {
  return justpressed[action];
}

bool Input::was_just_released(InputAction action) {
  return justreleased[action];
}

InputEvent Input::set_mouse_pos(int x, int y, int dx, int dy) {
  mouse_x = x;
  mouse_y = y;

  InputEvent event{};
  event.action = ACTION_NONE;
  event.type = INPUT_MOUSE_MOVE_EVENT;
  event.mouse_move_event.x = x;
  event.mouse_move_event.y = y;
  event.mouse_move_event.dx = dx;
  event.mouse_move_event.dy = dy;
  return event;
}

InputEvent Input::set_mouse_button(int button, int x, int y, bool pressed) {
  mouse_x = x;
  mouse_y = y;

  InputEvent event{};
  event.action = ACTION_NONE;
  event.type = INPUT_MOUSE_CLICK_EVENT;
  event.action = ACTION_NONE;
  event.pressed = pressed;
  event.mouse_click_event.button = button;
  event.mouse_click_event.x = x;
  event.mouse_click_event.y = y;
  return event;
}

vec2i Input::get_mouse_pos() {
  return {mouse_x, mouse_y};
}
