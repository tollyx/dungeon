#pragma once
#include "Tilemap.h"

Tilemap generate_dungeon(int width, int height, TileSet& tileset);
Tilemap generate_dungeon(unsigned int seed, int width, int height, TileSet& tileset);
