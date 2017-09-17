#pragma once
#include <string>

class Renderer;
struct Sprite;

// Automatically splits a tileset image to seperate sprites.
class Tileset {
  Sprite* sprites;
  int amount;
public:
  Tileset(Renderer* renderer, std::string imgPath, int imgWidth, int imgHeight, int tileWidth, int tileHeight);
  ~Tileset();
  int GetAmount();
  Sprite* GetSprite(int tileId);
};

