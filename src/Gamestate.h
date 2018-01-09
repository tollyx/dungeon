//
// Created by Adrian on 2017-09-19.
//

#ifndef DUNGEON_GAMESTATE_H
#define DUNGEON_GAMESTATE_H

class App;
struct InputEvent;

class Gamestate {
protected:
  App* app;
public:
  //virtual ~Gamestate() {};
  void init(App* app);
  virtual void load() = 0;
  virtual Gamestate* update(double delta) = 0;
  virtual void draw(double delta) = 0;
  virtual void quit() = 0;
  virtual void inputevent(InputEvent* event) = 0;
};


#endif //DUNGEON_GAMESTATE_H
