#include "TileSet.h"
#include <kaguya/kaguya.hpp>
#include <SDL2/SDL_log.h>

Tile null = Tile();

TileSet::TileSet() {}


TileSet::~TileSet() {}

void TileSet::load_from_table(kaguya::LuaStackRef table) {
  for (std::string key : table.keys()) {
    auto val = table[key];
    Tile t;
    if (val["glyph"].type() == LUA_TSTRING) {
      std::string s = val["glyph"];
      t.glyph = s.at(0);
    }
    else {
      SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Tileset: Missing value glyph for tile: %s", key.c_str());
    }
    if (val["fg"].type() == LUA_TTABLE) {
      auto fg = val["fg"];
      t.fg.r = fg[1];
      t.fg.g = fg[2];
      t.fg.b = fg[3];
      t.fg.a = fg[4];
    }
    else {
      SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Tileset: Missing value fg for tile: %s", key.c_str());
    }
    if (val["bg"].type() == LUA_TTABLE) {
      auto bg = val["bg"];
      t.bg.r = bg[1];
      t.bg.g = bg[2];
      t.bg.b = bg[3];
      t.bg.a = bg[4];
    }
    else {
      SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Tileset: Missing value bg for tile: %s", key.c_str());
    }
    if (val["passable"].type() == LUA_TBOOLEAN) {
      t.passable = val["passable"];
    }
    else {
      SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Tileset: Missing value passable for tile: %s", key.c_str());
    }
    if (val["opaque"].type() == LUA_TBOOLEAN) {
      t.opaque = val["opaque"];
    }
    else {
      SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Tileset: Missing value opaque for tile: %s", key.c_str());
    }
    if (val["wall"].type() == LUA_TBOOLEAN) {
      t.wall = val["wall"];
    }
    else {
      SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Tileset: Missing value wall for tile: %s", key.c_str());
    }
    if (val["tags"].type() == LUA_TTABLE) {
      for (std::string tag : val["tags"].values()) {
        t.tags.emplace_back(tag);
      }
    }
    else {
      SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Tileset: Missing value wall for tile: %s", key.c_str());
    }
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Tileset: Added tile: %s", key.c_str());
    tiles[key] = t;
    if (tiles.count(key) == 0) {
      SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Tileset: Could not find the tile we just added?!: %s", key.c_str());
    }
  }
}

void TileSet::add_tile(std::string name, Tile tile) {
  tiles[name] = tile;
}

Tile const& TileSet::get_tile(std::string name) {
  auto it = tiles.find(name);
  if (it == tiles.end()) {
    return null;
  }
  else {
    return it->second;
  }
}

std::vector<std::string> TileSet::find_tiles(bool passable, bool opaque, bool wall, std::vector<std::string> include_tags, std::vector<std::string> exclude_tags) {
  std::vector<std::string> found_tiles;

  // TODO: optimize this stuff, could use less for loops
  for (auto pair : tiles) {
    if (pair.second.passable == passable 
      && pair.second.opaque == opaque 
      && pair.second.wall == wall) {
      bool match = true;
      for (std::string& inc : include_tags) {
        if (!match) break;
        match = false;
        for (std::string& tag : pair.second.tags) {
          if (tag == inc) {
            match = true;
            break;
          }
        }
      }
      for (std::string& tag : pair.second.tags) {
        if (!match) break;
        for (std::string& excl : exclude_tags) {
          if (tag == excl) {
            match = false;
            break;
          }
        }
      }
      if (match) {
        found_tiles.emplace_back(pair.first);
      }
    }
  }

  return found_tiles;
}
