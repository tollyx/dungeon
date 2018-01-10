#include <SDL2/SDL.h>
#include "App.h"

#undef main
int main(int argc, char* argv[]) {
  App app{};
  int err = 1;
  if (app.init()) {
    err = app.start();
  }
  return err;
}
