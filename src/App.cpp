//
// Created by Adrian on 2017-09-19.
//

#include "App.h"
#include <SDL.h>
#include <ctime>
#include "Config.h"
#include "Renderer.h"
#include "imgui.h"
#include "Input.h"
#include "Gamestate.h"
#include "PlayState.h"

Uint64 perfFreq;
double currTime() {
  return SDL_GetPerformanceCounter() / (double)perfFreq;
}

bool App::init() {
  //setenv("MESA_DEBUG", "", 0);

  Config cfg = Config("dungeon.cfg");
  cfg.load();

  int windowWidth = cfg.getInt("ResolutionX", 1280);
  int windowHeight = cfg.getInt("ResolutionY", 720);
  bool vsync = cfg.getBool("VSync", false);
  bool wireframe = cfg.getBool("Wireframes", false);

  cfg.save();

  int err = 0;
  err = SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);
  SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
  if (err != 0) {
    const char* error = SDL_GetError();
    fprintf(stderr, error);
    SDL_Quit();
    return false;
  }
  SDL_LogVerbose(SDL_LOG_CATEGORY_SYSTEM,"SDL initialized.\n");

  renderer = new Renderer();
  if (!renderer->Init("Dungeon", windowWidth, windowHeight)) {
    const char* error = SDL_GetError();
    SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "%s", error);
    SDL_ShowSimpleMessageBox(0, "Error", error, nullptr);
    SDL_Quit();
    return false;
  }

  renderer->set_vsync_enabled(vsync);
  renderer->set_wireframes_enabled(wireframe);

  input = new Input();
  srand(static_cast<unsigned int>(time(nullptr)));
  perfFreq = SDL_GetPerformanceFrequency();
  return true;
}

int App::start() {
  current = new PlayState();
  current->init(this);
  current->load();

  double dt = 1.0/30;
  double currentTime = currTime();
  double accumulator = dt;

  bool running = true;
  Gamestate* nextstate = nullptr;

  while (running) {
    double newTime = currTime();
    double frametime = newTime - currentTime;
    if (frametime > 1.0/30) {
      frametime = 1.0/30;
    }

    if (nextstate != nullptr) {
      current->quit();
      delete current;
      current = nextstate;
      current->init(this);
      current->load();
    }

    currentTime = newTime;
    accumulator += frametime;

    SDL_Event ev{};
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
        case SDL_MOUSEMOTION:
          if (!io.WantCaptureMouse) {
            InputEvent inputEvent = input->set_mouse_pos(ev.motion.x, ev.motion.y, ev.motion.xrel, ev.motion.yrel);
            current->inputevent(&inputEvent);
          }
          break;
        case SDL_MOUSEBUTTONDOWN:
          if (!io.WantCaptureMouse) {
            InputEvent inputEvent = input->set_mouse_button(ev.button.button, ev.button.x, ev.button.y, true);
            current->inputevent(&inputEvent);
          }
          break;
        case SDL_MOUSEBUTTONUP:
          if (!io.WantCaptureMouse) {
            InputEvent inputEvent = input->set_mouse_button(ev.button.button, ev.button.x, ev.button.y, false);
            current->inputevent(&inputEvent);
          }
          break;
        case SDL_KEYDOWN:
          if (!io.WantCaptureKeyboard) {
            InputEvent inputEvent = input->set_key(ev.key.keysym.sym, (SDL_Keymod) ev.key.keysym.mod, true);
            current->inputevent(&inputEvent);
          }
          break;
        case SDL_KEYUP:
          if (!io.WantCaptureKeyboard){
            InputEvent inputEvent = input->set_key(ev.key.keysym.sym, (SDL_Keymod) ev.key.keysym.mod, false);
            current->inputevent(&inputEvent);
          }
          break;
        case SDL_QUIT:
          running = false;
          break;
        default:
          break;
      }
    }
    while (running && accumulator >= dt) {
      nextstate = current->update(dt);
      input->new_frame();

      accumulator -= dt;
    }

    renderer->Clear();
    current->draw(accumulator / dt);
    renderer->Present();
    SDL_Delay(1);
  }
  delete renderer;
  SDL_Quit();
  SDL_LogVerbose(SDL_LOG_CATEGORY_SYSTEM, "Quit.");
  return 0;
}
