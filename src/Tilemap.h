#pragma once
#include <vector>
#include "Actor.h"

struct vec2i;

class Tilemap {
  unsigned int* tilemap;
  std::vector<Actor*> actors;
  int width;
  int height;
public:
  Tilemap(int width = 1, int height = 1);
  ~Tilemap();
  int GetWidth();
  int GetHeight();
  int GetIndex(int x, int y); // Converts [x,y] to a 1D index.
  bool IsInsideBounds(int x, int y);
  std::vector<vec2i> getNeighbours(int x, int y, int range = 1);
  void SetTile(int x, int y, int tile); // "Tile" is inteded for tile ids, but can be anything really.
  int GetTile(int x, int y);
  bool IsBlocked(int x, int y); // Checks if there is an actor blocking the tile.

  void AddActor(Actor* actor);
  void RemoveActor(Actor* actor);
  Actor* GetActor(int x, int y, Actors type);
  std::vector<Actor*> GetActors(int x, int y, int range, Actors type);
  std::vector<Actor*>* GetActorList();
};

