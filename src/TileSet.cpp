#include "TileSet.h"
#include <kaguya/kaguya.hpp>
#include <SDL2/SDL_log.h>

Tile null = Tile();

TileSet::TileSet() {}


TileSet::~TileSet() {}

int TileSet::count() const {
  return tiles.size();
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
