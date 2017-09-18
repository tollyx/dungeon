#include "Game.h"

#include "imgui.h"
#include "Input.h"
#include "Renderer.h"

#include "Tilemap.h"
#include "Tileset.h"
#include "Hero.h"
#include "Goblin.h"
#include "Shaman.h"

const int mapwidth = 32;
const std::string map =
"# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #"
"# @ . . . # # # # # # # # . . . . . . . . . . . . . . . . . # #"
"# . . . . . . . . # # # # . # # . # # # # # # . # # # # # . # #"
"# . . . . # # # . . . . . . # . g . # # # # # . # # # . . g . #"
"# . . . . # # # # # # # # . # . . . # # . . . . . . . . . . . #"
"# # # . # # # # # # # # # . . . . g # # . # # # . # # . . g . #"
"# . . . . . . . . . . . . . # # # # # . . . # # . # # # # # # #"
"# . # # # # # # # # . # # . # # # # # . g . # # . # . . g . . #"
"# . . . . g # # . . . # . . . # # # # . . . # # . # . . . . . #"
"# . . g . . # # . # # # . s . . . # # # # # # # . . . . s . . #"
"# . . . . . # # . . . # . . . # . . . . . . . . . # . g . . . #"
"# # . # # # # # . # . # # # # # # # # # . # # # # # # # # . # #"
"# . . . . . . . . # . . . . . . . . . . . . . . . . . . . . . #"
"# . # # # # # # # # # # . # . # # # # # # # # # # . # # # # . #"
"# . . . . . . . . . . . . # . # . . . . # . . . . . # # # . . #"
"# # # # # # . # # # . # # # . # . . . . # . . . # . # # # . # #"
"# . . . . # . # . . . . . # . . . . . . . . . . # . # # . . . #"
"# . . . . # . # . . . . . # . # . . . . # # # # # . . . . . . #"
"# . . . . . . # . . . . . # . # # # # # # . . . . . # # . . . #"
"# . . . . # . # # # # # # # . . . . . . . . # # # # # # # # # #"
"# . . . . # . . . . . . . . . # # # # # # # # # # # # # # # # #"
"# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #"
"# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #"
"# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #"
"# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #"
"# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #"
"# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #"
"# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #"
"# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #"
"# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #"
"# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #"
"# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #"
"# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #";

Renderer* renderer;
Input* input;

Tileset* ascii;
Tilemap* tilemap;
Hero* hero;

bool paused;
bool debug = false;

void init() {
  paused = true;

  if (tilemap != nullptr) {
    delete tilemap;
    tilemap = nullptr;
    hero = nullptr;
  }

  if (hero != nullptr) {
    delete hero;
    hero = nullptr;
  }

  SDL_LogVerbose(SDL_LOG_CATEGORY_SYSTEM, "Creating tilemap...\n");
  tilemap = new Tilemap(32, 32);
  int y = 0;
  int x = 0;
  for (int i = 0; i < map.length(); i++) {

    if (y >= 32) {
      break;
    }
    if (x >= mapwidth) {
      y++;
      x = 0;
    }
    if (map[i] == ' ' || map[i] == '\t' || map[i] == '\n') {
      continue;
    }

    if (map[i] == '@') {
      hero = new Hero(tilemap, vec2i(x, y));
      tilemap->AddActor(hero);
      tilemap->SetTile(x, y, '.');
    }
    else if (map[i] == 'g') {
      tilemap->AddActor(new Goblin(tilemap, vec2i(x, y)));
      tilemap->SetTile(x, y, '.');
    }
    else if (map[i] == 's') {
      tilemap->AddActor(new Shaman(tilemap, vec2i(x, y)));
      tilemap->SetTile(x, y, '.');
    }
    else {
      tilemap->SetTile(x, y, map[i]);
    }
    x++;
  }
  SDL_LogVerbose(SDL_LOG_CATEGORY_SYSTEM, "Done.\n");
  SDL_LogVerbose(SDL_LOG_CATEGORY_SYSTEM, "Calculating initial FOV...\n");
  hero->CalcFOV();
  SDL_LogVerbose(SDL_LOG_CATEGORY_SYSTEM, "Done.\n");
}

void load(Renderer* rend, Input* inp) {
  renderer = rend;
  input = inp;
  SDL_LogVerbose(SDL_LOG_CATEGORY_SYSTEM, "Creating ascii tileset...\n");
  ascii = new Tileset(renderer, "./assets/12x12.bmp", 192, 192, 12, 12);
  SDL_LogVerbose(SDL_LOG_CATEGORY_SYSTEM, "Created ascii tileset.\n");
  input->bindkey(SDLK_r, ACTION_RESET);
  input->bindkey(SDLK_SPACE, ACTION_PAUSE);
  input->bindkey(SDLK_RETURN, ACTION_STEP);
  input->bindkey(SDLK_F1, ACTION_TOGGLE_DEBUG);
  SDL_LogVerbose(SDL_LOG_CATEGORY_SYSTEM, "Keybinds bound.\n");
  init();
}

float delay = .5;
double timer;

void update(double dt) {
  if (input->wasJustPressed(ACTION_TOGGLE_DEBUG)) {
    debug = !debug;
  }
  if (input->wasJustPressed(ACTION_PAUSE)) {
    paused = !paused;
  }
  if (input->wasJustPressed(ACTION_RESET)) {
    init();
  }

  timer += dt;
  if (!paused || input->wasJustPressed(ACTION_STEP)) {
    if (timer >= delay) {
      timer = 0;

      auto actors = tilemap->GetActorList();
      for (Actor* var : *actors) {
        var->Update();
      }
      for (int i = (int)actors->size() - 1; i >= 0; i--) {
        if (!actors->at(i)->alive) {
          if (actors->at(i) == hero) {
            hero = nullptr;
          }
          delete actors->at(i);
          actors->erase(actors->begin() + i);
        }
      }
    }
  }
}

void draw(double alpha) {
  
  
  if (debug) {
    bool wireframe = renderer->IsWireframesEnabled();
    ImGui::Checkbox("Wireframes", &wireframe);
    renderer->SetWireframesEnabled(wireframe);
    bool vsync = renderer->IsVSyncEnabled();
    ImGui::Checkbox("VSync", &vsync);
    renderer->SetVSyncEnabled(vsync);

    ImGui::BeginMainMenuBar();

    if (ImGui::BeginMenu("File")) {
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Edit")) {
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("View")) {
      ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();

    ImGui::SliderFloat("turndelay", &delay, .01f, 1);
  }


  for (int x = 0; x < 32; x++) {
    for (int y = 0; y < 32; y++) {
      if (hero == nullptr || hero->HasSeen(x, y)) {
        renderer->SetColor(1, 1, 1, 1);
        renderer->DrawSprite(ascii->GetSprite(tilemap->GetTile(x, y)), x * 12, y * 12);
        if (hero != nullptr && !hero->CanSee(x, y)) {
          renderer->SetColor(0, 0, 0, .5f);
          renderer->DrawSprite(ascii->GetSprite(219), x * 12, y * 12);
        }
      }
    }
  }
  auto actors = tilemap->GetActorList();
  for (Actor* var : *actors) {
    vec2i pos = var->getPosition();
    if (hero == nullptr || hero->CanSee(pos.x, pos.y)) {
      renderer->SetColor(0, 0, 0, 255);
      renderer->DrawSprite(ascii->GetSprite(219), pos.x * 12, pos.y * 12);

      int sprite;
      switch (var->Type()) {
      case ACT_HERO:
        renderer->SetColor(.2f, .6f, 1, 1);
        sprite = '@';
        break;
      case ACT_GOBLIN:
        renderer->SetColor(.6f, 1, .2f, 1);
        sprite = 'g';
        break;
      case ACT_SHAMAN:
        renderer->SetColor(.2f, 1, .6f, 1);
        sprite = 's';
        break;
      default:
        renderer->SetColor(1, 1, 1, 1);
        sprite = 2;
        break;
      }
      renderer->DrawSprite(ascii->GetSprite(sprite), pos.x * 12, pos.y * 12);
    }
  }
  if (hero != nullptr) {
    renderer->SetColor(155, 5, 5, 255);
    for (int i = 0; i < hero->health; i++) {
      renderer->SetColor(0, 0, 0, 255);
      renderer->DrawSprite(ascii->GetSprite(219), i * 12, 0);
      renderer->SetColor(255, 0, 0, 255);
      renderer->DrawSprite(ascii->GetSprite(3), i * 12, 0);
    }
  }
  if (paused) {
    renderer->SetColor(255, 0, 0, 255);
    renderer->DrawSprite(ascii->GetSprite(219), 12 * 0, 0);
    renderer->DrawSprite(ascii->GetSprite(219), 12 * 1, 0);
    renderer->DrawSprite(ascii->GetSprite(219), 12 * 2, 0);
    renderer->DrawSprite(ascii->GetSprite(219), 12 * 3, 0);
    renderer->DrawSprite(ascii->GetSprite(219), 12 * 4, 0);
    renderer->DrawSprite(ascii->GetSprite(219), 12 * 5, 0);
    renderer->DrawSprite(ascii->GetSprite(219), 12 * 6, 0);
    renderer->DrawSprite(ascii->GetSprite(219), 12 * 7, 0);
    renderer->SetColor(0, 0, 0, 255);
    renderer->DrawSprite(ascii->GetSprite('-'), 12 * 0, 0);
    renderer->DrawSprite(ascii->GetSprite('P'), 12 * 1, 0);
    renderer->DrawSprite(ascii->GetSprite('A'), 12 * 2, 0);
    renderer->DrawSprite(ascii->GetSprite('U'), 12 * 3, 0);
    renderer->DrawSprite(ascii->GetSprite('S'), 12 * 4, 0);
    renderer->DrawSprite(ascii->GetSprite('E'), 12 * 5, 0);
    renderer->DrawSprite(ascii->GetSprite('D'), 12 * 6, 0);
    renderer->DrawSprite(ascii->GetSprite('-'), 12 * 7, 0);
  }
}
