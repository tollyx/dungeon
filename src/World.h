#pragma once
#include "Tilemap.h"
#include <vector>

class World {
public:
  World();
  World(unsigned int seed);
  void Init();
  Tilemap* GetMap(unsigned int level); // returns null if invalid level or if not initialized
  ~World();
  
private:
  int worldseed;
  std::vector<unsigned int> seeds;
  std::vector<Tilemap*> maps;
};
