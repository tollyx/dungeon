#include "LuaHandler.h"

const char* DUNGEON_LUA_NAMESPACE = "dungeon";

LuaHandler::LuaHandler() {}

LuaHandler::~LuaHandler() {
  
}

void LuaHandler::add_tile(TileSet* tileset, std::string name, kaguya::LuaTable act) {
  Tile t;
  if (act["glyph"].type() == LUA_TSTRING) {
    std::string s = act["glyph"];
    t.glyph = s.at(0);
  }
  if (act["fg"].type() == LUA_TTABLE) {
    auto fg = act["fg"];
    t.fg.r = fg[1];
    t.fg.g = fg[2];
    t.fg.b = fg[3];
    t.fg.a = fg[4];
  }
  if (act["bg"].type() == LUA_TTABLE) {
    auto bg = act["bg"];
    t.bg.r = bg[1];
    t.bg.g = bg[2];
    t.bg.b = bg[3];
    t.bg.a = bg[4];
  }
  if (act["passable"].type() == LUA_TBOOLEAN) {
    t.passable = act["passable"];
  }
  if (act["opaque"].type() == LUA_TBOOLEAN) {
    t.opaque = act["opaque"];
  }
  if (act["wall"].type() == LUA_TBOOLEAN) {
    t.wall = act["wall"];
  }
  if (act["tags"].type() == LUA_TTABLE) {
    for (std::string tag : act["tags"].values()) {
      t.tags.emplace_back(tag);
    }
  }
  tileset->add_tile(name, t);
}

void LuaHandler::load_module(std::string name, TileSet* tileset) {
  lua[DUNGEON_LUA_NAMESPACE] = kaguya::NewTable();
  


  lua[DUNGEON_LUA_NAMESPACE]["add_actor"] = kaguya::function([](std::string name, kaguya::LuaTable act) {

  });

  lua[DUNGEON_LUA_NAMESPACE]["add_actors"] = kaguya::function([](kaguya::LuaTable actors) {
    for (auto pair : actors.map()) {
      
    }
  });

  lua[DUNGEON_LUA_NAMESPACE]["add_tile"] = kaguya::function([tileset](std::string name, kaguya::LuaTable tile) {
    add_tile(tileset, name, tile);
  });

  lua[DUNGEON_LUA_NAMESPACE]["add_tiles"] = kaguya::function([tileset](kaguya::LuaTable tiles) {
    for (auto pair : tiles.map()) {
      add_tile(tileset, pair.first, pair.second);
    }
  });

  lua.dofile(name + "/init.lua");
}
