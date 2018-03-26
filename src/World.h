#pragma once
#include "Tilemap.h"
#include <vector>

class World {
public:
  World(unsigned int seed = 0);
  Tilemap& GetMap(unsigned int level, TileSet& tileset);
  ~World();
  
private:
  int worldseed;
  std::vector<unsigned int> seeds;
  std::vector<Tilemap> maps;
};
