#pragma once

#include <SDL2/SDL.h>
#include <map>
#include "vec2i.h"

enum InputAction {
  ACTION_NONE,
  ACTION_ESCAPE_MENU,
  ACTION_RESET,
  ACTION_TOGGLE_DEBUG,
  ACTION_MOVE_NORTH,
  ACTION_MOVE_NORTHWEST,
  ACTION_MOVE_NORTHEAST,
  ACTION_MOVE_WEST,
  ACTION_MOVE_EAST,
  ACTION_MOVE_SOUTH,
  ACTION_MOVE_SOUTHWEST,
  ACTION_MOVE_SOUTHEAST,
  ACTION_WAIT,
  INPUTACTION_END // Used to get the length of the enum. Must be the final entry.
};

struct Bind {
  SDL_Keycode key;
  SDL_Keymod modifier;

  bool operator<(const Bind &o) const {
    if (key == o.key) {
      return modifier < o.modifier;
    }
    return key < o.key;
  }

  std::string get_name() {
    std::string bindname = "";
    if (modifier & KMOD_CTRL) bindname += "CTRL+";
    if (modifier & KMOD_ALT) bindname += "ALT+";
    if (modifier & KMOD_SHIFT) bindname += "SHIFT+";
    bindname += SDL_GetKeyName(key);
    return bindname;
  }
};

enum InputEventType {
  INPUT_MOUSE_MOVE_EVENT,
  INPUT_MOUSE_CLICK_EVENT,
  INPUT_KEY_EVENT,
};
struct InputEvent {
  InputEventType type;
  InputAction action;
  bool pressed;

  union {
    struct { // mouse click;
      int x;
      int y;
      int dx;
      int dy;
    } mouse_move_event;

    struct { // mouse click;
      int x;
      int y;
      int button;
    } mouse_click_event;

    struct { // key
      SDL_Keycode key;
      SDL_Keymod mod;
      bool echo;
    } key_press_event;
  };
};

class Input
{
  std::map<Bind, InputAction> binds;
  bool ispressed[INPUTACTION_END];
  bool justpressed[INPUTACTION_END];
  bool justreleased[INPUTACTION_END];
  int mouse_x, mouse_y;
public:
  Input();
  ~Input();
  void new_frame();
  InputEvent set_key(SDL_Keycode key, SDL_Keymod mod, bool pressed);
  InputEvent set_mouse_pos(int x, int y, int dx, int dy);
  InputEvent set_mouse_button(int button, int x, int y, bool pressed);
  void bind_key(SDL_Keycode key, InputAction action, SDL_Keymod mod = KMOD_NONE);
  void unbind_key(SDL_Keycode key, SDL_Keymod mod);
  bool is_pressed(InputAction action);
  bool was_just_pressed(InputAction action);
  bool was_just_released(InputAction action);
  vec2i get_mouse_pos();
};

