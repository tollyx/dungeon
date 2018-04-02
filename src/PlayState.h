#pragma once

#include "statemachine/StateStack.h"
#include "Tilemap.h"
#include "FieldOfView.h"
#include "World.h"

class SpriteAtlas;
class Actor;
class App;

class PlayState : public IState {
  SpriteAtlas* ascii;
  World world;
  Tilemap* tilemap;
  Actor * player_actor;
  FieldOfView fov;
  unsigned int current_entity_index;
  bool is_player_turn;
  unsigned int current_level;

  vec2i camera_pos;

  bool debug;
  bool debug_actors = false;
  bool debug_settings = false;
  bool debug_disable_fov = false;

  App* app;

public:
  PlayState(App* app);
  void new_game();
  void enter() override;
  StateResult update(float delta) override;
  void draw() override;
  void exit() override;
  void input(InputEvent& event) override;
};
