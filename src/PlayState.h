//
// Created by Adrian on 2017-09-21.
//

#ifndef DUNGEON_PLAYSTATE_H
#define DUNGEON_PLAYSTATE_H


#include "Gamestate.h"
#include "Tilemap.h"
#include "FieldOfView.h"

class Tileset;
class Actor;

class PlayState : public Gamestate {
  Tileset* ascii;
  Tilemap tilemap;
  Actor * hero;
  FieldOfView fov;

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
