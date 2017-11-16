//
// Created by Adrian on 2017-09-19.
//

#ifndef DUNGEON_APP_H
#define DUNGEON_APP_H

class Gamestate;
class Renderer;
class Input;

#include "gitparams.h"
#define ADD_QUOTES_HELPER(s) #s
#define ADD_QUOTES(s) ADD_QUOTES_HELPER(s)

class App {
public:
  const char* version = ADD_QUOTES(GIT_CUR_COMMIT);
  Gamestate* current;
  Renderer* renderer;
  Input* input;
  bool init();
  int start();
};


#endif //DUNGEON_APP_H
