//#define _ALLOW_ITERATOR_DEBUG_LEVEL_MISMATCH
//#define _ITERATOR_DEBUG_LEVEL 0  


#include <SDL.h>
#include <stdio.h>
#include <math.h>
#include <string>
#include <time.h>

#include "Renderer.h"
#include "Input.h"
#include "Config.h"
#include "Game.h"

#include "imgui.h"

Uint64 perfFreq;

double currTime();

#undef main
int main(int argc, char* argv[]) {
  Renderer* renderer;
  Input* input;

  SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);

  Config cfg = Config("dungeon.cfg");
  cfg.load();

  int windowWidth = cfg.getInt("ResolutionX", 1280);
  int windowHeight = cfg.getInt("ResolutionY", 720);
  bool vsync = cfg.getBool("VSync", false);
  bool wireframe = cfg.getBool("Wireframes", false);

  cfg.save();

  int err = 0;
  err = SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);
  if (err != 0) {
    const char* error = SDL_GetError();
    SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "%s", error);
    SDL_Quit();
    return 1;
  }
  SDL_Log("SDL initialized.\n");

  renderer = new Renderer();
  if (!renderer->Init("Dungeon", windowWidth, windowHeight)) {
    const char* error = SDL_GetError();
    SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "%s", error);
    SDL_ShowSimpleMessageBox(0, "Error", error, nullptr);
    SDL_Quit();
    return 1;
  }

  renderer->SetVSyncEnabled(vsync);
  renderer->SetWireframesEnabled(wireframe);

  input = new Input();
  srand(time(nullptr));
  load(renderer, input);

  perfFreq = SDL_GetPerformanceFrequency();

  double dt = 1.0/30;
  double currentTime = currTime();
  double accumulator = dt;

  int currentframe = 0;
  bool running = true;

  while (running) {
    double newTime = currTime();
    double frametime = newTime - currentTime;
    if (frametime > 1.0/30) {
      frametime = 1.0/30;
    }
    currentTime = newTime;

    accumulator += frametime;

    //dt = (SDL_GetTicks() - currentframe) * 0.001f;
    currentframe = SDL_GetTicks();
    
    SDL_Event ev;
    ImGuiIO &io = ImGui::GetIO();
    renderer->ImguiNewFrame();
    while (SDL_PollEvent(&ev)) {
      renderer->ImguiProcessEvents(&ev);
      switch (ev.type) {
      case SDL_WINDOWEVENT:
        switch (ev.window.event) {
        case SDL_WINDOWEVENT_CLOSE:
          running = false;
          break;
        default:
          break;
        }
        break;
      case SDL_KEYDOWN:
        if (!io.WantCaptureKeyboard)
          input->setkey(ev.key.keysym.sym, (SDL_Keymod)ev.key.keysym.mod, true);
        break;
      case SDL_KEYUP:
        if (!io.WantCaptureKeyboard)
          input->setkey(ev.key.keysym.sym, (SDL_Keymod)ev.key.keysym.mod, false);
        break;
      case SDL_QUIT:
        running = false;
        break;
      default:
        break;
      }
    }

    while (running && accumulator >= dt) {
      update(dt);
      input->newframe();

      accumulator -= dt;
    }

    renderer->Clear();
    draw(accumulator / dt);
    renderer->Present();
    SDL_Delay(1);
  }
  delete renderer;
  SDL_Quit();
  SDL_LogVerbose(SDL_LOG_CATEGORY_SYSTEM, "Quit.");
  return 0;
}

double currTime() {
  return SDL_GetPerformanceCounter() / (double)perfFreq;
}
