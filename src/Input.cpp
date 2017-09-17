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

void Input::newframe()
{
  for(int i = 0; i < INPUTACTION_END; i++)
  {
    justpressed[i] = false;
    justreleased[i] = false;
  }
}

void Input::setkey(SDL_Keycode key, SDL_Keymod mod, bool pressed)
{
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
        
    if (ispressed[action] != pressed) // Ignore keypress/release if it's already pressed/released, this disables repeats.
    {
      ispressed[action] = pressed;
      if (pressed)
      {
        justpressed[action] = true;
      }
      else
      {
        justreleased[action] = true;
      }
    }
  }
}

void Input::bindkey(SDL_Keycode key, InputAction action, SDL_Keymod mod)
{
  Bind bind = { key, mod };
  binds[bind] = action;
}

void Input::unbindkey(SDL_Keycode key, SDL_Keymod mod)
{
  Bind bind = { key, mod };
  binds.erase(bind);
}

bool Input::isPressed(InputAction action)
{
  return ispressed[action];
}

bool Input::wasJustPressed(InputAction action)
{
  return justpressed[action];
}

bool Input::wasJustReleased(InputAction action)
{
  return justreleased[action];
}
