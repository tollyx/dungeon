#include "Mapgen.h"
#include "vec2i.h"
#include <vector>
#include <queue>
#include <random>
#include "Rng.h"
#include <chrono>
#include <stack>
#include "Goblin.h"
#include "Hero.h"
#include "Pathfinder.h"

struct Room {
  vec2i pos;
  vec2i size;
};

bool aabb(Room &a, Room &b) {
  return a.pos.x <= b.pos.x + b.size.x && a.pos.x + a.size.x >= b.pos.x &&
         a.pos.y <= b.pos.y + b.size.y && a.pos.y + a.size.y >= b.pos.y;
}

template<class T> T rand_entry(std::vector<T> &vec, Rng  &rng) {
  return vec[rng.get_int(vec.size()-1)];
}

void maze_fill(Tilemap& map, int x, int y, std::string wall, std::string floor, Rng &rng) {
  if (!map.get_tile(x, y).wall) return;

  const std::vector<vec2i> dirs { vec2i(0,1), vec2i(1,0), vec2i(0,-1), vec2i(-1,0) };

  std::stack<vec2i> stack;
  stack.emplace(vec2i(x, y));
  while (!stack.empty()) {
    vec2i pos = stack.top();
    map.set_tile(pos.x, pos.y, floor);
    std::vector<vec2i> options;
    for (vec2i dir : dirs) {
      vec2i next = { pos.x + dir.x, pos.y + dir.y };
      if (!map.get_tile(next.x, next.y).wall) continue;
      if (next.x == 0 || next.x == map.get_width() - 1 || next.y == 0 || next.y == map.get_height() - 1) continue;

      int up = dir.y <= 0 ? 1 : 0;
      int down = dir.y >= 0 ? 1 : 0;
      int left = dir.x <= 0 ? 1 : 0;
      int right = dir.x >= 0 ? 1 : 0;

      std::vector<vec2i> neigh = map.get_neighbours(next.x, next.y, up, down, left, right);
      bool enclosed = true;
      for (vec2i n : neigh) {
        if (!map.get_tile(n.x, n.y).wall) {
          enclosed = false;
          break;
        }
      }

      if (enclosed) {
        options.emplace_back(next.x, next.y);
      }
    }
    if (!options.empty()) {
      stack.emplace(rand_entry<vec2i>(options,rng));
    }
    else {
      stack.pop();
    }
  }

}

Tilemap generate_dungeon(int width, int height, TileSet tileset) {
  return generate_dungeon(Rng::get_random_seed(), width, height, tileset);
}

Tilemap generate_dungeon(unsigned int seed, int width, int height, TileSet tileset) {
  Rng rng = Rng(seed);

  Tilemap map = Tilemap(tileset, width, height);

  std::vector<std::string> wall_tiles = tileset.find_tiles(false, true, true, { "dungeon", "wall" }, {});
  std::vector<std::string> floor_tiles = tileset.find_tiles(true, false, false, { "dungeon", "floor" }, {});
  std::vector<std::string> door_tiles = tileset.find_tiles(true, true, true, { "dungeon", "door" }, {});
  std::string entrance_tile = tileset.find_tiles(true, false, false, { "dungeon", "entrance" }).at(0);
  std::string exit_tile = tileset.find_tiles(true, false, false, { "dungeon", "exit" }).at(0);
#ifdef _DEBUG
  assert(wall_tiles.size() > 0);
  assert(floor_tiles.size() > 0);
  assert(door_tiles.size() > 0);
#endif

  // Set the whole map to walls
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      map.set_tile(x, y, rand_entry<std::string>(wall_tiles, rng));
    }
  }

  // Room placement
  std::vector<Room> rooms;
  for (int i = 0; i < sqrt(width*height); i++) {
    Room room;
    room.size = vec2i(rng.get_int(6, 12), rng.get_int(6, 12));
    room.pos = vec2i(rng.get_int(width - room.size.x), rng.get_int(height - room.size.y));

    // Check if the room overlaps with some other room
    bool coll = false;
    for (Room r : rooms) {
      if (aabb(room, r)) {
        coll = true;
        break;
      }
    }
    if (!coll) {
      rooms.emplace_back(room);
    }
  }

  // Fill the rooms with floor tiles
  for (Room r : rooms) {
    for (int x = r.pos.x+1; x < r.pos.x + r.size.x-1; x++) {
      for (int y = r.pos.y+1; y < r.pos.y + r.size.y-1; y++) {
        map.set_tile(x, y, rand_entry<std::string>(floor_tiles, rng));
      }
    }
  }
  
  // Maze generation
  std::vector<vec2i> maze_start_points;
  for (int x = 0; x < map.get_width(); x++) {
    for (int y = 0; y < map.get_height(); y++) {
      std::vector<vec2i> neigh = map.get_neighbours(x, y, 1);
      int count = 0;
      for (vec2i n : neigh) {
        if (map.get_tile(n.x, n.y).wall) { 
          count++;
        }
      }
      // If this tile is a wall and is completely surrounded by other walls, start generating a maze here.
      if (count >= 8) {
        maze_fill(map, x, y, rand_entry<std::string>(wall_tiles, rng), floor_tiles[rng.get_int(0, floor_tiles.size() - 1)], rng);
        maze_start_points.emplace_back(vec2i(x, y));
      }
    }
  }

  // Door placement
  for (Room r : rooms) {
    std::vector<vec2i> potential_doors;
    for (int y = 0; y < r.size.y; y++) {
      for (int x = 0; x < r.size.x; x++) {
        // if we are at the rooms walls, but not the corners
        if ((x == 0 || x == r.size.x-1) != (y == 0 || y == r.size.y - 1)) {
          int dx = 0;
          if (x == 0) {
            dx = -1;
          }
          else if (x == r.size.x-1) {
            dx = 1;
          }
          
          int dy = 0;
          if (y == 0) {
            dy = -1;
          }
          else if (y == r.size.y-1) {
            dy = 1;
          }

          // If there is a floor tile on the other side of this room wall
          if (map.get_tile(r.pos.x+x+dx, r.pos.y+y+dy).passable) {
            potential_doors.emplace_back(r.pos.x + x, r.pos.y + y);
          }
        }
      }
    }
    /* Debug thing, place doors at all potential spots
    for (vec2i pos : potential_doors) {
      map.set_tile(pos.x, pos.y, doortile);
    }
    /*/
    if (potential_doors.empty()) continue;

    // Pick up to 3 spots and place doors
    int doors_amount = potential_doors.size() < 3 ? potential_doors.size() : 4;
    doors_amount = rng.get_int(2, doors_amount);

    for (int i = 0; i < doors_amount; i++) {
    //for (int i = 0; i < potential_doors.size(); i++) {
      if (potential_doors.empty()) break;

      int r = rng.get_int(potential_doors.size()-1);
      vec2i pos = potential_doors.at(r);
      map.set_tile(pos.x, pos.y, rand_entry<std::string>(door_tiles, rng));
      potential_doors.erase(r + potential_doors.begin());
      for (int j = potential_doors.size() - 1; j >= 0; j--) {
        if ((pos - potential_doors[j]).dist() <= 4) {
          potential_doors.erase(j + potential_doors.begin());
        }
      }
    }
    //*/
  }

  // Clean up dead ends in the maze
  std::vector<vec2i> dead_ends;
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      std::vector<vec2i> neigh{vec2i(x + 1, y), vec2i(x, y + 1), vec2i(x - 1, y), vec2i(x, y - 1) };
      int count = 0;
      for (vec2i pos : neigh) {
        if (!map.get_tile(pos.x, pos.y).passable) {
          count++;
        }
      }
      if (count >= neigh.size() - 1) {
        dead_ends.emplace_back(vec2i(x, y));
      }
    }
  }
  int pass_amount = width + height;
  for (int pass = 0; pass < pass_amount; pass++) {
    if (dead_ends.empty()) break;
    std::vector<vec2i> new_dead_ends;
    for (vec2i pos : dead_ends) {
      std::vector<vec2i> neigh { vec2i(pos.x + 1, pos.y), vec2i(pos.x, pos.y + 1), vec2i(pos.x - 1, pos.y), vec2i(pos.x, pos.y - 1) };
      int count = 0;
      vec2i next;
      for (vec2i n : neigh) {
        if (!map.get_tile(n.x, n.y).passable) {
          continue;
        }
        else {
          count++;
          next = n;
        }
      }
      if (count == 1) {
        map.set_tile(pos.x, pos.y, rand_entry<std::string>(wall_tiles, rng));
        new_dead_ends.emplace_back(next);
      }
      else if (count == 0) {
        map.set_tile(pos.x, pos.y, rand_entry<std::string>(wall_tiles, rng));
      }
    }
    dead_ends = new_dead_ends;
  }

  // Place the entrance in a random room
  Room& startroom = rooms[rng.get_int(0, rooms.size() - 1)];
  vec2i startpos = startroom.pos;
  startpos.x += rng.get_int(1, startroom.size.x - 2);
  startpos.y += rng.get_int(1, startroom.size.y - 2);
  map.set_tile(startpos.x, startpos.y, entrance_tile);

  // Find the room furthest away from the entrance and make it the exit
  Pathfinder::DijkstraMap dijk;
  const float maxv = width+height;
  std::vector<vec2i> goals = { startpos };
  Pathfinder::calc_dijkstra_map(map, goals, dijk, maxv);
  
  float exitroomval = 0;
  Room* exitroom = &startroom;
  for (Room& room : rooms) {
    float room_min_v = maxv;
    for (int x = 0; x < room.size.x; x++) {
      for (int y = 0; y < room.size.y; y++) {
        float val = dijk.get_value(room.pos.x + x, room.pos.y + y);
        if (val < room_min_v) {
          room_min_v = val;
        }
      }
    }
    if (room_min_v > exitroomval) {
      exitroom = &room;
      exitroomval = room_min_v;
    }
  }

  vec2i exitpos = exitroom->pos;
  exitpos.x += rng.get_int(1, exitroom->size.x - 2);
  exitpos.y += rng.get_int(1, exitroom->size.y - 2);
  map.set_tile(exitpos.x, exitpos.y, exit_tile);
  float endval = dijk.get_value(exitpos.x, exitpos.y);

  auto path = Pathfinder::a_star(&map, startpos, exitpos);

  Pathfinder::calc_dijkstra_map(map, path, dijk, maxv);

  map.add_actor(new Hero(startpos));

  for (Room r : rooms) {
    float room_value = 1;
    for (int x = 0; x < r.size.x; x++) {
      for (int y = 0; y < r.size.y; y++) {
        float val = dijk.get_value(r.pos.x + x, r.pos.y + y)/maxv;
        if (val < room_value) {
          val = room_value;
        }
      }
    }
    if (rng.get_float() < 0.1f + 0.3f*room_value) {
      int amount = 1 + 3 * (rng.get_float() + room_value);
      for (int i = 0; i < amount; i++) {
        vec2i pos = r.pos;
        pos.x += rng.get_int(1, r.size.x - 2);
        pos.y += rng.get_int(1, r.size.y - 2);
        map.add_actor(new Goblin(pos));
      }
    }
  }

  /* dijkstra debug
  for (int x = 0; x < map.get_width(); x++) {
    for (int y = 0; y < map.get_height(); y++) {
      float dv = dijk.get_value(x, y);
      float a = dv / maxv;
      int val = (int)(a*99)+1;
      const Tile& tile = map.get_tile(x, y);
      if (tile.passable && tile.has_tag("floor")) {
        map.set_tile(x, y, "dijkstra_debug_floor_" + std::to_string(val));
      }
    }
  }
  //*/
  return map;
}
