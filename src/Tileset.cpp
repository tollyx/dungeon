#include "Tileset.h"
#include "Renderer.h"

Tileset::Tileset(Renderer* renderer, char * imgPath, int imgWidth, int imgHeight, int tileWidth, int tileHeight) {
  int tilesX = imgWidth / tileWidth;
  int tilesY = imgHeight / tileHeight;
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

Tileset::~Tileset() {
  delete sprites;
}

int Tileset::GetAmount() {
  return amount;
}

Sprite * Tileset::GetSprite(int tileId) {
  if (tileId >= 0 && tileId < amount) {
    return &sprites[tileId];
  }
  return nullptr;
}
