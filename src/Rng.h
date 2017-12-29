#pragma once

#include <random>

class Rng {
  unsigned int seed = 0;
  unsigned int step = 0;
  std::mt19937 mte;
public:
  Rng();
  Rng(unsigned int seed, unsigned int step = 0);
  ~Rng();

  int get_int(int max);
  int get_int(int min, int max);
};

