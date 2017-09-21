//
// Created by Adrian on 2017-09-21.
//

#include "PlayState.h"
#include "Input.h"
#include "Renderer.h"
#include "Actor.h"
#include "App.h"
#include "Tilemap.h"
#include "Tileset.h"
#include "FieldOfView.h"
#include "imgui.h"
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

void PlayState::load() {
  SDL_LogVerbose(SDL_LOG_CATEGORY_SYSTEM, "Creating ascii tileset...\n");
  ascii = new Tileset(app->renderer, "./assets/12x12.bmp", 192, 192, 12, 12);
  SDL_LogVerbose(SDL_LOG_CATEGORY_SYSTEM, "Created ascii tileset.\n");

  app->input->bindkey(SDLK_ESCAPE, ACTION_ESCAPE_MENU);
  app->input->bindkey(SDLK_KP_8, ACTION_MOVE_NORTH);
  app->input->bindkey(SDLK_KP_7, ACTION_MOVE_NORTHWEST);
  app->input->bindkey(SDLK_KP_9, ACTION_MOVE_NORTHEAST);
  app->input->bindkey(SDLK_KP_4, ACTION_MOVE_WEST);
  app->input->bindkey(SDLK_KP_6, ACTION_MOVE_EAST);
  app->input->bindkey(SDLK_KP_2, ACTION_MOVE_SOUTH);
  app->input->bindkey(SDLK_KP_1, ACTION_MOVE_SOUTHWEST);
  app->input->bindkey(SDLK_KP_3, ACTION_MOVE_SOUTHEAST);
  app->input->bindkey(SDLK_F1, ACTION_TOGGLE_DEBUG);

  app->input->bindkey(SDLK_r, ACTION_RESET);

  SDL_LogVerbose(SDL_LOG_CATEGORY_SYSTEM, "Keybinds bound.\n");
  new_game();
}

void PlayState::new_game() {
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

  if (fov != nullptr) {
    delete fov;
    fov = nullptr;
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
  fov = new FieldOfView(tilemap);
  fov->calc(hero->getPosition(), 6);
  fov->seen->debug_print();
  SDL_LogVerbose(SDL_LOG_CATEGORY_SYSTEM, "Done.\n");
}

Gamestate *PlayState::update(double delta) {
  timer += delta;
  if (!paused) {
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
      fov->calc(hero->getPosition(), 6);
    }
  }
  return nullptr;
}

void PlayState::draw(double delta) {
  if (debug) {
    bool wireframe = app->renderer->IsWireframesEnabled();
    ImGui::Checkbox("Wireframes", &wireframe);
    app->renderer->SetWireframesEnabled(wireframe);
    bool vsync = app->renderer->IsVSyncEnabled();
    ImGui::Checkbox("VSync", &vsync);
    app->renderer->SetVSyncEnabled(vsync);

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

  vec2i offset;
  offset.x = app->renderer->GetRendererWidth() / 2;
  offset.y = app->renderer->GetRendererHeight() / 2;
  vec2i heropos = hero->getPosition();
  offset.x -= heropos.x * 12;
  offset.y -= heropos.y * 12;

  tilemap->draw(app->renderer, ascii, offset.x, offset.y, fov);

  auto actors = tilemap->GetActorList();
  for (Actor* var : *actors) {
    vec2i pos = var->getPosition();
    if (fov == nullptr || fov->can_see(pos)) {
      app->renderer->SetColor(0, 0, 0, 255);
      app->renderer->DrawSprite(ascii->GetSprite(219), offset.x + pos.x * 12, offset.y + pos.y * 12);

      int sprite;
      switch (var->Type()) {
        case ACT_HERO:
          app->renderer->SetColor(.2f, .6f, 1, 1);
          sprite = '@';
          break;
        case ACT_GOBLIN:
          app->renderer->SetColor(.6f, 1, .2f, 1);
          sprite = 'g';
          break;
        case ACT_SHAMAN:
          app->renderer->SetColor(.2f, 1, .6f, 1);
          sprite = 's';
          break;
        default:
          app->renderer->SetColor(1, 1, 1, 1);
          sprite = 2;
          break;
      }
      app->renderer->DrawSprite(ascii->GetSprite(sprite), offset.x + pos.x * 12, offset.y + pos.y * 12);
    }
  }
  if (hero != nullptr) {
    app->renderer->SetColor(155, 5, 5, 255);
    for (int i = 0; i < hero->health; i++) {
      app->renderer->SetColor(0, 0, 0, 255);
      app->renderer->DrawSprite(ascii->GetSprite(219), i * 12, 0);
      app->renderer->SetColor(255, 0, 0, 255);
      app->renderer->DrawSprite(ascii->GetSprite(3), i * 12, 0);
    }
  }
  if (paused) {
    app->renderer->SetColor(255, 0, 0, 255);
    app->renderer->DrawSprite(ascii->GetSprite(219), 12 * 0, 0);
    app->renderer->DrawSprite(ascii->GetSprite(219), 12 * 1, 0);
    app->renderer->DrawSprite(ascii->GetSprite(219), 12 * 2, 0);
    app->renderer->DrawSprite(ascii->GetSprite(219), 12 * 3, 0);
    app->renderer->DrawSprite(ascii->GetSprite(219), 12 * 4, 0);
    app->renderer->DrawSprite(ascii->GetSprite(219), 12 * 5, 0);
    app->renderer->DrawSprite(ascii->GetSprite(219), 12 * 6, 0);
    app->renderer->DrawSprite(ascii->GetSprite(219), 12 * 7, 0);
    app->renderer->SetColor(0, 0, 0, 255);
    app->renderer->DrawSprite(ascii->GetSprite('-'), 12 * 0, 0);
    app->renderer->DrawSprite(ascii->GetSprite('P'), 12 * 1, 0);
    app->renderer->DrawSprite(ascii->GetSprite('A'), 12 * 2, 0);
    app->renderer->DrawSprite(ascii->GetSprite('U'), 12 * 3, 0);
    app->renderer->DrawSprite(ascii->GetSprite('S'), 12 * 4, 0);
    app->renderer->DrawSprite(ascii->GetSprite('E'), 12 * 5, 0);
    app->renderer->DrawSprite(ascii->GetSprite('D'), 12 * 6, 0);
    app->renderer->DrawSprite(ascii->GetSprite('-'), 12 * 7, 0);
  }
}

void PlayState::quit() {

}

void PlayState::inputevent(InputEvent *event) {
  if (event->type == INPUT_KEY_EVENT && event->pressed) {
    switch (event->action) {
      case ACTION_TOGGLE_DEBUG: debug = !debug; break;
      case ACTION_ESCAPE_MENU: paused = !paused; break;
      case ACTION_RESET: new_game(); break;
      default: break;
    }
  }
}
