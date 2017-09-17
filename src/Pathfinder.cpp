#include "Pathfinder.h"
#include "Tilemap.h"

namespace Pathfinder
{
  float distance(vec2i a, vec2i b)
  {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return sqrtf(dx*dx + dy*dy);
  }

  std::vector<vec2i> aStar(Tilemap * map, vec2i start, vec2i goal)
  {
    std::vector<AStarNode*> open;
    std::vector<AStarNode*> closed;

    std::vector<vec2i> path;

    AStarNode* st = new AStarNode();
    st->pos = start;
    open.push_back(st);

    while (open.size() != 0)
    {
      AStarNode* current = nullptr;
      int currentindex = -1;
      for (int i = 0; i < open.size(); i++)
      {
        if (current == nullptr || current->f > open[i]->f)
        {
          current = open[i];
          currentindex = i;
        }
      }
      open.erase(open.begin()+currentindex);
      closed.push_back(current);
      //map->SetTile(current->pos.x, current->pos.y, TILE_CLOSED);

      auto neighbours = map->getNeighbours(current->pos.x, current->pos.y);
      for each (auto pos in neighbours)
      {
        if (map->GetTile(pos.x, pos.y) == '#')
        {
          continue;
        }
        AStarNode* neighbour = new AStarNode();
        neighbour->pos = current->pos;
        neighbour->pos.x = pos.x;
        neighbour->pos.y = pos.y;

        neighbour->from = current;
        neighbour->g = current->g + 1;
        neighbour->h = distance(neighbour->pos, goal);
        neighbour->f = neighbour->g + neighbour->h;

        // Is this the goal?
        if (neighbour->pos == goal)
        {
          delete neighbour;
          path.push_back(goal);
          while (current->from != nullptr)
          {
            path.push_back(current->pos);
            current = current->from;
          }
          for each (AStarNode* var in open)
          {
            delete var;
          }
          for each (AStarNode* var in closed)
          {
            delete var;
          }
          return path;
        }

        // Check if the new node is in the closed list
        bool isClosed = false;
        for (auto it = closed.begin(); it != closed.end(); it++)
        {
          AStarNode* n = *it;
          if (n->pos == neighbour->pos)
          {
            isClosed = true;
            if (n->h > neighbour->h)
            {
              closed.erase(it);
              delete (*it);
              open.push_back(neighbour);
              //map->SetTile(neighbour->pos.x, neighbour->pos.y, TILE_OPENED);
              isClosed = false;
            }
            break;
          }
        }

        if (!isClosed)
        {
          // Check if the new node is in the open list
          bool isOpened = false;
          for (auto it = open.begin(); it != open.end(); it++)
          {
            AStarNode* n = *it;
            if (n->pos == neighbour->pos)
            {
              isOpened = true;
              if (n->h > neighbour->h)
              {
                open.erase(it);
                delete (*it);
                open.push_back(neighbour);
                //map->SetTile(neighbour->pos.x, neighbour->pos.y, TILE_OPENED);
              }
              break;
            }
          }

          if (!isOpened)
          {
            open.push_back(neighbour);
            //map->SetTile(neighbour->pos.x, neighbour->pos.y, TILE_OPENED);
          }
        }
        else
        {
          delete neighbour;
        }
      }
      for (auto it = open.begin(); it != open.end(); it++)
      {
        if (current == (*it))
        {
          open.erase(it);
          delete (*it);
          break;
        }
      }
    }
    for each (AStarNode* var in open)
    {
      delete var;
    }
    for each (AStarNode* var in closed)
    {
      delete var;
    }
    return path;
  }

  void calcDijkstraMap(Tilemap * map, std::vector<vec2i>* goals, DijkstraMap * out, float maxValue) {
    if (out->tilemap != nullptr) {
      delete out->tilemap;
    }
    out->tilemap = new float[map->GetWidth() * map->GetHeight()];
    for (int i = 0; i < map->GetWidth() * map->GetHeight(); i++) {
      out->tilemap[i] = maxValue;
    }
    out->height = map->GetHeight();
    out->width = map->GetWidth();
    for each (vec2i pos in *goals) {
      out->setValue(pos.x, pos.y, 0);
    }

    std::vector<vec2i> queue;

    for each (vec2i pos in *goals) {
      auto neigh = map->getNeighbours(pos.x, pos.y);
      for each (vec2i npos in neigh) {
        int val = out->getValue(npos.x, npos.y);
        if (map->GetTile(npos.x, npos.y) != '#' && val > 1) {
          if (npos.x != 0 && npos.y != 0) {
            out->setValue(npos.x, npos.y, 1.4f);
          }
          else {
            out->setValue(npos.x, npos.y, 1);
          }
          queue.push_back(npos);
        }
      }
    }

    while (queue.size() > 0) {
      vec2i current = queue.back();
      queue.pop_back();
      
      std::vector<vec2i> neigh = map->getNeighbours(current.x, current.y);
      for (int i = 0; i < neigh.size(); i++) {
        float val = out->getValue(current.x, current.y) + 1;
        vec2i npos = neigh[i];
        vec2i dp = npos - current;
        if (dp.x != 0 && dp.y != 0) {
          val += .4f;
        }
        if (map->GetTile(npos.x, npos.y) != '#' && out->getValue(npos.x, npos.y) > val) { // TODO: Remove hardcoded tile
          out->setValue(npos.x, npos.y, val);
          queue.push_back(neigh[i]);
        }
      }
    }
    return;
  }
}