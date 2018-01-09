#pragma once

#include "Renderer.h"
#include <string>
#include <map>
#include <vector>
#include <kaguya/kaguya.hpp>

struct Tile {
  char glyph = 0;
  Color fg = Color(1, 1, 1, 1);
  Color bg = Color(0, 0, 0, 1);
  bool passable = false;
  bool opaque = false;
  bool wall = true;
  std::string desc = "";
  std::vector<std::string> tags;

  bool has_tag(std::string tag) const {
    for (std::string t : tags) {
      if (t == tag) {
        return true;
      }
    }
    return false;
  }
};

class TileSet {
  std::map<std::string, Tile> tiles;
public:
  TileSet();
  ~TileSet();
  void load_from_table(kaguya::LuaStackRef table);
  void add_tile(std::string name, Tile tile);
  Tile const& get_tile(std::string name);
  std::vector<std::string> find_tiles(bool passable, bool opaque, bool wall, std::vector<std::string> include_tags = {}, std::vector<std::string> exclude_tags = {});
};

