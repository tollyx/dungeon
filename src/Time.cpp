#include "Time.h"
#include <vector>

std::vector<Time::Timer> timers;

void Time::Timer::tick(float dt) {
  time_current += dt;
  if (continuous) {
    callback();
  }
  else if (time_current >= duration) {
    callback();
    if (loop && !is_finished()) {
      count++;
      time_current -= duration;
    }
  }
}

bool Time::Timer::is_finished() {
  return (loop && count >= limit) || (!loop && time_current >= duration);
}

void Time::tick_timers(float dt) {
  for (int i = timers.size() - 1; i >= 0; i--) {
    timers[i].tick(dt);
    if (timers[i].is_finished()) {
      timers.erase(timers.begin() + i);
    }
  }
}

void Time::after(float seconds, TimerCallback callback) {
  timers.emplace_back(Timer {
      false,
      false,
      0,
      0,
      0,
      seconds,
      callback,
    });
}

void Time::every(float seconds, int limit, TimerCallback callback) {
  timers.emplace_back(Timer{
    true,
    false,
    limit,
    0,
    0,
    seconds,
    callback,
    });
}

void Time::during(float seconds, TimerCallback callback) {
  timers.emplace_back(Timer{
    false,
    true,
    0,
    0,
    0,
    seconds,
    callback,
    });
}
