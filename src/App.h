//
// Created by Adrian on 2017-09-19.
//

#ifndef DUNGEON_APP_H
#define DUNGEON_APP_H

#include "statemachine/StateStack.h"

class Renderer;
class Input;

#include "gitparams.h"
#define ADD_QUOTES_HELPER(s) #s
#define ADD_QUOTES(s) ADD_QUOTES_HELPER(s)

class App {
  bool running = true;
public:
  const char* version = ADD_QUOTES(GIT_CUR_COMMIT);
  StateStack statestack;
  Renderer* renderer;
  Input* input;
  bool init();
  int start();
};


#endif //DUNGEON_APP_H
