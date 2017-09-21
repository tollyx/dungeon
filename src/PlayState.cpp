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

  app->input->bind_key(SDLK_ESCAPE, ACTION_ESCAPE_MENU);
  app->input->bind_key(SDLK_KP_8, ACTION_MOVE_NORTH);
  app->input->bind_key(SDLK_KP_7, ACTION_MOVE_NORTHWEST);
  app->input->bind_key(SDLK_KP_9, ACTION_MOVE_NORTHEAST);
  app->input->bind_key(SDLK_KP_4, ACTION_MOVE_WEST);
  app->input->bind_key(SDLK_KP_6, ACTION_MOVE_EAST);
  app->input->bind_key(SDLK_KP_2, ACTION_MOVE_SOUTH);
  app->input->bind_key(SDLK_KP_1, ACTION_MOVE_SOUTHWEST);
  app->input->bind_key(SDLK_KP_3, ACTION_MOVE_SOUTHEAST);
  app->input->bind_key(SDLK_F1, ACTION_TOGGLE_DEBUG);

  app->input->bind_key(SDLK_r, ACTION_RESET);

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
      tilemap->add_actor(hero);
      tilemap->set_tile(x, y, '.');
    }
    else if (map[i] == 'g') {
      tilemap->add_actor(new Goblin(tilemap, vec2i(x, y)));
      tilemap->set_tile(x, y, '.');
    }
    else if (map[i] == 's') {
      tilemap->add_actor(new Shaman(tilemap, vec2i(x, y)));
      tilemap->set_tile(x, y, '.');
    }
    else {
      tilemap->set_tile(x, y, map[i]);
    }
    x++;
  }
  SDL_LogVerbose(SDL_LOG_CATEGORY_SYSTEM, "Done.\n");
  SDL_LogVerbose(SDL_LOG_CATEGORY_SYSTEM, "Calculating initial FOV...\n");
  fov = new FieldOfView(tilemap);
  fov->calc(hero->get_position(), 6);
  SDL_LogVerbose(SDL_LOG_CATEGORY_SYSTEM, "Done.\n");
}

Gamestate *PlayState::update(double delta) {
  timer += delta;
  if (!paused) {
    if (timer >= delay) {
      timer = 0;

      auto actors = tilemap->get_actor_list();
      for (Actor* var : *actors) {
        var->update();
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
      fov->calc(hero->get_position(), 6);
    }
  }
  return nullptr;
}

void PlayState::draw(double delta) {
  if (debug) {
    bool wireframe = app->renderer->is_wireframes_enabled();
    ImGui::Checkbox("Wireframes", &wireframe);
    app->renderer->set_wireframes_enabled(wireframe);
    bool vsync = app->renderer->is_vsync_enabled();
    ImGui::Checkbox("VSync", &vsync);
    app->renderer->set_vsync_enabled(vsync);

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
  offset.x = app->renderer->get_renderer_width() / 2;
  offset.y = app->renderer->get_renderer_height() / 2;
  vec2i heropos = hero->get_position();
  offset.x -= heropos.x * 12;
  offset.y -= heropos.y * 12;

  tilemap->draw(app->renderer, ascii, offset.x, offset.y, fov);

  auto actors = tilemap->get_actor_list();
  for (Actor* var : *actors) {
    vec2i pos = var->get_position();
    if (fov == nullptr || fov->can_see(pos)) {
      app->renderer->set_color(0, 0, 0, 255);
      app->renderer->draw_sprite(ascii->get_sprite(219), offset.x + pos.x * 12, offset.y + pos.y * 12);

      int sprite;
      switch (var->Type()) {
        case ACT_HERO:
          app->renderer->set_color(.2f, .6f, 1, 1);
          sprite = '@';
          break;
        case ACT_GOBLIN:
          app->renderer->set_color(.6f, 1, .2f, 1);
          sprite = 'g';
          break;
        case ACT_SHAMAN:
          app->renderer->set_color(.2f, 1, .6f, 1);
          sprite = 's';
          break;
        default:
          app->renderer->set_color(1, 1, 1, 1);
          sprite = 2;
          break;
      }
      app->renderer->draw_sprite(ascii->get_sprite(sprite), offset.x + pos.x * 12, offset.y + pos.y * 12);
    }
  }
  if (hero != nullptr) {
    app->renderer->set_color(155, 5, 5, 255);
    for (int i = 0; i < hero->health; i++) {
      app->renderer->set_color(0, 0, 0, 255);
      app->renderer->draw_sprite(ascii->get_sprite(219), i * 12, 0);
      app->renderer->set_color(255, 0, 0, 255);
      app->renderer->draw_sprite(ascii->get_sprite(3), i * 12, 0);
    }
  }
  if (paused) {
    app->renderer->set_color(255, 0, 0, 255);
    app->renderer->draw_sprite(ascii->get_sprite(219), 12 * 0, 0);
    app->renderer->draw_sprite(ascii->get_sprite(219), 12 * 1, 0);
    app->renderer->draw_sprite(ascii->get_sprite(219), 12 * 2, 0);
    app->renderer->draw_sprite(ascii->get_sprite(219), 12 * 3, 0);
    app->renderer->draw_sprite(ascii->get_sprite(219), 12 * 4, 0);
    app->renderer->draw_sprite(ascii->get_sprite(219), 12 * 5, 0);
    app->renderer->draw_sprite(ascii->get_sprite(219), 12 * 6, 0);
    app->renderer->draw_sprite(ascii->get_sprite(219), 12 * 7, 0);
    app->renderer->set_color(0, 0, 0, 255);
    app->renderer->draw_sprite(ascii->get_sprite('-'), 12 * 0, 0);
    app->renderer->draw_sprite(ascii->get_sprite('P'), 12 * 1, 0);
    app->renderer->draw_sprite(ascii->get_sprite('A'), 12 * 2, 0);
    app->renderer->draw_sprite(ascii->get_sprite('U'), 12 * 3, 0);
    app->renderer->draw_sprite(ascii->get_sprite('S'), 12 * 4, 0);
    app->renderer->draw_sprite(ascii->get_sprite('E'), 12 * 5, 0);
    app->renderer->draw_sprite(ascii->get_sprite('D'), 12 * 6, 0);
    app->renderer->draw_sprite(ascii->get_sprite('-'), 12 * 7, 0);
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
