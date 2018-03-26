#pragma once
#include <string>
#include <kaguya/kaguya.hpp>
#include "TileSet.h"

class LuaHandler {
public:
  LuaHandler();
  ~LuaHandler();
  void load_module(std::string name, TileSet* tileset);
  static void LuaHandler::add_tile(TileSet* tileset, std::string name, kaguya::LuaTable act);
private:
  kaguya::State lua;
};
