#include "Mapgen.h"
#include "vec2i.h"
#include <vector>
#include <queue>
#include <random>
#include "Rng.h"
#include <chrono>

struct Room {
  vec2i pos;
  vec2i size;
};

const char walltile = '#';
const char floortile = '.';
const char doortile = '+';
const char testtile = ' ';

bool aabb(Room &a, Room &b) {
  return a.pos.x <= b.pos.x + b.size.x && a.pos.x + a.size.x >= b.pos.x &&
         a.pos.y <= b.pos.y + b.size.y && a.pos.y + a.size.y >= b.pos.y;
}

void maze_fill(Tilemap& map, int x, int y, Rng &rng) {
  if (map.get_tile(x, y) != walltile) return;

  const std::vector<vec2i> dirs { vec2i(0,1), vec2i(1,0), vec2i(0,-1), vec2i(-1,0) };

  std::vector<vec2i> stack { vec2i(x,y) };
  while (!stack.empty()) {
    vec2i pos = stack.back();
    map.set_tile(pos.x, pos.y, floortile);
    std::vector<vec2i> options;
    for (vec2i dir : dirs) {
      vec2i next = { pos.x + dir.x, pos.y + dir.y };
      if (map.get_tile(next.x, next.y) != walltile) continue;
      if (next.x == 0 || next.x == map.get_width() - 1 || next.y == 0 || next.y == map.get_height() - 1) continue;

      int up = dir.y <= 0 ? 1 : 0;
      int down = dir.y >= 0 ? 1 : 0;
      int left = dir.x <= 0 ? 1 : 0;
      int right = dir.x >= 0 ? 1 : 0;

      std::vector<vec2i> neigh = map.get_neighbours(next.x, next.y, up, down, left, right);
      bool enclosed = true;
      for (vec2i n : neigh) {
        if (map.get_tile(n.x, n.y) != walltile) {
          enclosed = false;
          break;
        }
      }

      if (enclosed) {
        options.emplace_back(next.x, next.y);
      }
    }
    if (!options.empty()) {
      stack.emplace_back(options.at(rng.get_int(options.size() - 1)));
    }
    else {
      stack.pop_back();
    }
  }

}

Tilemap generate_dungeon(int width, int height) {
  return generate_dungeon(std::random_device()(), width, height);
}

Tilemap generate_dungeon(unsigned int seed, int width, int height) {
  Tilemap map = Tilemap(width, height);

  // Set the whole map to walls
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      map.set_tile(x, y, walltile);
    }
  }

  Rng rng = Rng(seed);

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
        map.set_tile(x, y, floortile);
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
        if (map.get_tile(n.x, n.y) == walltile) count++;
      }
      // If this tile is a wall and is completely surrounded by other walls, start generating a maze here.
      if (count >= 8) {
        maze_fill(map, x, y, rng);
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
          if (map.get_tile(r.pos.x+x+dx, r.pos.y+y+dy) == floortile) {
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

    // Pick up to 3 spots and place doorss
    int doors_amount = potential_doors.size() < 3 ? potential_doors.size() : 4;
    doors_amount = rng.get_int(2, doors_amount);

    for (int i = 0; i < doors_amount; i++) {
    //for (int i = 0; i < potential_doors.size(); i++) {
      if (potential_doors.empty()) break;

      int r = rng.get_int(potential_doors.size()-1);
      vec2i pos = potential_doors.at(r);
      map.set_tile(pos.x, pos.y, doortile);
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
  for (int y = 0; y < map.get_height(); y++) {
    for (int x = 0; x < map.get_width(); x++) {
      std::vector<vec2i> neigh{vec2i(x + 1, y), vec2i(x, y + 1), vec2i(x - 1, y), vec2i(x, y - 1) };
      int count = 0;
      for (vec2i pos : neigh) {
        if (map.get_tile(pos.x, pos.y) == walltile) {
          count++;
        }
      }
      if (count >= neigh.size() - 1) {
        dead_ends.emplace_back(vec2i(x, y));
      }
    }
  }
  int pass_amount = 100;
  for (int pass = 0; pass < pass_amount; pass++) {
    if (dead_ends.empty()) break;
    std::vector<vec2i> new_dead_ends;
    for (vec2i pos : dead_ends) {
      std::vector<vec2i> neigh { vec2i(pos.x + 1, pos.y), vec2i(pos.x, pos.y + 1), vec2i(pos.x - 1, pos.y), vec2i(pos.x, pos.y - 1) };
      int count = 0;
      vec2i next;
      for (vec2i n : neigh) {
        if (map.get_tile(n.x, n.y) == walltile) {
          continue;
        }
        else {
          count++;
          next = n;
        }
      }
      if (count == 1) {
        map.set_tile(pos.x, pos.y, walltile);
        new_dead_ends.emplace_back(next);
      }
      else if (count == 0) {
        map.set_tile(pos.x, pos.y, walltile);
      }
    }
    dead_ends = new_dead_ends;
  }

  /* flood-fill the map to see that you can actually reach everywhere
  bool started = false;
  for (int x = 0; x < map.get_width(); x++) {
    for (int y = 0; y < map.get_height(); y++) {
      if (map.get_tile(x,y) == floortile) {
        std::vector<vec2i> stack{vec2i(x,y)};
        map.set_tile(x, y, testtile);
        while (!stack.empty()) {
          vec2i pos = stack.back();
          stack.pop_back();

          auto neigh = map.get_neighbours(pos.x, pos.y);
          for (vec2i n : neigh) {
            char tile = map.get_tile(n.x, n.y);
            if (tile == floortile || tile == doortile) {
              map.set_tile(pos.x, pos.y, testtile);
              stack.emplace_back(n);
            }
          }
        }

        started = true;
        break;
      }
    }
    if (started) break;
  }
  //*/
  return map;
}
