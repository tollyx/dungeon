#pragma once
#include <string>

class Renderer;
struct Sprite;

// Automatically splits a tileset image to seperate sprites.
class SpriteAtlas {
  Sprite* sprites;
  int amount;
  int tile_width;
  int tile_height;
public:
  SpriteAtlas(Renderer* renderer, std::string imgPath, int imgWidth, int imgHeight, int tileWidth, int tileHeight);
  ~SpriteAtlas();
  int get_amount();
  int get_tile_width();
  int get_tile_height();
  Sprite* get_sprite(int tileId);
};

