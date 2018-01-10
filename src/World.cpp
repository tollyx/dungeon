#include "World.h"
#include <time.h>
#include <cstdlib>

World::World() {
  World(0);
}

World::World(unsigned int seed) {
  worldseed = seed;
}

void World::Init() {
  if (seeds.size() != 0) {
    return;
  }
  srand(worldseed);
  rand(); rand(); rand();
  for (int i = 0; i < 10; i++) {
    seeds.push_back(rand());
    maps.push_back(nullptr);
  }
}

Tilemap* World::GetMap(unsigned int level) {
  if (level < seeds.size()) {
    if (maps[level] != nullptr) {
      return maps[level];
    }
    else {
      Tilemap* map = new Tilemap(TileSet(),64, 64);
      // TODO: generate map
      maps[level] = map;
      return map;
    }
  }
  return nullptr;
}

World::~World() {
  seeds.clear();
  for (Tilemap* map : maps) {
    if (map != nullptr) {
      delete map;
    }
  }
  maps.clear();
}
