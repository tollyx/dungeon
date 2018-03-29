#include "World.h"
#include <time.h>
#include <cstdlib>
#include "Mapgen.h"
#include "TileSet.h"

World::World(unsigned int seed) {
  worldseed = seed;
  srand(worldseed);
  rand(); rand(); rand();
  for (int i = 0; i < 10; i++) {
    seeds.push_back(rand());
    maps.push_back(Tilemap());
  }
}

Tilemap& World::GetMap(unsigned int level, TileSet& tileset) {
  if (maps[level].get_height() > 1) {
    return maps[level];
  }
  else {
    maps[level] = generate_dungeon(seeds[level], 64, 64, tileset);
    return maps[level];
  }
}

World::~World() {}
