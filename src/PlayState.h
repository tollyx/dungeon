#pragma once

#include "Gamestate.h"
#include "Tilemap.h"
#include "FieldOfView.h"

class SpriteAtlas;
class Actor;

class PlayState : public Gamestate {
  SpriteAtlas* ascii;
  Tilemap tilemap;
  Actor * player_actor;
  FieldOfView fov;
  unsigned int current_entity_index;
  bool is_player_turn;

  vec2i camera_pos;

  bool debug;
  bool debug_actors = false;
  bool debug_settings = false;
  bool debug_disable_fov = false;

public:
  void new_game();
  void load() override;
  Gamestate* update(double delta) override;
  void draw(double delta) override;
  void quit() override;
  void inputevent(InputEvent* event) override;
};
