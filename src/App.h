//
// Created by Adrian on 2017-09-19.
//

#ifndef DUNGEON_APP_H
#define DUNGEON_APP_H

#pragma once

class Gamestate;
class Renderer;
class Input;

class App {
public:
  Gamestate* current;
  Renderer* renderer;
  Input* input;
  bool init();
  int start();
};


#endif //DUNGEON_APP_H
