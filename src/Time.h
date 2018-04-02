#pragma once

#include <functional>

namespace Time {
  typedef std::function<void()> TimerCallback;
  struct Timer {
    bool loop;
    bool continuous;
    int limit;
    int count;
    float time_current;
    float duration;
    TimerCallback callback;
    void tick(float dt);
    bool is_finished();
  };

  void tick_timers(float dt);
  void after(float seconds, TimerCallback callback);
  void every(float seconds, int limit, TimerCallback callback);
  void during(float seconds, TimerCallback callback);
}
