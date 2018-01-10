//
// Created by Adrian on 2017-09-21.
//

#include "PlayState.h"
#include "Input.h"
#include "Renderer.h"
#include "Actor.h"
#include "App.h"
#include "SpriteAtlas.h"
#include "Mapgen.h"
#include "FieldOfView.h"
#include "imgui.h"
#include "Hero.h"
#include "Goblin.h"
#include "Shaman.h"
#include "Rng.h"
#include "TileSet.h"
#include <kaguya/kaguya.hpp>

InputAction player_action;
TileSet tileset;

void PlayState::load() {
  SDL_LogVerbose(SDL_LOG_CATEGORY_SYSTEM, "Creating ascii tileset...\n");
  ascii = new SpriteAtlas(app->renderer, "./assets/12x12.bmp", 192, 192, 12, 12);
  SDL_LogVerbose(SDL_LOG_CATEGORY_SYSTEM, "Created ascii tileset.\n");

  app->input->bind_key(SDLK_ESCAPE, ACTION_ESCAPE_MENU);

  kaguya::State lua;
  lua["tiles"] = kaguya::NewTable();
  lua(R"LUA(
actors = dofile('data/actors.lua')
tiles = dofile('data/tiles.lua')
)LUA");
  tileset.load_from_table(lua.globalTable().getField("tiles"));

  // Movement: keypad
  app->input->bind_key(SDLK_KP_8, ACTION_MOVE_NORTH);
  app->input->bind_key(SDLK_KP_7, ACTION_MOVE_NORTHWEST);
  app->input->bind_key(SDLK_KP_9, ACTION_MOVE_NORTHEAST);
  app->input->bind_key(SDLK_KP_4, ACTION_MOVE_WEST);
  app->input->bind_key(SDLK_KP_6, ACTION_MOVE_EAST);
  app->input->bind_key(SDLK_KP_2, ACTION_MOVE_SOUTH);
  app->input->bind_key(SDLK_KP_1, ACTION_MOVE_SOUTHWEST);
  app->input->bind_key(SDLK_KP_3, ACTION_MOVE_SOUTHEAST);
  app->input->bind_key(SDLK_KP_5, ACTION_WAIT);

  // Movement: arrow-keys
  app->input->bind_key(SDLK_UP, ACTION_MOVE_NORTH);
  app->input->bind_key(SDLK_DOWN, ACTION_MOVE_SOUTH);
  app->input->bind_key(SDLK_LEFT, ACTION_MOVE_WEST);
  app->input->bind_key(SDLK_RIGHT, ACTION_MOVE_EAST);

  // Movement: vim-keys
  app->input->bind_key(SDLK_k, ACTION_MOVE_NORTH);
  app->input->bind_key(SDLK_y, ACTION_MOVE_NORTHWEST);
  app->input->bind_key(SDLK_u, ACTION_MOVE_NORTHEAST);
  app->input->bind_key(SDLK_h, ACTION_MOVE_WEST);
  app->input->bind_key(SDLK_l, ACTION_MOVE_EAST);
  app->input->bind_key(SDLK_j, ACTION_MOVE_SOUTH);
  app->input->bind_key(SDLK_n, ACTION_MOVE_SOUTHWEST);
  app->input->bind_key(SDLK_m, ACTION_MOVE_SOUTHEAST);

  // General
  app->input->bind_key(SDLK_PERIOD, ACTION_WAIT);

  // debug
  app->input->bind_key(SDLK_F1, ACTION_TOGGLE_DEBUG);
  app->input->bind_key(SDLK_r, ACTION_RESET);

  SDL_LogVerbose(SDL_LOG_CATEGORY_SYSTEM, "Keybinds bound.\n");
  new_game();
}

void PlayState::new_game() {
  player_action = ACTION_NONE;

  tilemap.delete_actors();
  player_actor = nullptr;

  SDL_LogVerbose(SDL_LOG_CATEGORY_SYSTEM, "Creating tilemap...\n");

  Rng rng;
  tilemap = generate_dungeon(48, 48, tileset);
  vec2i heropos;
  Tile t;
  do {
    heropos.x = rng.get_int(1, tilemap.get_width() - 1);
    heropos.y = rng.get_int(1, tilemap.get_width() - 1);
    t = tilemap.get_tile(heropos.x, heropos.y);
  } while (!t.passable);
  SDL_LogVerbose(SDL_LOG_CATEGORY_SYSTEM, "Done.\n");
  SDL_LogVerbose(SDL_LOG_CATEGORY_SYSTEM, "Calculating initial FOV...\n");
  fov = FieldOfView(&tilemap);
  SDL_LogVerbose(SDL_LOG_CATEGORY_SYSTEM, "Done.\n");

  current_entity_index = 0;
  Actor* actor = tilemap.get_actor_list()->at(current_entity_index);
  is_player_turn = actor->player_controlled;
  if (is_player_turn) { 
    camera_pos = actor->get_position();
    fov.calc(actor->get_position(), 6);
  }
}

Gamestate *PlayState::update(double delta) {
  while (!is_player_turn || player_action != ACTION_NONE) {
    std::vector<Actor*>* actors = tilemap.get_actor_list();
    Actor* actor = actors->at(current_entity_index);

    if (is_player_turn && actor->is_alive()) {
      vec2i dir;
      switch (player_action) {
        case ACTION_MOVE_NORTH: dir = {0, -1}; break;
        case ACTION_MOVE_NORTHWEST: dir = {-1, -1}; break;
        case ACTION_MOVE_NORTHEAST: dir = {1, -1}; break;
        case ACTION_MOVE_WEST: dir = {-1, 0}; break;
        case ACTION_MOVE_EAST: dir = {1, 0}; break;
        case ACTION_MOVE_SOUTH: dir = {0, 1}; break;
        case ACTION_MOVE_SOUTHWEST: dir = {-1, 1}; break;
        case ACTION_MOVE_SOUTHEAST: dir = {1, 1}; break;
        case ACTION_WAIT: dir = {0, 0}; break;
        default: player_action = ACTION_NONE; SDL_LogVerbose(SDL_LOG_CATEGORY_SYSTEM, "Turn aborted: no player action.\n"); return nullptr; // abort turn
      }
      if (dir != vec2i(0,0)) {
        if (!actor->move(dir.x, dir.y, &tilemap)) {
          vec2i heropos = actor->get_position();
          Actor* act = tilemap.get_actor(heropos.x + dir.x, heropos.y + dir.y);
          if(act == nullptr) {
            SDL_LogVerbose(SDL_LOG_CATEGORY_SYSTEM, "Turn aborted: invalid player action.\n");
            player_action = ACTION_NONE;
            return nullptr; // unable to move and nothing to attack == abort turn
          }
          if (act->is_alive() && act->get_actor_faction() != actor->get_actor_faction()) {
            actor->attack(act);
          }
        }
      }
      vec2i pos = actor->get_position();
      fov.calc(pos, 6);
      camera_pos = pos;
    }
    actor->update(&tilemap);

    player_action = ACTION_NONE;

    current_entity_index = (current_entity_index + 1) % actors->size(); // Increase the current actor index
    Actor* next = actors->at(current_entity_index);
    is_player_turn = next->player_controlled; // Check if next actor is player controlled
    if (is_player_turn) {
      camera_pos = next->get_position();
      player_actor = next;
      fov.calc(player_actor->get_position(), 6);
    }
  }
  return nullptr;
}

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

      ImGui::Checkbox("Disable FoV", &debug_disable_fov);

      ImGui::End();
    }
    if (debug_actors) {
      ImGui::Begin("Actors", &debug_actors);

      auto actors = tilemap.get_actor_list();
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

  const vec2i asciisize = {
      ascii->get_tile_width(),
      ascii->get_tile_height(),
  };
  const vec2i tilesize = {
      app->renderer->get_renderer_width() / ascii->get_tile_width(),
      app->renderer->get_renderer_height() / ascii->get_tile_height(),
  };
  const vec2i margin = {
      (app->renderer->get_renderer_width() - tilesize.x * asciisize.x)/2,
      (app->renderer->get_renderer_height() - tilesize.y * asciisize.y)/2,
  };
  const vec2i offset = {
      (tilesize.x/2-camera_pos.x),
      (tilesize.y/2- camera_pos.y),
  };
  tilemap.draw(app->renderer, ascii, margin.x, margin.y, -offset.x, -offset.y, tilesize.x, tilesize.y, debug_disable_fov ? nullptr : &fov);

  auto entities = tilemap.get_actor_list();

  const Color black = Color(0, 0, 0, 1);

  // Draw dead actors
  for (Actor* var : *entities) {
    if (var->is_alive()) continue;

    vec2i pos = var->get_position();
    if (debug_disable_fov || fov.can_see(pos)) {
      int sprite = var->get_sprite_id();

      Color fg = var->get_sprite_color()*0.35f;
      app->renderer->draw_sprite(ascii->get_sprite(sprite), fg, black, margin.x + (offset.x + pos.x) * asciisize.x, margin.y + (offset.y + pos.y) * asciisize.y);
    }
  }

  // Draw the rest of the entities
  for (Actor* var : *entities) {
    if (!var->is_alive()) continue;

    vec2i pos = var->get_position();
    if (debug_disable_fov || fov.can_see(pos)) {
    
      int sprite = var->get_sprite_id();
      Color fg = var->get_sprite_color();
      Color bg = tilemap.get_tile(pos.x, pos.y).bg;
      app->renderer->draw_sprite(ascii->get_sprite(sprite), fg, bg, margin.x + (offset.x + pos.x) * asciisize.x, margin.y + (offset.y + pos.y) * asciisize.y);
    }
  }
  if (player_actor != nullptr) {
    Color fg = Color(1, 0.01, 0.01, 1);
    for (int i = 0; i < player_actor->get_health(); i++) {
      app->renderer->draw_sprite(ascii->get_sprite(3), fg, black, (i+1) * asciisize.x, asciisize.y);
    }
  }
}

void PlayState::quit() {
}

void PlayState::inputevent(InputEvent *event) {
  if (event->type == INPUT_KEY_EVENT && event->pressed) {
    switch (event->action) {
      case ACTION_TOGGLE_DEBUG: debug = !debug; break;
      case ACTION_RESET: new_game(); break;
      case ACTION_ESCAPE_MENU: break; // TODO
      case ACTION_NONE: break;
      default: player_action = event->action; break;
    }
  }
}
