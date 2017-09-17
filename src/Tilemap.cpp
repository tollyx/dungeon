#include "Tilemap.h"
#include "vec2i.h"
#include "Actor.h"

int Tilemap::GetIndex(int x, int y) 
{
  return y * width + x;
}

Tilemap::Tilemap(int width, int height)
{
  this->width = width;
  this->height = height;
  tilemap = new unsigned int[width*height]{0};
}

Tilemap::~Tilemap() 
{
  delete tilemap;
  for (auto var : actors) {
    delete var;
  }
}

int Tilemap::GetWidth() 
{
  return width;
}

int Tilemap::GetHeight() 
{
  return height;
}

bool Tilemap::IsInsideBounds(int x, int y) 
{
  return x >= 0 && x < width && y >= 0 && y < height;
}

std::vector<vec2i> Tilemap::getNeighbours(int x, int y, int range) 
{
  std::vector<vec2i> neigh;
  if (range == 0) 
  {
    neigh.push_back({x,y});
    return neigh;
  }
  for (int dx = -range; dx <= range; dx++) 
  {
    for (int dy = -range; dy <= range; dy++) 
    {
      if ((dx != 0 || dy != 0) && IsInsideBounds(x + dx, y + dy)) 
      {
        neigh.push_back({x+dx,y+dy});
      }
    }
  }
  return neigh;
}

void Tilemap::SetTile(int x, int y, int tile) 
{
  if (IsInsideBounds(x, y)) 
  {
    tilemap[GetIndex(x, y)] = tile;
  }
}

int Tilemap::GetTile(int x, int y) 
{
  if (IsInsideBounds(x, y)) 
  {
    return tilemap[GetIndex(x, y)];
  }
  return -1;
}

bool Tilemap::IsBlocked(int x, int y) 
{
  if (IsInsideBounds(x, y)) 
  {
    if (tilemap[GetIndex(x,y)] == '#') { // TODO: Replace hardcoded tiles
      return true;
    }
    for (Actor* var : actors) {
      vec2i pos = var->getPosition();
      if (var->IsAlive() && pos.x == x && pos.y == y) {
        return true;
      }
    }
    return false;
  }
  return true;
}

void Tilemap::AddActor(Actor * actor) {
  for (Actor* var : actors) {
    if (var == actor) {
      return;
    }
  }
  actors.push_back(actor);
}

void Tilemap::RemoveActor(Actor * actor) {
  for (auto it = actors.begin(); it != actors.end(); it++) {
    if ((*it) == actor) {
      actors.erase(it);
      return;
    }
  }
}

Actor * Tilemap::GetActor(int x, int y, Actors type) {
  vec2i pos = { x,y };
  for (Actor* act : actors) {
    if (act->isTypeOf(type)) {
      vec2i apos = act->getPosition();
      if (apos == pos) {
        return act;
      }
    }
  }
  return nullptr;
}

std::vector<Actor*> Tilemap::GetActors(int x, int y, int range, Actors type) {
  std::vector<Actor*> found;
  std::vector<vec2i> neigh = getNeighbours(x, y, range);
  for (Actor* act : actors) {
    for (vec2i pos : neigh) {
      if (act->isTypeOf(type)) {
        vec2i apos = act->getPosition();
        if (apos == pos) {
          found.push_back(act);
          break;
        }
      }
    }
  }
  return found;
}

std::vector<Actor*>* Tilemap::GetActorList() {
  return &actors;
}
