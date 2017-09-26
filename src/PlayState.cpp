//
// Created by Adrian on 2017-09-21.
//

#include "PlayState.h"
#include "Input.h"
#include "Renderer.h"
#include "Actor.h"
#include "App.h"
#include "Tileset.h"
#include "Tilemap.h"
#include "FieldOfView.h"
#include "imgui.h"
#include "Hero.h"
#include "Goblin.h"
#include "Shaman.h"

const int mapwidth = 32;

InputAction action;

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
  app->input->bind_key(SDLK_KP_5, ACTION_WAIT);
  app->input->bind_key(SDLK_UP, ACTION_MOVE_NORTH);
  app->input->bind_key(SDLK_DOWN, ACTION_MOVE_SOUTH);
  app->input->bind_key(SDLK_LEFT, ACTION_MOVE_WEST);
  app->input->bind_key(SDLK_RIGHT, ACTION_MOVE_EAST);
  app->input->bind_key(SDLK_F1, ACTION_TOGGLE_DEBUG);

  app->input->bind_key(SDLK_r, ACTION_RESET);

  SDL_LogVerbose(SDL_LOG_CATEGORY_SYSTEM, "Keybinds bound.\n");
  new_game();
}

void PlayState::new_game() {
  action = ACTION_NONE;

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

  std::string map =
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

  SDL_LogVerbose(SDL_LOG_CATEGORY_SYSTEM, "Creating tilemap...\n");
  tilemap = new Tilemap(32, 32);
  int y = 0;
  int x = 0;
  for (char i : map) {

    if (y >= 32) {
      break;
    }
    if (x >= mapwidth) {
      y++;
      x = 0;
    }
    if (i == ' ' || i == '\t' || i == '\n') {
      continue;
    }

    if (i == '@') {
      hero = new Hero(tilemap, vec2i(x, y));
      tilemap->add_entity(hero);
      tilemap->set_tile(x, y, '.');
    }
    else if (i == 'g') {
      tilemap->add_entity(new Goblin(tilemap, vec2i(x, y)));
      tilemap->set_tile(x, y, '.');
    }
    else if (i == 's') {
      tilemap->add_entity(new Shaman(tilemap, vec2i(x, y)));
      tilemap->set_tile(x, y, '.');
    }
    else {
      tilemap->set_tile(x, y, i);
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
  if (action != ACTION_NONE) {
    if (hero && hero->is_alive()) {
      vec2i dir;
      switch (action) {
        case ACTION_MOVE_NORTH: dir = {0, -1}; break;
        case ACTION_MOVE_NORTHWEST: dir = {-1, -1}; break;
        case ACTION_MOVE_NORTHEAST: dir = {1, -1}; break;
        case ACTION_MOVE_WEST: dir = {-1, 0}; break;
        case ACTION_MOVE_EAST: dir = {1, 0}; break;
        case ACTION_MOVE_SOUTH: dir = {0, 1}; break;
        case ACTION_MOVE_SOUTHWEST: dir = {-1, 1}; break;
        case ACTION_MOVE_SOUTHEAST: dir = {1, 1}; break;
        case ACTION_WAIT: dir = {0, 0}; break;
        default: action = ACTION_NONE; return nullptr; // abort turn
      }
      if (dir != vec2i(0,0)) {
        if (!hero->move(dir.x, dir.y)) {
          vec2i heropos = hero->get_position();
          auto acts = tilemap->get_entities(heropos.x + dir.x, heropos.y + dir.y, 0, ENTITY_ACTOR);
          if(acts.empty()) {
            return nullptr; // unable to move and nothing to attack == abort turn
          }
          for (auto ent : acts) {
            auto act = (Actor*)ent;
            if (act->is_alive() && act->get_actor_team() != hero->get_actor_team()) {
              hero->attack(act);
              break;
            }
          }
        }
      }
      hero->update();
      fov->calc(hero->get_position(), 6);
    }

    auto actors = tilemap->get_entity_list();
    for (Entity* var : *actors) {
      if (var == hero) continue;
      if (var->entity_type() == ENTITY_ACTOR) {
        ((Actor*)var)->update();
      }
    }
    /* // We got enough memory, we can leave the corpses on the field.
    unsigned int actor_size = actors->size();
    for (unsigned int i = actor_size - 1; i <= actor_size; i--) { // Woo unsigned int underflow abuse!
      if (!actors->at(i)->is_alive()) {
        if (actors->at(i) == hero) {
          hero = nullptr;
        }
        delete actors->at(i);
        actors->erase(actors->begin() + i);
      }
    }
     */
    action = ACTION_NONE;
  }
  return nullptr;
}

bool debug_actors = false;
bool debug_settings = false;

void PlayState::draw(double delta) {
  if (debug) {
    {
      ImGui::BeginMainMenuBar();

      if (ImGui::BeginMenu("View")) {
        ImGui::MenuItem("Settings", nullptr, &debug_settings);
        ImGui::MenuItem("Actors", nullptr, &debug_actors);
        ImGui::EndMenu();
      }

      ImGui::EndMainMenuBar();
    }
    if (debug_settings) {
      ImGui::Begin("Settings", &debug_settings);

      bool wireframe = app->renderer->is_wireframes_enabled();
      ImGui::Checkbox("Wireframes", &wireframe);
      app->renderer->set_wireframes_enabled(wireframe);
      bool vsync = app->renderer->is_vsync_enabled();
      ImGui::Checkbox("VSync", &vsync);
      app->renderer->set_vsync_enabled(vsync);

      ImGui::End();
    }
    if (debug_actors) {
      ImGui::Begin("Actors", &debug_actors);

      auto actors = tilemap->get_entity_list();
      const char* headers[] {
          "id", "name", "health", "strength"
      };
      static float widths[4]{};
      ImGui::BeginTable("ActorColumns", headers, widths, 4);
      for (Entity* ent : *actors) {
        if (ent->entity_type() == ENTITY_ACTOR) {
          auto act = (Actor*) ent;
          ImGui::Text("%d", act->id); ImGui::NextColumn();
          ImGui::Text(act->name.c_str()); ImGui::NextColumn();
          ImGui::Text("%d/%d", act->get_health(), act->get_health_max()); ImGui::NextColumn();
          ImGui::Text("%d", act->get_strength()); ImGui::NextColumn();
        }
      }
      ImGui::EndTable();

      ImGui::End();
    }
  }

  vec2i asciisize = {
      ascii->get_tile_width(),
      ascii->get_tile_height(),
  };
  vec2i tilesize = {
      app->renderer->get_renderer_width() / ascii->get_tile_width(),
      app->renderer->get_renderer_height() / ascii->get_tile_height(),
  };
  vec2i margin = {
      (app->renderer->get_renderer_width() - tilesize.x * asciisize.x)/2,
      (app->renderer->get_renderer_height() - tilesize.y * asciisize.y)/2,
  };
  vec2i heropos = {0,0};
  if (hero) {
    heropos = hero->get_position();
  }
  vec2i offset = {
      (tilesize.x/2-heropos.x),
      (tilesize.y/2-heropos.y),
  };

  tilemap->draw(app->renderer, ascii, margin.x, margin.y, -offset.x, -offset.y, tilesize.x, tilesize.y, fov);

  auto entities = tilemap->get_entity_list();
  for (Entity* var : *entities) {
    vec2i pos = var->get_position();
    if (fov == nullptr || fov->can_see(pos)) {
      app->renderer->set_color(0, 0, 0, 255);
      app->renderer->draw_sprite(ascii->get_sprite(219), margin.x + (offset.x + pos.x) * asciisize.x, margin.y + (offset.y + pos.y) * asciisize.y);

      int sprite = var->get_sprite_id();
      app->renderer->set_color(1, 1, 1, 1);

      app->renderer->draw_sprite(ascii->get_sprite(sprite), margin.x + (offset.x + pos.x) * asciisize.x, margin.y + (offset.y + pos.y) * asciisize.y);
    }
  }
  if (hero != nullptr) {
    app->renderer->set_color(155, 5, 5, 255);
    for (int i = 0; i < hero->get_health(); i++) {
      app->renderer->set_color(0, 0, 0, 255);
      app->renderer->draw_sprite(ascii->get_sprite(219), (i+1) * asciisize.x, asciisize.y);
      app->renderer->set_color(255, 0, 0, 255);
      app->renderer->draw_sprite(ascii->get_sprite(3), (i+1) * asciisize.x, asciisize.y);
    }
  }
}

void PlayState::quit() {
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
}

void PlayState::inputevent(InputEvent *event) {
  if (event->type == INPUT_KEY_EVENT && event->pressed) {
    switch (event->action) {
      case ACTION_TOGGLE_DEBUG: debug = !debug; break;
      case ACTION_RESET: new_game(); break;
      case ACTION_ESCAPE_MENU: break; // TODO
      case ACTION_NONE: break;
      default: action = event->action; break;
    }
  }
}
