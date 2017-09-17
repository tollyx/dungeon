#pragma once

#include <SDL.h>
#include <map>

enum InputAction {
  ACTION_NONE,
  ACTION_PAUSE,
  ACTION_RESET,
  ACTION_STEP,
  ACTION_TOGGLE_DEBUG,
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
};

class Input
{
  std::map<Bind, InputAction> binds;
  bool ispressed[INPUTACTION_END];
  bool justpressed[INPUTACTION_END];
  bool justreleased[INPUTACTION_END];
public:
  Input();
  ~Input();
  void newframe();
  void setkey(SDL_Keycode key, SDL_Keymod mod, bool pressed);
  void bindkey(SDL_Keycode key, InputAction action, SDL_Keymod mod = KMOD_NONE);
  void unbindkey(SDL_Keycode key, SDL_Keymod mod);
  bool isPressed(InputAction action);
  bool wasJustPressed(InputAction action);
  bool wasJustReleased(InputAction action);
};

