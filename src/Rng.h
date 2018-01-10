#pragma once

#include <random>

class Rng {
  unsigned int seed = 0;
  unsigned int step = 0;
  std::mt19937 mte;
public:
  static unsigned int get_random_seed();

  Rng();
  Rng(unsigned int seed, unsigned int step = 0);
  ~Rng();

  int get_int(int max = 100);
  int get_int(int min, int max);
  float get_float(float max = 1);
  float get_float(float min, float max);
};

