//
// Created by Adrian on 2017-09-21.
//

#ifndef DUNGEON_PLAYSTATE_H
#define DUNGEON_PLAYSTATE_H


#include "Gamestate.h"

class Tileset;
class Tilemap;
class Actor;
class FieldOfView;

class PlayState : public Gamestate {
  Tileset* ascii;
  Tilemap* tilemap;
  Actor * hero;
  FieldOfView *fov;
  float delay = .5;
  double timer;

  bool paused;
  bool debug;
public:
  void new_game();
  void load() override;
  Gamestate* update(double delta) override;
  void draw(double delta) override;
  void quit() override;
  void inputevent(InputEvent* event) override;
};


#endif //DUNGEON_PLAYSTATE_H
