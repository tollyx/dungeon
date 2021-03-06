//
// Created by Adrian on 2017-09-19.
//

#include "App.h"
#include <SDL2/SDL.h>
#include <ctime>
#include <fstream>
#include <string>
#include <kaguya/kaguya.hpp>
#include "Renderer.h"
#include "imgui/imgui.h"
#include "Input.h"
#include "PlayState.h"
#include "LuaHandler.h"
#include "Time.h"
#include <memory>

Uint64 perfFreq;
double currTime() {
  return SDL_GetPerformanceCounter() / (double)perfFreq;
}

struct Config {
  struct {
    bool fullscreen = false;
    int width = 792;
    int height = 600;
  } window;
  struct {
    bool vsync = false;
    bool wireframes = false;
  } gfx;
} config;

void load_config() {
  kaguya::State state;
  state["dungeon"] = kaguya::NewTable();
  if (state.dofile("config.lua")) {
    state(R"lua(
local cfg = {
	window = {
		fullscreen = false,
		width = 792,
		height = 600,
	},
	gfx = {
		vsync = false,
		wireframes = false,
	}
}

if type(dungeon.config) == "function" then
  	dungeon.config(cfg)
end

config = cfg
)lua");
    config.gfx.vsync = state["config"]["gfx"]["vsync"];
    config.gfx.wireframes = state["config"]["gfx"]["wireframes"];
    config.window.fullscreen = state["config"]["window"]["fullscreen"];
    config.window.width = state["config"]["window"]["width"];
    config.window.height = state["config"]["window"]["height"];
  }
}

bool App::init() {
  //setenv("MESA_DEBUG", "", 0);

  int err = 0;
  err = SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);
  SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);
  if (err != 0) {
    const char* error = SDL_GetError();
    fprintf(stderr, error);
    SDL_Quit();
    return false;
  }
  SDL_LogVerbose(SDL_LOG_CATEGORY_SYSTEM,"SDL initialized.\n");

  
  load_config();
  SDL_LogVerbose(SDL_LOG_CATEGORY_SYSTEM, "Config Loaded.\n");

  renderer = new Renderer();
  std::string title = "Dungeon ";
  title += version;
  if (!renderer->Init(title, config.window.width, config.window.height)) {
    const char* error = SDL_GetError();
    SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "%s", error);
    SDL_ShowSimpleMessageBox(0, "Error", error, nullptr);
    SDL_Quit();
    return false;
  }

  renderer->set_vsync_enabled(config.gfx.vsync);
  renderer->set_wireframes_enabled(config.gfx.wireframes);

  input = new Input();
  srand(static_cast<unsigned int>(time(nullptr)));
  perfFreq = SDL_GetPerformanceFrequency();
  SDL_LogVerbose(SDL_LOG_CATEGORY_SYSTEM, "App initialized.\n");
  return true;
}

int App::start() {
  double dt = 1.0/30;
  double currentTime = currTime();
  double accumulator = dt;

  bool show_log = false;
  running = true;
  statestack.push(std::unique_ptr<IState>(new PlayState(this)));

  while (running) {
    double newTime = currTime();
    double frametime = newTime - currentTime;
    if (frametime > 1.0/30) {
      frametime = 1.0/30;
    }

    currentTime = newTime;
    accumulator += frametime;

    SDL_Event ev{};
    ImGuiIO &io = ImGui::GetIO();

    renderer->ImguiNewFrame();
    while (SDL_PollEvent(&ev)) {
      //renderer->ImguiProcessEvents(&ev);
      switch (ev.type) {
        case SDL_WINDOWEVENT:
          switch (ev.window.event) {
            case SDL_WINDOWEVENT_CLOSE:
              running = false;
              break;
            case SDL_WINDOWEVENT_RESIZED:
              renderer->set_window_size(ev.window.data1, ev.window.data2);
              break;
            default:
              break;
          }
          break;
        case SDL_MOUSEMOTION:
          if (!io.WantCaptureMouse) {
            InputEvent inputEvent = input->set_mouse_pos(ev.motion.x, ev.motion.y, ev.motion.xrel, ev.motion.yrel);
            statestack.input(inputEvent);
          }
          break;
        case SDL_MOUSEBUTTONDOWN:
          if (!io.WantCaptureMouse) {
            InputEvent inputEvent = input->set_mouse_button(ev.button.button, ev.button.x, ev.button.y, true);
            statestack.input(inputEvent);
          }
          break;
        case SDL_MOUSEBUTTONUP:
          if (!io.WantCaptureMouse) {
            InputEvent inputEvent = input->set_mouse_button(ev.button.button, ev.button.x, ev.button.y, false);
            statestack.input(inputEvent);
          }
          break;
        case SDL_KEYDOWN:
          if (!io.WantCaptureKeyboard) {
            InputEvent inputEvent = input->set_key(ev.key.keysym.sym, (SDL_Keymod) ev.key.keysym.mod, true);
            statestack.input(inputEvent);
          }
          break;
        case SDL_KEYUP:
          if (!io.WantCaptureKeyboard){
            InputEvent inputEvent = input->set_key(ev.key.keysym.sym, (SDL_Keymod) ev.key.keysym.mod, false);
            statestack.input(inputEvent);
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
      statestack.update(dt);
      input->new_frame();
      Time::tick_timers(dt);
      accumulator -= dt;
    }
    renderer->Clear();
    statestack.draw();
    renderer->Present();
    SDL_Delay(1);
  }
  delete renderer;
  //ImGui::Shutdown();
  SDL_Quit();

  SDL_LogVerbose(SDL_LOG_CATEGORY_SYSTEM, "Quit.");
  return 0;
}
