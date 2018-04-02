#pragma once
#include <string>

namespace Log {
  using std::string;

  enum LogLevel {
    Error,
    Warning,
    Info,
    Debug,
    Trace,
  };

  void set_log_level(LogLevel level);
  //void enable_log_to_file(bool enable);
  //void set_log_path(string path);
  void error(string tag, string message);
  void warning(string tag, string message);
  void info(string tag, string message);
  void debug(string tag, string message);
  void trace(string tag, string message);
}
