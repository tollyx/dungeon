#include "Rng.h"
#include <random>
#include <chrono>

std::random_device rd;

unsigned int Rng::get_random_seed() {
  return rd();
}

Rng::Rng() {
  Rng(get_random_seed(), 0);
}


Rng::Rng(unsigned int seed, unsigned int step) {
  this->seed = seed;
  this->step = step;
  mte = std::mt19937(seed);
  for (unsigned int i = 0; i < step; i++) {
    get_int(100);
  }
}

Rng::~Rng() {}

int Rng::get_int(int max) {
  return get_int(0, max);
}

int Rng::get_int(int min, int max) {
  step++;
  return std::uniform_int_distribution<int>{min, max}(mte);
}

float Rng::get_float(float max) {
  return get_float(0, max);
}

float Rng::get_float(float min, float max) {
  step++;
  return std::uniform_real_distribution<float>{min, max}(mte);
}
