#include "SpriteAtlas.h"
#include "Renderer.h"

SpriteAtlas::SpriteAtlas(Renderer* renderer, std::string imgPath, int imgWidth, int imgHeight, int tileWidth, int tileHeight) {
  int tilesX = imgWidth / tileWidth;
  int tilesY = imgHeight / tileHeight;
  tile_width = tileWidth;
  tile_height = tileHeight;
  amount = tilesX*tilesY;
  sprites = new Sprite[amount];
  if (renderer->LoadTexture(imgPath) != nullptr) {
    for (int i = 0; i < amount; i++) {
      int x = i%tilesX * tileWidth;
      int y = (i / tilesX) * tileHeight;
      sprites[i] = renderer->CreateSprite(imgPath, x, y, tileWidth, tileHeight);
    }
  }
  else {
    amount = -1; // Prevents it from returning non-existing sprite objects.
  }
}

SpriteAtlas::~SpriteAtlas() {
  delete sprites;
}

int SpriteAtlas::get_amount() {
  return amount;
}

Sprite * SpriteAtlas::get_sprite(int tileId) {
  if (tileId >= 0 && tileId < amount) {
    return &sprites[tileId];
  }
  return nullptr;
}

int SpriteAtlas::get_tile_width() {
  return tile_width;
}

int SpriteAtlas::get_tile_height() {
  return tile_height;
}
